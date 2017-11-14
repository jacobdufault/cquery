'use strict';

import * as path from 'path';

import * as vscode from 'vscode';
import * as vscodelc from 'vscode-languageclient';
import {Message} from 'vscode-jsonrpc';

type Nullable<T> = T|null;

// Increment version number whenever we want to make sure the user updates the
// extension. cquery will emit an error notification if this does not match its
// internal number.
const VERSION = 3


function assert(condition: boolean) {
  if (!condition)
    throw new Error();
}

class MyErrorHandler implements vscodelc.ErrorHandler {
  constructor(readonly config: vscode.WorkspaceConfiguration) {}

  error(error: Error, message: Message, count: number): vscodelc.ErrorAction {
    return vscodelc.ErrorAction.Continue;
  }

  closed(): vscodelc.CloseAction {
    const restart = this.config.get('launch.autoRestart');

    if (this.config.get('launch.notifyOnCrash')) {
      if (restart)
        vscode.window.showInformationMessage(
            'cquery has crashed; it has been restarted.');
      else
        vscode.window.showInformationMessage(
            'cquery has crashed; it has not been restarted.');
    }

    if (restart)
      return vscodelc.CloseAction.Restart;
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

enum SymbolType {
  Type = 0,
  Function,
  Variable
}
class Symbol {
  constructor(
      readonly type: SymbolType, readonly isTypeMember: boolean,
      readonly ranges: Array<vscode.Range>) {}
}
function parseSymbol(u): Symbol {
  return new Symbol(u.type, u.is_type_member, parseRanges(u.ranges));
}

function jumpToUriAtPosition(
    uri: vscode.Uri, position: vscode.Position, preserveFocus: boolean) {
  vscode.workspace.openTextDocument(uri).then(d => {
    if (!d) {
      vscode.window.activeTextEditor.revealRange(
          new vscode.Range(position, position),
          vscode.TextEditorRevealType.InCenter);
      vscode.window.activeTextEditor.selection =
          new vscode.Selection(position, position);
    } else {
      vscode.window.showTextDocument(d, undefined, preserveFocus).then(e => {
        e.revealRange(
            new vscode.Range(position, position),
            vscode.TextEditorRevealType.InCenter);
        e.selection = new vscode.Selection(position, position);
      })
    }
  })
}

function getClientConfig(context: vscode.ExtensionContext) {
  let config = vscode.workspace.getConfiguration('cquery');

  if (config.get('launch.workingDirectory') == '') {
    const kOpenSettings = 'Open Settings';
    vscode.window
        .showErrorMessage(
            'Please specify the "cquery.launch.workingDirectory" setting and reload vscode',
            kOpenSettings)
        .then(selected => {
          if (selected == kOpenSettings)
            vscode.commands.executeCommand(
                'workbench.action.openWorkspaceSettings');
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
    resourceDirectory: config.get('misc.resourceDirectory'),
    maxWorkspaceSearchResults: config.get('misc.maxWorkspaceSearchResults'),
    indexerCount: config.get('misc.indexerCount'),
    enableIndexing: config.get('misc.enableIndexing'),
    enableCacheWrite: config.get('misc.enableCacheWrite'),
    enableCacheRead: config.get('misc.enableCacheRead'),
    includeCompletionMaximumPathLength:
        config.get('completion.include.maximumPathLength'),
    includeCompletionWhitelistLiteralEnding:
        config.get('completion.include.whitelistLiteralEnding'),
    includeCompletionWhitelist: config.get('completion.include.whitelist'),
    includeCompletionBlacklist: config.get('completion.include.blacklist'),
    showDocumentLinksOnIncludes: config.get('showDocumentLinksOnIncludes'),
    diagnosticsOnParse: config.get('diagnostics.onParse'),
    diagnosticsOnCodeCompletion: config.get('diagnostics.onCodeCompletion'),
    codeLensOnLocalVariables: config.get('codeLens.onLocalVariables'),
  }

  if (!clientConfig.cacheDirectory) {
    if (!context.storagePath) {
      const kOpenSettings = 'Open Settings';
      vscode.window
          .showErrorMessage(
              'Could not auto-discover cache directory. Please use "Open Folder" ' +
                  'or specify it in the |cquery.cacheDirectory| setting.',
              kOpenSettings)
          .then((selected) => {
            if (selected == kOpenSettings)
              vscode.commands.executeCommand(
                  'workbench.action.openWorkspaceSettings');
          });
      return;
    }

    // Provide a default cache directory if it is not present. Insert next to
    // the project since if the user has an SSD they most likely have their
    // source files on the SSD as well.
    const generateCacheDirectory = () => {
      let workspaceDir = vscode.workspace.rootPath.replace(/\\/g, '/');
      if (!workspaceDir.endsWith('/'))
        workspaceDir += '/';
      return workspaceDir + '.vscode/cquery_cached_index/'
    };
    let cacheDir = generateCacheDirectory();
    clientConfig.cacheDirectory = cacheDir;
    config.update('cacheDirectory', cacheDir, false /*global*/);
  }

  return clientConfig;
}



class TypeHierarchyNode {
  constructor(
      readonly name: string, readonly location: vscode.Location|undefined,
      readonly children: TypeHierarchyNode[]) {}

  get usr() {
    return this.name;
  }
}

class TypeHierarchyProvider implements
    vscode.TreeDataProvider<TypeHierarchyNode> {
  root: TypeHierarchyNode[] = [];

  readonly onDidChangeEmitter: vscode.EventEmitter<any> =
      new vscode.EventEmitter<any>();
  readonly onDidChangeTreeData: vscode.Event<any> =
      this.onDidChangeEmitter.event;

  getTreeItem(element: TypeHierarchyNode): vscode.TreeItem {
    const kBaseName = '[[Base]]'

    let collapseState = vscode.TreeItemCollapsibleState.None;
    if (element.children.length > 0)
      collapseState = vscode.TreeItemCollapsibleState.Expanded;
    if (element.children && element.name == kBaseName) {
      collapseState = vscode.TreeItemCollapsibleState.Collapsed;
    } else if (
        element.children.length == 1 && element.children[0].name == kBaseName) {
      assert(element.name != kBaseName);
      collapseState = vscode.TreeItemCollapsibleState.Collapsed;
    }

    return {
      label: element.name,
      collapsibleState: collapseState,
      contextValue: 'cqueryGoto',
      command: {
        command: '_cquery._hackGotoForTreeView',
        title: 'Goto',
        arguments: [element, element.children.length != 0]
      }
    };
  }

  getChildren(element?: TypeHierarchyNode):
      TypeHierarchyNode[]|Thenable<TypeHierarchyNode[]> {
    if (!element)
      return this.root;
    return element.children;
  }
}



enum CallType {
  Normal = 0,
  Base = 1,
  Derived = 2
}
class CallTreeNode {
  // These properties come directly from the langauge server.
  name: string
  usr: string
  location: vscode.Location
  hasCallers: boolean
  callType: CallType

  // Cached state, local to just the extension.
  _depth: number = 0
  _cachedCallers: CallTreeNode[]|undefined
}

class CallTreeProvider implements vscode.TreeDataProvider<CallTreeNode> {
  root: CallTreeNode[] = [];

  constructor(
      readonly languageClient: vscodelc.LanguageClient,
      readonly derivedDark: string, readonly derivedLight: string,
      readonly baseDark: string, readonly baseLight: string) {}

  readonly onDidChangeEmitter: vscode.EventEmitter<any> =
      new vscode.EventEmitter<any>();
  readonly onDidChangeTreeData: vscode.Event<any> =
      this.onDidChangeEmitter.event;

  getTreeItem(element: CallTreeNode): vscode.TreeItem {
    let collapseState = vscode.TreeItemCollapsibleState.None
    if (element.hasCallers) {
      if (element._depth < 2)
        collapseState = vscode.TreeItemCollapsibleState.Expanded;
      else
        collapseState = vscode.TreeItemCollapsibleState.Collapsed;
    }

    let light = '';
    let dark = '';
    if (element.callType == CallType.Base) {
      light = this.baseLight;
      dark = this.baseDark;
    } else if (element.callType == CallType.Derived) {
      light = this.derivedLight;
      dark = this.derivedDark;
    }
    return {
      label: element.name,
      collapsibleState: collapseState,
      contextValue: 'cqueryGoto',
      command: {
        command: '_cquery._hackGotoForTreeView',
        title: 'Goto',
        arguments: [element, element.hasCallers]
      },
      iconPath: {light: light, dark: dark}
    };
  }

  getChildren(element?: CallTreeNode): CallTreeNode[]|Thenable<CallTreeNode[]> {
    if (!element)
      return this.root;
    if (element._cachedCallers !== undefined)
      return element._cachedCallers;

    return this.languageClient
        .sendRequest('$cquery/callTreeExpand', {usr: element.usr})
        .then((result: CallTreeNode[]) => {
          for (let child of result)
            child._depth = element._depth + 1;
          element._cachedCallers = result;
          return result;
        });
  }
}



export function activate(context: vscode.ExtensionContext) {
  /////////////////////////////////////
  // Setup configuration, start server.
  /////////////////////////////////////

  // Load configuration. Tell user they need to restart vscode if they change
  // configuration values.
  let clientConfig = getClientConfig(context);
  context.subscriptions.push(vscode.workspace.onDidChangeConfiguration(() => {
    let newConfig = getClientConfig(context);
    for (let key in newConfig) {
      if (!newConfig.hasOwnProperty(key))
        continue;

      if (!clientConfig ||
          JSON.stringify(clientConfig[key]) != JSON.stringify(newConfig[key])) {
        const kReload = 'Reload'
        vscode.window
            .showInformationMessage(
                `Please reload to apply cquery configuration change (${
                                                                       key
                                                                     } changed).`,
                kReload)
            .then((selected) => {
              if (selected == kReload)
                vscode.commands.executeCommand('workbench.action.reloadWindow');
            });
        break;
      }
    }
  }));
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
  };
  console.log(
      `Starting ${serverOptions.command} in ${serverOptions.options.cwd}`);

  // Options to control the language client
  let clientOptions: vscodelc.LanguageClientOptions = {
    documentSelector: ['c', 'cpp'],
    // synchronize: {
    // 	configurationSection: 'cquery',
    // 	fileEvents: workspace.createFileSystemWatcher('**/.cc')
    // },
    diagnosticCollectionName: 'cquery',
    outputChannelName: 'cquery',
    revealOutputChannelOn: vscodelc.RevealOutputChannelOn.Never,
    initializationOptions: clientConfig,
    initializationFailedHandler: (e) => {
      console.log(e);
      return false;
    },
    errorHandler:
        new MyErrorHandler(vscode.workspace.getConfiguration('cquery'))
  }

  // Create the language client and start the client.
  let languageClient = new vscodelc.LanguageClient(
      'cquery', 'cquery', serverOptions, clientOptions);
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
      languageClient
          .sendRequest(methodName, {
            textDocument: {
              uri: uri.toString(),
            },
            position: position
          })
          .then((locations: Array<object>) => {
            if (autoGotoIfSingle && locations.length == 1) {
              let location = parseLocation(locations[0])
              vscode.commands.executeCommand(
                  'cquery.goto', location.uri, location.range.start, []);
            } else {
              vscode.commands.executeCommand(
                  'editor.action.showReferences', uri, position,
                  parseLocations(locations));
            }
          })
    }
  }
  vscode.commands.registerCommand(
      'cquery.vars', makeRefHandler('$cquery/vars'));
  vscode.commands.registerCommand(
      'cquery.callers', makeRefHandler('$cquery/callers'));
  vscode.commands.registerCommand(
      'cquery.base', makeRefHandler('$cquery/base', true));
  vscode.commands.registerCommand(
      'cquery.derived', makeRefHandler('$cquery/derived'));

  // The language client does not correctly deserialize arguments, so we have a
  // wrapper command that does it for us.
  vscode.commands.registerCommand(
      'cquery.showReferences', (uri, position, locations) => {
        vscode.commands.executeCommand(
            'editor.action.showReferences', parseUri(uri),
            parsePosition(position), parseLocations(locations));
      });


  vscode.commands.registerCommand('cquery.goto', (uri, position, locations) => {
    let parsedPosition = parsePosition(position);
    let parsedUri = parseUri(uri);
    jumpToUriAtPosition(parsedUri, parsedPosition, false /*preserveFocus*/);
  });


  // FixIt support
  vscode.commands.registerCommand('cquery._applyFixIt', (uri, pTextEdits) => {
    const textEdits = p2c.asTextEdits(pTextEdits);

    function applyEdits(e: vscode.TextEditor) {
      e.edit(editBuilder => {
         for (const edit of textEdits)
           editBuilder.replace(edit.range, edit.newText);
       }).then(success => {
        if (!success)
          vscode.window.showErrorMessage('Failed to apply FixIt');
      });
    }

    // Find existing open document.
    for (const textEditor of vscode.window.visibleTextEditors) {
      if (textEditor.document.uri.toString() == uri) {
        applyEdits(textEditor);
        return;
      }
    }

    // Failed, open new document.
    vscode.workspace.openTextDocument(parseUri(uri))
        .then(d => {vscode.window.showTextDocument(d).then(e => {
                if (!e)
                  vscode.window.showErrorMessage(
                      'Failed to to get editor for FixIt');

                applyEdits(e);
              })});
  });


  // AutoImplement
  vscode.commands.registerCommand(
      'cquery._autoImplement', (uri, pTextEdits) => {
        vscode.commands.executeCommand('cquery._applyFixIt', uri, pTextEdits)
            .then(() => {
              vscode.commands.executeCommand(
                  'cquery.goto', uri, pTextEdits[0].range.start);
            });
      });


  // Insert include.
  vscode.commands.registerCommand(
      'cquery._insertInclude', (uri, pTextEdits) => {
        if (pTextEdits.length == 1)
          vscode.commands.executeCommand('cquery._applyFixIt', uri, pTextEdits);
        else {
          let items: Array<vscode.QuickPickItem> = [];
          class MyQuickPick implements vscode.QuickPickItem {
            constructor(
                public label: string, public description: string,
                public edit: any) {}
          }
          for (let edit of pTextEdits) {
            items.push(new MyQuickPick(edit.newText, '', edit));
          }
          vscode.window.showQuickPick(items).then((selected: MyQuickPick) => {
            vscode.commands.executeCommand(
                'cquery._applyFixIt', uri, [selected.edit]);
          });
        }
      });


  // Inactive regions.
  let config = vscode.workspace.getConfiguration('cquery');
  const inactiveRegionDecorationType =
      vscode.window.createTextEditorDecorationType({
        isWholeLine: true,
        light: {
          color: <string>config.get('theme.light.inactiveRegion.textColor'),
          backgroundColor:
              <string>config.get('theme.light.inactiveRegion.backgroundColor'),
        },
        dark: {
          color: <string>config.get('theme.dark.inactiveRegion.textColor'),
          backgroundColor:
              <string>config.get('theme.dark.inactiveRegion.backgroundColor'),
        }
      });
  languageClient.onReady().then(() => {
    languageClient.onNotification('$cquery/setInactiveRegions', (args) => {
      let uri = args.uri;
      let ranges = parseRanges(args.inactiveRegions);
      for (const textEditor of vscode.window.visibleTextEditors) {
        if (textEditor.document.uri.toString() == uri) {
          vscode.window.activeTextEditor.setDecorations(
              inactiveRegionDecorationType, ranges);
          break;
        }
      }
    });
  });


  // Progress
  let statusStyle = config.get('misc.status');
  if (statusStyle == 'short' || statusStyle == 'detailed') {
    let statusIcon =
        vscode.window.createStatusBarItem(vscode.StatusBarAlignment.Right);
    statusIcon.text = 'cquery: loading';
    statusIcon.tooltip =
        'cquery is loading project metadata (ie, compile_commands.json)';
    statusIcon.show();
    languageClient.onReady().then(() => {
      languageClient.onNotification('$cquery/progress', (args) => {
        let indexRequestCount = args.indexRequestCount;
        let doIdMapCount = args.doIdMapCount;
        let loadPreviousIndexCount = args.loadPreviousIndexCount;
        let onIdMappedCount = args.onIdMappedCount;
        let onIndexedCount = args.onIndexedCount;
        let total = indexRequestCount + doIdMapCount + loadPreviousIndexCount +
            onIdMappedCount + onIndexedCount;


        let detailedJobString = `indexRequest: ${indexRequestCount}, ` +
            `doIdMap: ${doIdMapCount}, ` +
            `loadPreviousIndex: ${loadPreviousIndexCount}, ` +
            `onIdMapped: ${onIdMappedCount}, ` +
            `onIndexed: ${onIndexedCount}`;

        if (total == 0 && statusStyle == 'short') {
          statusIcon.text = 'cquery: idle';
        } else {
          statusIcon.text = `cquery: ${total} jobs`;
          if (statusStyle == 'detailed') {
            statusIcon.text += ` (${detailedJobString})`
          }
        }
        statusIcon.tooltip = 'cquery jobs: ' + detailedJobString;
      });
    });
  }

  // Type hierarchy.
  const typeHierarchyProvider = new TypeHierarchyProvider();
  vscode.window.registerTreeDataProvider(
      'cquery.typeHierarchy', typeHierarchyProvider);
  // TODO: support showing base types, evaluate them on demand though as it may
  // result in cyclic graphics.
  vscode.commands.registerCommand('cquery.typeHierarchyTree', () => {
    let position = vscode.window.activeTextEditor.selection.active;
    let uri = vscode.window.activeTextEditor.document.uri;
    languageClient
        .sendRequest('$cquery/typeHierarchyTree', {
          textDocument: {
            uri: uri.toString(),
          },
          position: position
        })
        .then((typeEntry: TypeHierarchyNode | undefined) => {
          if (typeEntry) {
            typeHierarchyProvider.root = [];
            typeHierarchyProvider.root.push(typeEntry);
            typeHierarchyProvider.onDidChangeEmitter.fire();
          }
        })
  });
  vscode.commands.registerCommand('cquery.clearTypeHierarchyTree', () => {
    typeHierarchyProvider.root = [];
    typeHierarchyProvider.onDidChangeEmitter.fire();
  });


  // Call tree
  let derivedDark =
      context.asAbsolutePath(path.join('resources', 'derived-dark.svg'));
  let derivedLight =
      context.asAbsolutePath(path.join('resources', 'derived-light.svg'));
  let baseDark =
      context.asAbsolutePath(path.join('resources', 'base-dark.svg'));
  let baseLight =
      context.asAbsolutePath(path.join('resources', 'base-light.svg'));
  const callTreeProvider = new CallTreeProvider(
      languageClient, derivedDark, derivedLight, baseDark, baseLight);
  vscode.window.registerTreeDataProvider('cquery.callTree', callTreeProvider);
  vscode.commands.registerCommand('cquery.callTree', () => {
    let position = vscode.window.activeTextEditor.selection.active;
    let uri = vscode.window.activeTextEditor.document.uri;
    languageClient
        .sendRequest('$cquery/callTreeInitial', {
          textDocument: {
            uri: uri.toString(),
          },
          position: position
        })
        .then((callNodes: CallTreeNode[]) => {
          callTreeProvider.root = [];
          for (let callNode of callNodes) {
            callNode._depth = 0
            callTreeProvider.root.push(callNode);
          }
          if (callNodes)
            callTreeProvider.onDidChangeEmitter.fire();
        });
  });
  vscode.commands.registerCommand('cquery.clearCallTree', () => {
    callTreeProvider.root = [];
    callTreeProvider.onDidChangeEmitter.fire();
  });

  vscode.commands.registerCommand(
      '_cquery._gotoForTreeView', (node: TypeHierarchyNode | CallTreeNode) => {
        if (!node.location)
          return;

        let parsedUri = parseUri(node.location.uri);
        let parsedPosition = parsePosition(node.location.range.start);

        jumpToUriAtPosition(parsedUri, parsedPosition, true /*preserveFocus*/)
      });

  let lastGotoNodeUsr: string
  let lastGotoClickTime: number
  vscode.commands.registerCommand(
      '_cquery._hackGotoForTreeView',
      (node: TypeHierarchyNode | CallTreeNode, hasChildren: boolean) => {
        if (!node.location)
          return;

        if (!hasChildren) {
          vscode.commands.executeCommand('_cquery._gotoForTreeView', node);
          return;
        }

        if (lastGotoNodeUsr != node.usr) {
          lastGotoNodeUsr = node.usr;
          lastGotoClickTime = Date.now();
          return;
        }

        const kDoubleClickTimeMs = config.get('treeViews.doubleClickTimeoutMs');
        const elapsed = Date.now() - lastGotoClickTime;
        lastGotoClickTime = Date.now();
        if (elapsed < kDoubleClickTimeMs)
          vscode.commands.executeCommand('_cquery._gotoForTreeView', node);
      });


  // Semantic highlighting
  // TODO:
  //   - enable bold/italic decorators, might need change in vscode
  //   - only function call icon if the call is implicit
  //   - get color of variable based on its USR
  let makeSemanticDecorationType =
      (color: Nullable<string>, underline: boolean,
       showIcon: boolean): vscode.TextEditorDecorationType => {

        return vscode.window.createTextEditorDecorationType({
          isWholeLine: false,
          rangeBehavior: vscode.DecorationRangeBehavior.OpenOpen,
          color: color,
          textDecoration: underline ? 'underline' : '',
          after: {
            contentIconPath: showIcon ?
                context.asAbsolutePath('resources/function_call.svg') :
                '',
          }
        });
      };
  const typeDecorationType = makeSemanticDecorationType(
      'red' /*color*/, false /*underline*/, false /*showIcon*/);
  const typeFuncDecorationType = makeSemanticDecorationType(
      null /*color*/, false /*underline*/, true /*showIcon*/);

  let varColors =
      config.get<Array<string>>('semanticHighlighting.variables.colors');

  const localVarDecorationTypes = varColors.map(
      (color) => makeSemanticDecorationType(
          color /*color*/, false /*underline*/, false /*showIcon*/));
  const typeVarDecorationTypes = varColors.map(
      (color) => makeSemanticDecorationType(
          color /*color*/, true /*underline*/, false /*showIcon*/));

  let lastLocalVarDecorationType = 0;
  let lastTypeVarDecorationType = 0;

  let semanticConfig = {type: false, func: false, var : false};
  let updateConfigValues = () => {
    let get = (name) => config.get<boolean>(name, false);
    semanticConfig.type = get('semanticHighlighting.types.enabled');
    semanticConfig.func = get('semanticHighlighting.functions.enabled');
    semanticConfig.var = get('semanticHighlighting.variables.enabled');
  };
  updateConfigValues();

  let tryGetDecorationType =
      (symbol: Symbol): Nullable<vscode.TextEditorDecorationType> => {
        switch (symbol.type) {
          case SymbolType.Type:
            if (!semanticConfig.type)
              return null;
            return typeDecorationType;
          case SymbolType.Function:
            if (!semanticConfig.func)
              return null;
            if (symbol.isTypeMember)
              return typeFuncDecorationType;
            return null;
          case SymbolType.Variable:
            if (!semanticConfig.var)
              return null;
            if (symbol.isTypeMember) {
              return typeVarDecorationTypes
                  [lastTypeVarDecorationType++ % varColors.length];
            }
            return localVarDecorationTypes
                [lastLocalVarDecorationType++ % varColors.length];
        }
      };

  let allDecorationTypes = [
    typeDecorationType, typeFuncDecorationType, ...localVarDecorationTypes,
    ...typeVarDecorationTypes
  ];

  languageClient.onReady().then(() => {
    languageClient.onNotification(
        '$cquery/publishSemanticHighlighting', (args) => {
          updateConfigValues();
          lastLocalVarDecorationType = 0;
          lastTypeVarDecorationType = 0;

          let uri = args.uri;
          if (vscode.window.activeTextEditor.document.uri.toString() == uri) {
            let decorations =
                new Map<vscode.TextEditorDecorationType, Array<vscode.Range>>();

            for (let symbol0 of args.symbols) {
              let symbol: Symbol = parseSymbol(symbol0);
              let type = tryGetDecorationType(symbol);
              if (!type)
                continue;
              if (decorations.has(type)) {
                let existing = decorations.get(type);
                for (let range of symbol.ranges)
                  existing.push(range);
              } else {
                decorations.set(type, symbol.ranges);
              }
            }

            // Clear decorations and set new ones. We might not use all of the
            // decorations so clear before setting.
            allDecorationTypes.forEach((type) => {
              vscode.window.activeTextEditor.setDecorations(type, []);
            });
            decorations.forEach((ranges, type) => {
              vscode.window.activeTextEditor.setDecorations(type, ranges);
            });
          }
        });
  });
}