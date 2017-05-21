'use strict';

import * as path from 'path';

import * as vscode from 'vscode';
import * as vscodelc from 'vscode-languageclient';
import { Message } from 'vscode-jsonrpc';

// Increment version number whenever we want to make sure the user updates the
// extension. cquery will emit an error notification if this does not match its
// internal number.
const VERSION = 1

class MyErrorHandler implements vscodelc.ErrorHandler {
    error(error: Error, message: Message, count: number): vscodelc.ErrorAction {
      console.log('!!!! error');
      console.log(error);
      console.log(message);
      console.log(count);
      return vscodelc.ErrorAction.Continue;
    }
    closed(): vscodelc.CloseAction	{
      return vscodelc.CloseAction.DoNotRestart;
    }
}

function parsePosition(p): vscode.Position {
  return new vscode.Position(p.line, p.character);
}
function parseRange(r): vscode.Range {
  return new vscode.Range(parsePosition(r.start), parsePosition(r.end));
}
function parseRanges(rs): Array<vscode.Range> {
  let parsed = [];
  for (let r of rs)
    parsed.push(parseRange(r));
  return parsed;
}
function parseLocation(l): vscode.Location {
  return new vscode.Location(parseUri(l.uri), parseRange(l.range));
}
function parseLocations(ls): Array<vscode.Location> {
  let parsed = [];
  for (let l of ls)
    parsed.push(parseLocation(l));
  return parsed;
}
function parseUri(u): vscode.Uri {
  return vscode.Uri.parse(u);
}

function getClientConfig(context: vscode.ExtensionContext) {
  let config = vscode.workspace.getConfiguration('cquery');

  if (config.get('launch.workingDirectory') == '') {
    const kOpenSettings = 'Open Settings';
    vscode.window.showErrorMessage('Please specify the \"launch.workingDirectory\" setting and reload vscode', kOpenSettings).then((selected) => {
      if (selected == kOpenSettings)
        vscode.commands.executeCommand('workbench.action.openWorkspaceSettings');
    });
    return;
  }

  let clientConfig = {
    launchWorkingDirectory: <string>config.get('launch.workingDirectory'),
    launchCommand: <string>config.get('launch.command'),
    cacheDirectory: config.get('cacheDirectory'),
    indexWhitelist: config.get('index.whitelist'),
    indexBlacklist: config.get('index.blacklist'),
    extraClangArguments: config.get('index.extraClangArguments'),
    maxWorkspaceSearchResults: config.get('misc.maxWorkspaceSearchResults'),
    indexerCount: config.get('misc.indexerCount'),
    enableIndexing: config.get('misc.enableIndexing'),
    enableCacheWrite: config.get('misc.enableCacheWrite'),
    enableCacheRead: config.get('misc.enableCacheRead'),
    includeCompletionWhitelistLiteralEnding: config.get('completion.include.whitelistLiteralEnding'),
    includeCompletionWhitelist: config.get('completion.include.whitelist'),
    includeCompletionBlacklist: config.get('completion.include.blacklist'),
    showDocumentLinksOnIncludes: config.get('showDocumentLinksOnIncludes'),
    codeLensOnLocalVariables: config.get('codeLens.onLocalVariables'),
  }

  if (!clientConfig.cacheDirectory) {
    if (!context.storagePath) {
      const kOpenSettings = 'Open Settings';
      vscode.window.showErrorMessage(
          'Could not auto-discover cache directory. Please use "Open Folder" ' +
          'or specify it in the |cquery.cacheDirectory| setting.', kOpenSettings).then((selected) => {
        if (selected == kOpenSettings)
          vscode.commands.executeCommand('workbench.action.openWorkspaceSettings');
      });
      return;
    }

    console.log('Updating cache directory')
    clientConfig.cacheDirectory = context.storagePath.replace(/\\/g, '/') + "/cquerycache/"
    config.update('cacheDirectory',
                  clientConfig.cacheDirectory,
                  false /*global*/).then(() => {
                    console.log('Updated config cache directory');
                  }, (reason) => {
                    console.log('Cache directory update failed');
                    console.log(reason);
                  });
  }

  return clientConfig;
}

export function activate(context: vscode.ExtensionContext) {
  /////////////////////////////////////
  // Setup configuration, start server.
  /////////////////////////////////////

  // Load configuration. Tell user they need to restart vscode if they change
  // configuration values.
  let clientConfig = getClientConfig(context);
  context.subscriptions.push(vscode.workspace.onDidChangeConfiguration(() => {
    let newConfig = getClientConfig(context)
    for (let key in newConfig) {
      if (!newConfig.hasOwnProperty(key)) continue;

      if (!clientConfig || JSON.stringify(clientConfig[key]) != JSON.stringify(newConfig[key])) {
        const kReload = 'Reload'
        vscode.window.showInformationMessage(`Please reload to apply cquery configuration change (${key} changed).`, kReload).then((selected) => {
          if (selected == kReload)
            vscode.commands.executeCommand('workbench.action.reloadWindow');
        });
        break;
      }
    }
  }))
  if (!clientConfig)
    return;

  clientConfig['clientVersion'] = VERSION

  let serverOptions: vscodelc.ServerOptions = {
    command: clientConfig.launchCommand,
    args: ['--language-server'],
    options: {
      cwd: clientConfig.launchWorkingDirectory
      // env: { 'MALLOC_CHECK_': '2' }
    }
  }
  console.log(`Starting ${serverOptions.command} in ${serverOptions.options.cwd}`);

  // Options to control the language client
  let clientOptions: vscodelc.LanguageClientOptions = {
    documentSelector: ['cpp'],
    // synchronize: {
    // 	configurationSection: 'cquery',
    // 	fileEvents: workspace.createFileSystemWatcher('**/.cc')
    // },
    diagnosticCollectionName: 'cquery',
    outputChannelName: 'cquery',
    revealOutputChannelOn: vscodelc.RevealOutputChannelOn.Error,
    initializationOptions: clientConfig,
    initializationFailedHandler: (e) => {
      console.log(e);
      return false;
    },
    errorHandler: new MyErrorHandler()
  }

  // Create the language client and start the client.
  let languageClient = new vscodelc.LanguageClient('cquery', 'cquery', serverOptions, clientOptions);
  let disposable = languageClient.start();
  context.subscriptions.push(disposable);

  let p2c = languageClient.protocol2CodeConverter;

  /////////////////////
  // Register commands.
  /////////////////////

  vscode.commands.registerCommand('cquery.freshenIndex', () => {
    languageClient.sendNotification('$cquery/freshenIndex');
  });

  function makeRefHandler(methodName, autoGotoIfSingle = false) {
    return () => {
      let position = vscode.window.activeTextEditor.selection.active;
      let uri = vscode.window.activeTextEditor.document.uri;
      languageClient.sendRequest(methodName, {
        textDocument: { uri: uri.toString(), },
        position: position
      }).then((locations: Array<object>) => {
        if (autoGotoIfSingle && locations.length == 1) {
          let location = parseLocation(locations[0])
          vscode.commands.executeCommand(
            'cquery.goto',
            location.uri, location.range.start, []);
        } else {
          vscode.commands.executeCommand(
            'editor.action.showReferences',
            uri, position, parseLocations(locations));
        }
      })
    }
  }
  vscode.commands.registerCommand('cquery.vars', makeRefHandler('$cquery/vars'));
  vscode.commands.registerCommand('cquery.callers', makeRefHandler('$cquery/callers'));
  vscode.commands.registerCommand('cquery.base', makeRefHandler('$cquery/base', true));
  vscode.commands.registerCommand('cquery.derived', makeRefHandler('$cquery/derived'));

  // The language client does not correctly deserialize arguments, so we have a
  // wrapper command that does it for us.
  vscode.commands.registerCommand('cquery.showReferences', (uri, position, locations) => {
    vscode.commands.executeCommand(
      'editor.action.showReferences',
      parseUri(uri), parsePosition(position), parseLocations(locations));
  });


  vscode.commands.registerCommand('cquery.goto', (uri, position, locations) => {
    let parsedPosition = parsePosition(position);
    let parsedUri = parseUri(uri);

    vscode.workspace.openTextDocument(parsedUri).then(d => {
      if (!d) {
        vscode.window.activeTextEditor.revealRange(new vscode.Range(parsedPosition, parsedPosition), vscode.TextEditorRevealType.InCenter);
        vscode.window.activeTextEditor.selection = new vscode.Selection(parsedPosition, parsedPosition);
      }
      else {
        vscode.window.showTextDocument(d).then(e => {
          e.revealRange(new vscode.Range(parsedPosition, parsedPosition), vscode.TextEditorRevealType.InCenter);
          e.selection = new vscode.Selection(parsedPosition, parsedPosition);
        })
      }
    })
  });


  // FixIt support
  vscode.commands.registerCommand('cquery._applyFixIt', (uri, pTextEdits) => {
    const textEdits = p2c.asTextEdits(pTextEdits);

    for (const textEditor of vscode.window.visibleTextEditors) {
      if (textEditor.document.uri.toString() == uri) {
        textEditor.edit(editBuilder => {
          for (const edit of textEdits)
            editBuilder.replace(edit.range, edit.newText);
        }).then(success => {
          if (!success)
            vscode.window.showErrorMessage('Failed to apply FixIt');
        });
        return;
      }
    }

    vscode.window.showErrorMessage(`FixIt: could not find editor for ${uri}`);
  });


  // Inactive regions.
  let config = vscode.workspace.getConfiguration('cquery');
	const inactiveRegionDecorationType = vscode.window.createTextEditorDecorationType({
    isWholeLine: true,
    light: {
      color: <string>config.get('theme.light.inactiveRegion.textColor'),
      backgroundColor: <string>config.get('theme.light.inactiveRegion.backgroundColor'),
    },
    dark: {
      color: <string>config.get('theme.dark.inactiveRegion.textColor'),
      backgroundColor: <string>config.get('theme.dark.inactiveRegion.backgroundColor'),
    }
	});
  languageClient.onReady().then(() => {
    languageClient.onNotification('$cquery/setInactiveRegions', (args) => {
      let uri = args.uri;
      let ranges = parseRanges(args.inactiveRegions)
      for (const textEditor of vscode.window.visibleTextEditors) {
        if (textEditor.document.uri.toString() == uri) {
          vscode.window.activeTextEditor.setDecorations(
              inactiveRegionDecorationType, ranges);
          break;
        }
      }
    });
  });
}
