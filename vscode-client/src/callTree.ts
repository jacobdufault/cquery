import {Event, EventEmitter, Location, TreeDataProvider, TreeItem, TreeItemCollapsibleState} from 'vscode';
import {LanguageClient} from 'vscode-languageclient/lib/main';


enum CallType {
  Normal = 0,
  Base = 1,
  Derived = 2
}
export class CallTreeNode {
  // These properties come directly from the langauge server.
  name: string
  usr: string
  location: Location
  hasCallers: boolean
  callType: CallType

  // Cached state, local to just the extension.
  _depth: number = 0
  _cachedCallers: CallTreeNode[]|undefined
}

export class CallTreeProvider implements TreeDataProvider<CallTreeNode> {
  root: CallTreeNode[] = [];

  constructor(
      readonly languageClient: LanguageClient, readonly derivedDark: string,
      readonly derivedLight: string, readonly baseDark: string,
      readonly baseLight: string) {}

  readonly onDidChangeEmitter: EventEmitter<any> = new EventEmitter<any>();
  readonly onDidChangeTreeData: Event<any> = this.onDidChangeEmitter.event;

  getTreeItem(element: CallTreeNode): TreeItem {
    let collapseState = TreeItemCollapsibleState.None
    if (element.hasCallers) {
      if (element._depth < 2)
        collapseState = TreeItemCollapsibleState.Expanded;
      else
        collapseState = TreeItemCollapsibleState.Collapsed;
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
