/* --------------------------------------------------------------------------------------------
 * Copyright (c) Microsoft Corporation. All rights reserved.
 * Licensed under the MIT License. See License.txt in the project root for license information.
 * ------------------------------------------------------------------------------------------ */
'use strict';

import * as path from 'path';

import { commands, window, workspace, Disposable, ExtensionContext, Location, Position, TextEditorRevealType, Uri, Range, Selection } from 'vscode';
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

export function activate(context: ExtensionContext) {
	console.log('Running');

	// commands.getCommands(false).then((cmds: string[]) => {
	// 	console.log(cmds.join(' '));
	// });

	commands.registerCommand('superindex.goto', (uri, position, locations) => {
		function parsePosition(p): Position {
			return new Position(p.line, p.character);
		}
		function parseRange(r): Range {
			return new Range(parsePosition(r.start), parsePosition(r.end));
		}
		function parseLocation(l): Location {
			return new Location(parseUri(l.uri), parseRange(l.range));
		}
		function parseUri(u): Uri {
			return Uri.parse(u);
		}

		let parsedPosition = parsePosition(position);
		// let parsedUri = parseUri(uri + `#${position.line},${position.character}`);
		// let parsedUri = parseUri(uri + `#L${position.line}`);
		let parsedUri = parseUri(uri);

    console.error('setting selection to ' + parsedPosition.line + ',' + parsedPosition.character);

		// const {scheme, path, query, fragment} = parsedUri;
		// console.log(`openTextDocument parsedUri=${parsedUri} scheme=${scheme} path=${path} query=${query} fragment=${fragment}`);
		workspace.openTextDocument(parsedUri).then(d => {
			console.log('got ' + d);

			if (!d) {
				window.activeTextEditor.revealRange(new Range(parsedPosition, parsedPosition), TextEditorRevealType.InCenter);
				window.activeTextEditor.selection = new Selection(parsedPosition, parsedPosition);
			}
			else {
				window.showTextDocument(d).then(e => {
					e.revealRange(new Range(parsedPosition, parsedPosition), TextEditorRevealType.InCenter);
					e.selection = new Selection(parsedPosition, parsedPosition);
				})
			}
		})

		//let parsedLocation = parseLocation(locations[0]);
		//commands.executeCommand(
		//	'editor.action.goToTypeDefinition', parsedUri, parsedLocation);
	});

	// Proxy editor.action.showReferences so we can deserialize arguments into
	// the correct vscode types.
	commands.registerCommand('superindex.showReferences', (uri, position, locations) => {
		function parsePosition(p): Position {
			return new Position(p.line, p.character);
		}
		function parseRange(r): Range {
			return new Range(parsePosition(r.start), parsePosition(r.end));
		}
		function parseLocation(l): Location {
			return new Location(parseUri(l.uri), parseRange(l.range));
		}
		function parseUri(u): Uri {
			return Uri.parse(u);
		}

		let parsedUri = parseUri(uri);
		let parsedPosition = parsePosition(position);
		let parsedLocations = [];
		for (let location of locations)
			parsedLocations.push(parseLocation(location));

		commands.executeCommand(
			'editor.action.showReferences',
			parsedUri, parsedPosition, parsedLocations);
	});



	let serverOptions: ServerOptions = {
		command: 'indexer.exe', args: ['--language-server'], options: {
			cwd: 'C:\\Users\\jacob\\Desktop\\superindex\\indexer\\x64\\Debug',
			// stdio: 'ascii'
    	//stdio?: string | string[];
			//env?: any;
			//detached?: boolean
		}
	}

	// Options to control the language client
	let clientOptions: LanguageClientOptions = {
		documentSelector: ['cpp'],
		// synchronize: {
		// 	configurationSection: 'superindex',
		// 	fileEvents: workspace.createFileSystemWatcher('**/.cc')
		// },

		diagnosticCollectionName: 'superindexdiag',
    outputChannelName: 'superindexoutput',
    revealOutputChannelOn: RevealOutputChannelOn.Info,
    /**
     * The encoding use to read stdout and stderr. Defaults
     * to 'utf8' if ommitted.
     */
    stdioEncoding: 'ascii',
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
	let languageClient = new LanguageClient('superindex', 'Super Index', serverOptions, clientOptions);
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
