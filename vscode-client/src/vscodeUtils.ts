import {Position, Range, Selection, TextEditorRevealType, Uri, window, workspace} from 'vscode';

export function jumpToUriAtPosition(
    uri: Uri, position: Position, preserveFocus: boolean) {
  workspace.openTextDocument(uri).then(d => {
    if (!d) {
      window.activeTextEditor.revealRange(
          new Range(position, position), TextEditorRevealType.InCenter);
      window.activeTextEditor.selection = new Selection(position, position);
    } else {
      window.showTextDocument(d, undefined, preserveFocus).then(e => {
        e.revealRange(
            new Range(position, position), TextEditorRevealType.InCenter);
        e.selection = new Selection(position, position);
      })
    }
  })
}
