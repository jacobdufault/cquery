/* --------------------------------------------------------------------------------------------
 * Copyright (c) Microsoft Corporation. All rights reserved.
 * Licensed under the MIT License. See License.txt in the project root for license information.
 * ------------------------------------------------------------------------------------------ */
'use strict';

import * as path from 'path';

import * as vscode from 'vscode';
import { LanguageClient, LanguageClientOptions, SettingMonitor, ServerOptions, TransportKind, RevealOutputChannelOn, InitializationFailedHandler, ErrorHandler, ErrorAction, CloseAction } from 'vscode-languageclient';
import { Message } from 'vscode-jsonrpc';

class Foo implements ErrorHandler {
    /**
     * An error has occurred while writing or reading from the connection.
     *
     * @param error - the error received
     * @param message - the message to be delivered to the server if know.
     * @param count - a count indicating how often an error is received. Will
     *  be reset if a message got successfully send or received.
     */
    error(error: Error, message: Message, count: number): ErrorAction {
      console.log('!!!! error');
      console.log(error);
      console.log(message);
      console.log(count);
      return ErrorAction.Continue;
    }
    /**
     * The connection to the server got closed.
     */
    closed(): CloseAction	{
      console.log('!!!! closed');
      return CloseAction.DoNotRestart;
    }
}

export function activate(context: vscode.ExtensionContext) {
  console.log('Running');

  // commands.getCommands(false).then((cmds: string[]) => {
  // 	console.log(cmds.join(' '));
  // });

  vscode.commands.registerCommand('superindex.goto', (uri, position, locations) => {
    function parsePosition(p): vscode.Position {
      return new vscode.Position(p.line, p.character);
    }
    function parseRange(r): vscode.Range {
      return new vscode.Range(parsePosition(r.start), parsePosition(r.end));
    }
    function parseLocation(l): vscode.Location {
      return new vscode.Location(parseUri(l.uri), parseRange(l.range));
    }
    function parseUri(u): vscode.Uri {
      return vscode.Uri.parse(u);
    }

    let parsedPosition = parsePosition(position);
    // let parsedUri = parseUri(uri + `#${position.line},${position.character}`);
    // let parsedUri = parseUri(uri + `#L${position.line}`);
    let parsedUri = parseUri(uri);

    console.error('setting selection to ' + parsedPosition.line + ',' + parsedPosition.character);

    // const {scheme, path, query, fragment} = parsedUri;
    // console.log(`openTextDocument parsedUri=${parsedUri} scheme=${scheme} path=${path} query=${query} fragment=${fragment}`);
    vscode.workspace.openTextDocument(parsedUri).then(d => {
      console.log('got ' + d);

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

    //let parsedLocation = parseLocation(locations[0]);
    //commands.executeCommand(
    //	'editor.action.goToTypeDefinition', parsedUri, parsedLocation);
  });

  // Proxy editor.action.showReferences so we can deserialize arguments into
  // the correct vscode types.
  vscode.commands.registerCommand('superindex.showReferences', (uri, position, locations) => {
    function parsePosition(p): vscode.Position {
      return new vscode.Position(p.line, p.character);
    }
    function parseRange(r): vscode.Range {
      return new vscode.Range(parsePosition(r.start), parsePosition(r.end));
    }
    function parseLocation(l): vscode.Location {
      return new vscode.Location(parseUri(l.uri), parseRange(l.range));
    }
    function parseUri(u): vscode.Uri {
      return vscode.Uri.parse(u);
    }

    let parsedUri = parseUri(uri);
    let parsedPosition = parsePosition(position);
    let parsedLocations = [];
    for (let location of locations)
      parsedLocations.push(parseLocation(location));

    vscode.commands.executeCommand(
      'editor.action.showReferences',
      parsedUri, parsedPosition, parsedLocations);
  });

  let config = vscode.workspace.getConfiguration('cquery');
  let clientConfig = {
    cacheDirectory: config.get('cacheDirectory'),
    whitelist: config.get('whitelist'),
    blacklist: config.get('blacklist'),
    indexerCount: config.get('indexerCount'),
    maxWorkspaceSearchResults: config.get('maxWorkspaceSearchResults'),
    extraClangArguments: config.get('extraClangArguments'),
    enableIndexing: config.get('enableIndexing'),
    enableCacheWrite: config.get('enableCacheWrite'),
    enableCacheRead: config.get('enableCacheRead')
  }

  if (!clientConfig.cacheDirectory) {
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

  let serverOptions: ServerOptions = {
      command: 'indexer.exe', args: ['--language-server'], options: {
      cwd: 'C:/Users/jacob/Desktop/superindex/indexer/x64/Debug',
      // stdio: 'ascii'
      //stdio?: string | string[];
      // env: {
      //   'MALLOC_CHECK_': '2'
      // }
      //env?: any;
      //detached?: boolean
    }
  }

  // Options to control the language client
  let clientOptions: LanguageClientOptions = {
    documentSelector: ['cpp'],
    // synchronize: {
    // 	configurationSection: 'cquery',
    // 	fileEvents: workspace.createFileSystemWatcher('**/.cc')
    // },
    diagnosticCollectionName: 'cquery',
    outputChannelName: 'cquery',
    revealOutputChannelOn: RevealOutputChannelOn.Error,
    /**
     * The encoding use to read stdout and stderr. Defaults
     * to 'utf8' if ommitted.
     */
    stdioEncoding: 'ascii',
    initializationOptions: clientConfig,
    // initializationOptions?: any | (() => any);
    initializationFailedHandler: (e) => {
      console.log(e);
      return false;
    },
    errorHandler: new Foo()
    // uriConverters?: {
        // code2Protocol: c2p.URIConverter;
        // protocol2Code: p2c.URIConverter;
    // }
  }

  // Create the language client and start the client.
  let languageClient = new LanguageClient('cquery', 'cquery', serverOptions, clientOptions);
  languageClient.onReady().then(() => {
    console.log('!! onReady');
    // languageClient.onNotification('window/showMessage', (m) => {
      // console.log('!!!! onNotification')
      // console.log(m);
    // });
  });
  console.log("starting");
  let disposable = languageClient.start();
  context.subscriptions.push(disposable);
}
