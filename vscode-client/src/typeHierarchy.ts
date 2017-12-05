import {Event, EventEmitter, Location, TreeDataProvider, TreeItem, TreeItemCollapsibleState} from 'vscode';

export class TypeHierarchyNode {
  constructor(
      readonly name: string, readonly location: Location|undefined,
      readonly children: TypeHierarchyNode[]) {}

  get usr() {
    return this.name;
  }
}

export class TypeHierarchyProvider implements TreeDataProvider<TypeHierarchyNode> {
  root: TypeHierarchyNode[] = [];

  readonly onDidChangeEmitter: EventEmitter<any> = new EventEmitter<any>();
  readonly onDidChangeTreeData: Event<any> = this.onDidChangeEmitter.event;

  getTreeItem(element: TypeHierarchyNode): TreeItem {
    const kBaseName = '[[Base]]'

    let collapseState = TreeItemCollapsibleState.None;
    if (element.children.length > 0)
      collapseState = TreeItemCollapsibleState.Expanded;
    if (element.children && element.name == kBaseName) {
      collapseState = TreeItemCollapsibleState.Collapsed;
    } else if (
        element.children.length == 1 && element.children[0].name == kBaseName) {
      collapseState = TreeItemCollapsibleState.Collapsed;
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