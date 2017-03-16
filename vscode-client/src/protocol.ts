/* --------------------------------------------------------------------------------------------
 * Copyright (c) Microsoft Corporation. All rights reserved.
 * Licensed under the MIT License. See License.txt in the project root for license information.
 * ------------------------------------------------------------------------------------------ */
'use strict';

import { RequestType, RequestType0, NotificationType, NotificationType0 } from 'vscode-jsonrpc';

import {
	TextDocumentContentChangeEvent, Position, Range, Location, Diagnostic, Command,
	TextEdit, WorkspaceEdit, WorkspaceSymbolParams,
	TextDocumentIdentifier, VersionedTextDocumentIdentifier, TextDocumentItem, TextDocumentSaveReason,
	CompletionItem, CompletionList, Hover, SignatureHelp,
	Definition, ReferenceContext, DocumentHighlight, DocumentSymbolParams,
	SymbolInformation, CodeLens, CodeActionContext, FormattingOptions, DocumentLink
} from 'vscode-languageserver-types';

/**
 * A document filter denotes a document by different properties like
 * the [language](#TextDocument.languageId), the [scheme](#Uri.scheme) of
 * its resource, or a glob-pattern that is applied to the [path](#TextDocument.fileName).
 *
 * @sample A language filter that applies to typescript files on disk: `{ language: 'typescript', scheme: 'file' }`
 * @sample A language filter that applies to all package.json paths: `{ language: 'json', pattern: '**package.json' }`
 */
export interface DocumentFilter {
	/**
	 * A language id, like `typescript`.
	 */
	language?: string;

	/**
	 * A Uri [scheme](#Uri.scheme), like `file` or `untitled`.
	 */
	scheme?: string;

	/**
	 * A glob pattern, like `*.{ts,js}`.
	 */
	pattern?: string;
}

/**
 * A document selector is the combination of one or many document filters.
 *
 * @sample `let sel:DocumentSelector = [{ language: 'typescript' }, { language: 'json', pattern: '**∕tsconfig.json' }]`;
 */
export type DocumentSelector = DocumentFilter[];

/**
 * General paramters to to regsiter for an notification or to register a provider.
 */
export interface Registration {
	/**
	 * The id used to register the request. The id can be used to deregister
	 * the request again.
	 */
	id: string;

	/**
	 * The method to register for.
	 */
	method: string;

	/**
	 * Options necessary for the registration.
	 */
	registerOptions?: any;
}

export interface RegistrationParams {
	registrations: Registration[];
}

/**
 * The `client/registerFeature` request is sent from the server to the client to register a new feature
 * handler on the client side.
 */
export namespace RegistrationRequest {
	export const type = new RequestType<RegistrationParams, void, void, void>('client/registerFeature');
}

/**
 * General parameters to unregister a request or notification.
 */
export interface Unregistration {
	/**
	 * The id used to unregister the request or notification. Usually an id
	 * provided during the register request.
	 */
	id: string;

	/**
	 * The method to unregister for.
	 */
	method: string;
}

export interface UnregistrationParams {
	unregisterations: Unregistration[];
}

/**
 * The `client/unregisterFeature` request is sent from the server to the client to unregister a previously registered feature
 * handler on the client side.
 */
export namespace UnregistrationRequest {
	export const type = new RequestType<UnregistrationParams, void, void, void>('client/unregisterFeature');
}

/**
 * A parameter literal used in requests to pass a text document and a position inside that
 * document.
 */
export interface TextDocumentPositionParams {
	/**
	 * The text document.
	 */
	textDocument: TextDocumentIdentifier;

	/**
	 * The position inside the text document.
	 */
	position: Position;
}

//---- Initialize Method ----

/**
 * Workspace specific client capabilities.
 */
export interface WorkspaceClientCapabilites {
	/**
	 * The client supports applying batch edits
	 * to the workspace.
	 */
	applyEdit?: boolean;

	/**
	 * Capabilities specific to the `workspace/didChangeConfiguration` notification.
	 */
	didChangeConfiguration?: {
		/**
		 * Did change configuration notification supports dynamic registration.
		 */
		dynamicRegistration?: boolean;
	};

	/**
	 * Capabilities specific to the `workspace/didChangeWatchedFiles` notification.
	 */
	didChangeWatchedFiles?: {
		/**
		 * Did change watched files notification supports dynamic registration.
		 */
		dynamicRegistration?: boolean;
	};

	/**
	 * Capabilities specific to the `workspace/symbol` request.
	 */
	symbol?: {
		/**
		 * Symbol request supports dynamic registration.
		 */
		dynamicRegistration?: boolean;
	};

	/**
	 * Capabilities specific to the `workspace/executeCommand` request.
	 */
	executeCommand?: {
		/**
		 * Execute command supports dynamic registration.
		 */
		dynamicRegistration?: boolean;
	};
}

/**
 * Text document specific client capabilities.
 */
export interface TextDocumentClientCapabilities {

	synchronization?: {
		/**
		 * Whether text document synchronization supports dynamic registration.
		 */
		dynamicRegistration?: boolean;

		/**
		 * The client supports sending will save notifications.
		 */
		willSave?: boolean;

		/**
		 * The client supports sending a will save request and
		 * waits for a response providing text edits which will
		 * be applied to the document before it is saved.
		 */
		willSaveWaitUntil?: boolean;

		/**
		 * The client supports did save notifications.
		 */
		didSave?: boolean;
	}

	/**
	 * Capabilities specific to the `textDocument/completion`
	 */
	completion?: {
		/**
		 * Whether completion supports dynamic registration.
		 */
		dynamicRegistration?: boolean;

		/**
		 * The client supports the following `CompletionItem` specific
		 * capabilities.
		 */
		completionItem?: {
			/**
			 * Client supports snippets as insert text.
			 *
			 * A snippet can define tab stops and placeholders with `$1`, `$2`
			 * and `${3:foo}`. `$0` defines the final tab stop, it defaults to
			 * the end of the snippet. Placeholders with equal identifiers are linked,
			 * that is typing in one will update others too.
			 */
			snippetSupport?: boolean;
		}
	};

	/**
	 * Capabilities specific to the `textDocument/hover`
	 */
	hover?: {
		/**
		 * Whether hover supports dynamic registration.
		 */
		dynamicRegistration?: boolean;
	};

	/**
	 * Capabilities specific to the `textDocument/signatureHelp`
	 */
	signatureHelp?: {
		/**
		 * Whether signature help supports dynamic registration.
		 */
		dynamicRegistration?: boolean;
	};

	/**
	 * Capabilities specific to the `textDocument/references`
	 */
	references?: {
		/**
		 * Whether references supports dynamic registration.
		 */
		dynamicRegistration?: boolean;
	};

	/**
	 * Capabilities specific to the `textDocument/documentHighlight`
	 */
	documentHighlight?: {
		/**
		 * Whether document highlight supports dynamic registration.
		 */
		dynamicRegistration?: boolean;
	};

	/**
	 * Capabilities specific to the `textDocument/documentSymbol`
	 */
	documentSymbol?: {
		/**
		 * Whether document symbol supports dynamic registration.
		 */
		dynamicRegistration?: boolean;
	};

	/**
	 * Capabilities specific to the `textDocument/formatting`
	 */
	formatting?: {
		/**
		 * Whether formatting supports dynamic registration.
		 */
		dynamicRegistration?: boolean;
	};

	/**
	 * Capabilities specific to the `textDocument/rangeFormatting`
	 */
	rangeFormatting?: {
		/**
		 * Whether range formatting supports dynamic registration.
		 */
		dynamicRegistration?: boolean;
	};

	/**
	 * Capabilities specific to the `textDocument/onTypeFormatting`
	 */
	onTypeFormatting?: {
		/**
		 * Whether on type formatting supports dynamic registration.
		 */
		dynamicRegistration?: boolean;
	};

	/**
	 * Capabilities specific to the `textDocument/definition`
	 */
	definition?: {
		/**
		 * Whether definition supports dynamic registration.
		 */
		dynamicRegistration?: boolean;
	};

	/**
	 * Capabilities specific to the `textDocument/codeAction`
	 */
	codeAction?: {
		/**
		 * Whether code action supports dynamic registration.
		 */
		dynamicRegistration?: boolean;
	};

	/**
	 * Capabilities specific to the `textDocument/codeLens`
	 */
	codeLens?: {
		/**
		 * Whether code lens supports dynamic registration.
		 */
		dynamicRegistration?: boolean;
	};

	/**
	 * Capabilities specific to the `textDocument/documentLink`
	 */
	documentLink?: {
		/**
		 * Whether document link supports dynamic registration.
		 */
		dynamicRegistration?: boolean;
	};

	/**
	 * Capabilities specific to the `textDocument/rename`
	 */
	rename?: {
		/**
		 * Whether rename supports dynamic registration.
		 */
		dynamicRegistration?: boolean;
	};
}

/**
 * Defines the capabilities provided by the client.
 */
export interface ClientCapabilities {
	/**
	 * Workspace specific client capabilities.
	 */
	workspace?: WorkspaceClientCapabilites;

	/**
	 * Text document specific client capabilities.
	 */
	textDocument?: TextDocumentClientCapabilities;

	/**
	 * Experimental client capabilities.
	 */
	experimental?: any;
}

/**
 * Defines how the host (editor) should sync
 * document changes to the language server.
 */
export namespace TextDocumentSyncKind {
	/**
	 * Documents should not be synced at all.
	 */
	export const None = 0;

	/**
	 * Documents are synced by always sending the full content
	 * of the document.
	 */
	export const Full = 1;

	/**
	 * Documents are synced by sending the full content on open.
	 * After that only incremental updates to the document are
	 * send.
	 */
	export const Incremental = 2;
}

export type TextDocumentSyncKind = 0 | 1 | 2;

/**
 * General text document registration options.
 */
export interface TextDocumentRegistrationOptions {
	/**
	 * A document selector to identify the scope of the registration. If set to null
	 * the document selector provided on the client side will be used.
	 */
	documentSelector: DocumentSelector | null;
}

/**
 * Completion options.
 */
export interface CompletionOptions {
	/**
	 * The server provides support to resolve additional
	 * information for a completion item.
	 */
	resolveProvider?: boolean;

	/**
	 * The characters that trigger completion automatically.
	 */
	triggerCharacters?: string[];
}
/**
 * Signature help options.
 */
export interface SignatureHelpOptions {
	/**
	 * The characters that trigger signature help
	 * automatically.
	 */
	triggerCharacters?: string[];
}

/**
 * Code Lens options.
 */
export interface CodeLensOptions {
	/**
	 * Code lens has a resolve provider as well.
	 */
	resolveProvider?: boolean;
}

/**
 * Format document on type options
 */
export interface DocumentOnTypeFormattingOptions {
	/**
	 * A character on which formatting should be triggered, like `}`.
	 */
	firstTriggerCharacter: string;

	/**
	 * More trigger characters.
	 */
	moreTriggerCharacter?: string[];
}

/**
 * Document link options
 */
export interface DocumentLinkOptions {
	/**
	 * Document links have a resolve provider as well.
	 */
	resolveProvider?: boolean;
}

/**
 * Execute command options.
 */
export interface ExecuteCommandOptions {
	/**
	 * The commands to be executed on the server
	 */
	commands: string[]
}

/**
 * Save options.
 */
export interface SaveOptions {
	/**
	 * The client is supposed to include the content on save.
	 */
	includeText?: boolean;
}

export interface TextDocumentSyncOptions {
	/**
	 * Open and close notifications are sent to the server.
	 */
	openClose?: boolean;
	/**
	 * Change notificatins are sent to the server. See TextDocumentSyncKind.None, TextDocumentSyncKind.Full
	 * and TextDocumentSyncKindIncremental.
	 */
	change?: TextDocumentSyncKind;
	/**
	 * Will save notifications are sent to the server.
	 */
	willSave?: boolean;
	/**
	 * Will save wait until requests are sent to the server.
	 */
	willSaveWaitUntil?: boolean;
	/**
	 * Save notifications are sent to the server.
	 */
	save?: SaveOptions;
}

/**
 * Defines the capabilities provided by a language
 * server.
 */
export interface ServerCapabilities {
	/**
	 * Defines how text documents are synced. Is either a detailed structure defining each notification or
	 * for backwards compatibility the TextDocumentSyncKind number.
	 */
	textDocumentSync?: TextDocumentSyncOptions | TextDocumentSyncKind;
	/**
	 * The server provides hover support.
	 */
	hoverProvider?: boolean;
	/**
	 * The server provides completion support.
	 */
	completionProvider?: CompletionOptions;
	/**
	 * The server provides signature help support.
	 */
	signatureHelpProvider?: SignatureHelpOptions;
	/**
	 * The server provides goto definition support.
	 */
	definitionProvider?: boolean;
	/**
	 * The server provides find references support.
	 */
	referencesProvider?: boolean;
	/**
	 * The server provides document highlight support.
	 */
	documentHighlightProvider?: boolean;
	/**
	 * The server provides document symbol support.
	 */
	documentSymbolProvider?: boolean;
	/**
	 * The server provides workspace symbol support.
	 */
	workspaceSymbolProvider?: boolean;
	/**
	 * The server provides code actions.
	 */
	codeActionProvider?: boolean;
	/**
	 * The server provides code lens.
	 */
	codeLensProvider?: CodeLensOptions;
	/**
	 * The server provides document formatting.
	 */
	documentFormattingProvider?: boolean;
	/**
	 * The server provides document range formatting.
	 */
	documentRangeFormattingProvider?: boolean;
	/**
	 * The server provides document formatting on typing.
	 */
	documentOnTypeFormattingProvider?: {
		/**
		 * A character on which formatting should be triggered, like `}`.
		 */
		firstTriggerCharacter: string;
		/**
		 * More trigger characters.
		 */
		moreTriggerCharacter?: string[]
	};
	/**
	 * The server provides rename support.
	 */
	renameProvider?: boolean;
	/**
	 * The server provides document link support.
	 */
	documentLinkProvider?: DocumentLinkOptions;
	/**
	 * The server provides execute command support.
	 */
	executeCommandProvider?: ExecuteCommandOptions;
	/**
	 * Experimental server capabilities.
	 */
	experimental?: any;
}

/**
 * The initialize request is sent from the client to the server.
 * It is sent once as the request after starting up the server.
 * The requests parameter is of type [InitializeParams](#InitializeParams)
 * the response if of type [InitializeResult](#InitializeResult) of a Thenable that
 * resolves to such.
 */
export namespace InitializeRequest {
	export const type = new RequestType<InitializeParams, InitializeResult, InitializeError, void>('initialize');
}

/**
 * The initialize parameters
 */
export interface InitializeParams {
	/**
	 * The process Id of the parent process that started
	 * the server.
	 */
	processId: number;

	/**
	 * The rootPath of the workspace. Is null
	 * if no folder is open.
	 *
	 * @deprecated in favour of rootUri.
	 */
	rootPath?: string | null;

	/**
	 * The rootUri of the workspace. Is null if no
	 * folder is open. If both `rootPath` and `rootUri` are set
	 * `rootUri` wins.
	 */
	rootUri: string | null;

	/**
	 * The capabilities provided by the client (editor or tool)
	 */
	capabilities: ClientCapabilities;

	/**
	 * User provided initialization options.
	 */
	initializationOptions?: any;

	/**
	 * The initial trace setting. If omitted trace is disabled ('off').
	 */
	trace?: 'off' | 'messages' | 'verbose';
}

/**
 * The result returned from an initilize request.
 */
export interface InitializeResult {
	/**
	 * The capabilities the language server provides.
	 */
	capabilities: ServerCapabilities;
}

/**
 * Known error codes for an `InitializeError`;
 */
export namespace InitializeError {
	/**
	 * If the protocol version provided by the client can't be handled by the server.
	 * @deprecated This initialize error got replaced by client capabilities. There is
	 * no version handshake in version 3.0x
	 */
	export const unknownProtocolVersion: number = 1;
}

/**
 * The data type of the ResponseError if the
 * initialize request fails.
 */
export interface InitializeError {
	/**
	 * Indicates whether the client should retry to send the
	 * initialize request after showing the message provided
	 * in the {@link ResponseError}
	 */
	retry: boolean;
}

export interface InitializedParams {
}

/**
 * The intialized notification is send from the client to the
 * server after the client is fully initialized and the server
 * is allowed to send requests from the server to the client.
 */
export namespace InitializedNotification {
	export const type = new NotificationType<InitializedParams, void>('initialized');
}

//---- Shutdown Method ----

/**
 * A shutdown request is sent from the client to the server.
 * It is sent once when the client descides to shutdown the
 * server. The only notification that is sent after a shudown request
 * is the exit event.
 */
export namespace ShutdownRequest {
	export const type = new RequestType0<void, void, void>('shutdown');
}

//---- Exit Notification ----

/**
 * The exit event is sent from the client to the server to
 * ask the server to exit its process.
 */
export namespace ExitNotification {
	export const type = new NotificationType0<void>('exit');
}

//---- Configuration notification ----

/**
 * The configuration change notification is sent from the client to the server
 * when the client's configuration has changed. The notification contains
 * the changed configuration as defined by the language client.
 */
export namespace DidChangeConfigurationNotification {
	export const type = new NotificationType<DidChangeConfigurationParams, void>('workspace/didChangeConfiguration');
}

/**
 * The parameters of a change configuration notification.
 */
export interface DidChangeConfigurationParams {
	/**
	 * The actual changed settings
	 */
	settings: any;
}

//---- Message show and log notifications ----

/**
 * The message type
 */
export namespace MessageType {
	/**
	 * An error message.
	 */
	export const Error = 1;
	/**
	 * A warning message.
	 */
	export const Warning = 2;
	/**
	 * An information message.
	 */
	export const Info = 3;
	/**
	 * A log message.
	 */
	export const Log = 4;
}

export type MessageType = 1 | 2 | 3 | 4;

/**
 * The parameters of a notification message.
 */
export interface ShowMessageParams {
	/**
	 * The message type. See {@link MessageType}
	 */
	type: MessageType;

	/**
	 * The actual message
	 */
	message: string;
}

/**
 * The show message notification is sent from a server to a client to ask
 * the client to display a particular message in the user interface.
 */
export namespace ShowMessageNotification {
	export const type = new NotificationType<ShowMessageParams, void>('window/showMessage');
}

export interface MessageActionItem {
	/**
	 * A short title like 'Retry', 'Open Log' etc.
	 */
	title: string;
}

export interface ShowMessageRequestParams {
	/**
	 * The message type. See {@link MessageType}
	 */
	type: MessageType;

	/**
	 * The actual message
	 */
	message: string;

	/**
	 * The message action items to present.
	 */
	actions?: MessageActionItem[];
}

/**
 * The show message request is sent from the server to the clinet to show a message
 * and a set of options actions to the user.
 */
export namespace ShowMessageRequest {
	export const type = new RequestType<ShowMessageRequestParams, MessageActionItem, void, void>('window/showMessageRequest');
}

/**
 * The log message notification is sent from the server to the client to ask
 * the client to log a particular message.
 */
export namespace LogMessageNotification {
	export const type = new NotificationType<LogMessageParams, void>('window/logMessage');
}

/**
 * The log message parameters.
 */
export interface LogMessageParams {
	/**
	 * The message type. See {@link MessageType}
	 */
	type: MessageType;

	/**
	 * The actual message
	 */
	message: string;
}

//---- Telemetry notification

/**
 * The telemetry event notification is sent from the server to the client to ask
 * the client to log telemetry data.
 */
export namespace TelemetryEventNotification {
	export const type = new NotificationType<any, void>('telemetry/event');
}

//---- Text document notifications ----

/**
 * The parameters send in a open text document notification
 */
export interface DidOpenTextDocumentParams {
	/**
	 * The document that was opened.
	 */
	textDocument: TextDocumentItem;
}

/**
 * The document open notification is sent from the client to the server to signal
 * newly opened text documents. The document's truth is now managed by the client
 * and the server must not try to read the document's truth using the document's
 * uri.
 */
export namespace DidOpenTextDocumentNotification {
	export const type = new NotificationType<DidOpenTextDocumentParams, TextDocumentRegistrationOptions>('textDocument/didOpen');
}

/**
 * The change text document notification's parameters.
 */
export interface DidChangeTextDocumentParams {
	/**
	 * The document that did change. The version number points
	 * to the version after all provided content changes have
	 * been applied.
	 */
	textDocument: VersionedTextDocumentIdentifier;

	/**
	 * The actual content changes.
	 */
	contentChanges: TextDocumentContentChangeEvent[];
}

/**
 * Descibe options to be used when registered for text document change events.
 */
export interface TextDocumentChangeRegistrationOptions extends TextDocumentRegistrationOptions {
	/**
	 * How documents are synced to the server.
	 */
	syncKind: TextDocumentSyncKind;
}

/**
 * The document change notification is sent from the client to the server to signal
 * changes to a text document.
 */
export namespace DidChangeTextDocumentNotification {
	export const type = new NotificationType<DidChangeTextDocumentParams, TextDocumentChangeRegistrationOptions>('textDocument/didChange');
}

/**
 * The parameters send in a close text document notification
 */
export interface DidCloseTextDocumentParams {
	/**
	 * The document that was closed.
	 */
	textDocument: TextDocumentIdentifier;
}

/**
 * The document close notification is sent from the client to the server when
 * the document got closed in the client. The document's truth now exists
 * where the document's uri points to (e.g. if the document's uri is a file uri
 * the truth now exists on disk).
 */
export namespace DidCloseTextDocumentNotification {
	export const type = new NotificationType<DidCloseTextDocumentParams, TextDocumentRegistrationOptions>('textDocument/didClose');
}

/**
 * The parameters send in a save text document notification
 */
export interface DidSaveTextDocumentParams {
	/**
	 * The document that was closed.
	 */
	textDocument: VersionedTextDocumentIdentifier;

	/**
	 * Optional the content when saved. Depends on the includeText value
	 * when the save notifcation was requested.
	 */
	text?: string;
}

/**
 * Save registration options.
 */
export interface TextDocumentSaveRegistrationOptions extends TextDocumentRegistrationOptions, SaveOptions {
}

/**
 * The document save notification is sent from the client to the server when
 * the document got saved in the client.
 */
export namespace DidSaveTextDocumentNotification {
	export const type = new NotificationType<DidSaveTextDocumentParams, TextDocumentSaveRegistrationOptions>('textDocument/didSave');
}

/**
 * The parameters send in a will save text document notification.
 */
export interface WillSaveTextDocumentParams {
	/**
	 * The document that will be saved.
	 */
	textDocument: TextDocumentIdentifier;

	/**
	 * The 'TextDocumentSaveReason'.
	 */
	reason: TextDocumentSaveReason;
}

/**
 * A document will save notification is sent from the client to the server before
 * the document is actually saved.
 */
export namespace WillSaveTextDocumentNotification {
	export const type = new NotificationType<WillSaveTextDocumentParams, TextDocumentRegistrationOptions>('textDocument/willSave');
}

/**
 * A document will save request is sent from the client to the server before
 * the document is actually saved. The request can return an array of TextEdits
 * which will be applied to the text document before it is saved. Please note that
 * clients might drop results if computing the text edits took too long or if a
 * server constantly fails on this request. This is done to keep the save fast and
 * reliable.
 */
export namespace WillSaveTextDocumentWaitUntilRequest {
	export const type = new RequestType<WillSaveTextDocumentParams, TextEdit[], void, TextDocumentRegistrationOptions>('textDocument/willSaveWaitUntil');
}

//---- File eventing ----

/**
 * The watched files notification is sent from the client to the server when
 * the client detects changes to file watched by the lanaguage client.
 */
export namespace DidChangeWatchedFilesNotification {
	export const type = new NotificationType<DidChangeWatchedFilesParams, void>('workspace/didChangeWatchedFiles');
}

/**
 * The watched files change notification's parameters.
 */
export interface DidChangeWatchedFilesParams {
	/**
	 * The actual file events.
	 */
	changes: FileEvent[];
}

/**
 * The file event type
 */
export namespace FileChangeType {
	/**
	 * The file got created.
	 */
	export const Created = 1;
	/**
	 * The file got changed.
	 */
	export const Changed = 2;
	/**
	 * The file got deleted.
	 */
	export const Deleted = 3;
}

export type FileChangeType = 1 | 2 | 3;

/**
 * An event describing a file change.
 */
export interface FileEvent {
	/**
	 * The file's uri.
	 */
	uri: string;
	/**
	 * The change type.
	 */
	type: FileChangeType;
}

//---- Diagnostic notification ----

/**
 * Diagnostics notification are sent from the server to the client to signal
 * results of validation runs.
 */
export namespace PublishDiagnosticsNotification {
	export const type = new NotificationType<PublishDiagnosticsParams, void>('textDocument/publishDiagnostics');
}

/**
 * The publish diagnostic notification's parameters.
 */
export interface PublishDiagnosticsParams {
	/**
	 * The URI for which diagnostic information is reported.
	 */
	uri: string;

	/**
	 * An array of diagnostic information items.
	 */
	diagnostics: Diagnostic[];
}

//---- Completion Support --------------------------

/**
 * Completion registration options.
 */
export interface CompletionRegistrationOptions extends TextDocumentRegistrationOptions, CompletionOptions {
}

/**
 * Request to request completion at a given text document position. The request's
 * parameter is of type [TextDocumentPosition](#TextDocumentPosition) the response
 * is of type [CompletionItem[]](#CompletionItem) or [CompletionList](#CompletionList)
 * or a Thenable that resolves to such.
 */
export namespace CompletionRequest {
	export const type = new RequestType<TextDocumentPositionParams, CompletionItem[] | CompletionList, void, CompletionRegistrationOptions>('textDocument/completion');
}

/**
 * Request to resolve additional information for a given completion item.The request's
 * parameter is of type [CompletionItem](#CompletionItem) the response
 * is of type [CompletionItem](#CompletionItem) or a Thenable that resolves to such.
 */
export namespace CompletionResolveRequest {
	export const type = new RequestType<CompletionItem, CompletionItem, void, void>('completionItem/resolve');
}

//---- Hover Support -------------------------------

/**
 * Request to request hover information at a given text document position. The request's
 * parameter is of type [TextDocumentPosition](#TextDocumentPosition) the response is of
 * type [Hover](#Hover) or a Thenable that resolves to such.
 */
export namespace HoverRequest {
	export const type = new RequestType<TextDocumentPositionParams, Hover, void, TextDocumentRegistrationOptions>('textDocument/hover');
}

//---- SignatureHelp ----------------------------------

/**
 * Signature help registration options.
 */
export interface SignatureHelpRegistrationOptions extends TextDocumentRegistrationOptions, SignatureHelpOptions {
}

export namespace SignatureHelpRequest {
	export const type = new RequestType<TextDocumentPositionParams, SignatureHelp, void, SignatureHelpRegistrationOptions>('textDocument/signatureHelp');
}

//---- Goto Definition -------------------------------------

/**
 * A request to resolve the defintion location of a symbol at a given text
 * document position. The request's parameter is of type [TextDocumentPosition]
 * (#TextDocumentPosition) the response is of type [Definition](#Definition) or a
 * Thenable that resolves to such.
 */
export namespace DefinitionRequest {
	export const type = new RequestType<TextDocumentPositionParams, Definition, void, TextDocumentRegistrationOptions>('textDocument/definition');
}

//---- Reference Provider ----------------------------------

/**
 * Parameters for a [ReferencesRequest](#ReferencesRequest).
 */
export interface ReferenceParams extends TextDocumentPositionParams {
	context: ReferenceContext
}

/**
 * A request to resolve project-wide references for the symbol denoted
 * by the given text document position. The request's parameter is of
 * type [ReferenceParams](#ReferenceParams) the response is of type
 * [Location[]](#Location) or a Thenable that resolves to such.
 */
export namespace ReferencesRequest {
	export const type = new RequestType<ReferenceParams, Location[], void, TextDocumentRegistrationOptions>('textDocument/references');
}

//---- Document Highlight ----------------------------------

/**
 * Request to resolve a [DocumentHighlight](#DocumentHighlight) for a given
 * text document position. The request's parameter is of type [TextDocumentPosition]
 * (#TextDocumentPosition) the request reponse is of type [DocumentHighlight[]]
 * (#DocumentHighlight) or a Thenable that resolves to such.
 */
export namespace DocumentHighlightRequest {
	export const type = new RequestType<TextDocumentPositionParams, DocumentHighlight[], void, TextDocumentRegistrationOptions>('textDocument/documentHighlight');
}

//---- Document Symbol Provider ---------------------------

/**
 * A request to list all symbols found in a given text document. The request's
 * parameter is of type [TextDocumentIdentifier](#TextDocumentIdentifier) the
 * response is of type [SymbolInformation[]](#SymbolInformation) or a Thenable
 * that resolves to such.
 */
export namespace DocumentSymbolRequest {
	export const type = new RequestType<DocumentSymbolParams, SymbolInformation[], void, TextDocumentRegistrationOptions>('textDocument/documentSymbol');
}

//---- Workspace Symbol Provider ---------------------------

/**
 * A request to list project-wide symbols matching the query string given
 * by the [WorkspaceSymbolParams](#WorkspaceSymbolParams). The response is
 * of type [SymbolInformation[]](#SymbolInformation) or a Thenable that
 * resolves to such.
 */
export namespace WorkspaceSymbolRequest {
	export const type = new RequestType<WorkspaceSymbolParams, SymbolInformation[], void, void>('workspace/symbol');
}

//---- Code Action Provider ----------------------------------



/**
 * Params for the CodeActionRequest
 */
export interface CodeActionParams {
	/**
	 * The document in which the command was invoked.
	 */
	textDocument: TextDocumentIdentifier;

	/**
	 * The range for which the command was invoked.
	 */
	range: Range;

	/**
	 * Context carrying additional information.
	 */
	context: CodeActionContext;
}

/**
 * A request to provide commands for the given text document and range.
 */
export namespace CodeActionRequest {
	export const type = new RequestType<CodeActionParams, Command[], void, TextDocumentRegistrationOptions>('textDocument/codeAction');
}

//---- Code Lens Provider -------------------------------------------

/**
 * Params for the Code Lens request.
 */
export interface CodeLensParams {
	/**
	 * The document to request code lens for.
	 */
	textDocument: TextDocumentIdentifier;
}

/**
 * Code Lens registration options.
 */
export interface CodeLensRegistrationOptions extends TextDocumentRegistrationOptions, CodeLensOptions {
}

/**
 * A request to provide code lens for the given text document.
 */
export namespace CodeLensRequest {
	export const type = new RequestType<CodeLensParams, CodeLens[], void, CodeLensRegistrationOptions>('textDocument/codeLens');
}

/**
 * A request to resolve a command for a given code lens.
 */
export namespace CodeLensResolveRequest {
	export const type = new RequestType<CodeLens, CodeLens, void, void>('codeLens/resolve');
}

//---- Formatting ----------------------------------------------

export interface DocumentFormattingParams {
	/**
	 * The document to format.
	 */
	textDocument: TextDocumentIdentifier;

	/**
	 * The format options
	 */
	options: FormattingOptions;
}

/**
 * A request to to format a whole document.
 */
export namespace DocumentFormattingRequest {
	export const type = new RequestType<DocumentFormattingParams, TextEdit[], void, TextDocumentRegistrationOptions>('textDocument/formatting');
}

export interface DocumentRangeFormattingParams {
	/**
	 * The document to format.
	 */
	textDocument: TextDocumentIdentifier;

	/**
	 * The range to format
	 */
	range: Range;

	/**
	 * The format options
	 */
	options: FormattingOptions;
}

/**
 * A request to to format a range in a document.
 */
export namespace DocumentRangeFormattingRequest {
	export const type = new RequestType<DocumentRangeFormattingParams, TextEdit[], void, TextDocumentRegistrationOptions>('textDocument/rangeFormatting');
}

export interface DocumentOnTypeFormattingParams {
	/**
	 * The document to format.
	 */
	textDocument: TextDocumentIdentifier;

	/**
	 * The position at which this request was send.
	 */
	position: Position;

	/**
	 * The character that has been typed.
	 */
	ch: string;

	/**
	 * The format options.
	 */
	options: FormattingOptions;
}

/**
 * Format document on type options
 */
export interface DocumentOnTypeFormattingRegistrationOptions extends TextDocumentRegistrationOptions, DocumentOnTypeFormattingOptions {
}

/**
 * A request to format a document on type.
 */
export namespace DocumentOnTypeFormattingRequest {
	export const type = new RequestType<DocumentOnTypeFormattingParams, TextEdit[], void, DocumentOnTypeFormattingRegistrationOptions>('textDocument/onTypeFormatting');
}

//---- Rename ----------------------------------------------

export interface RenameParams {
	/**
	 * The document to format.
	 */
	textDocument: TextDocumentIdentifier;

	/**
	 * The position at which this request was send.
	 */
	position: Position;

	/**
	 * The new name of the symbol. If the given name is not valid the
	 * request must return a [ResponseError](#ResponseError) with an
	 * appropriate message set.
	 */
	newName: string;
}

/**
 * A request to rename a symbol.
 */
export namespace RenameRequest {
	export const type = new RequestType<RenameParams, WorkspaceEdit, void, TextDocumentRegistrationOptions>('textDocument/rename');
}

//---- Document Links ----------------------------------------------

export interface DocumentLinkParams {
	/**
	 * The document to provide document links for.
	 */
	textDocument: TextDocumentIdentifier;
}

/**
 * Document link registration options
 */
export interface DocumentLinkRegistrationOptions extends TextDocumentRegistrationOptions, DocumentLinkOptions {
}

/**
 * A request to provide document links
 */
export namespace DocumentLinkRequest {
	export const type = new RequestType<DocumentLinkParams, DocumentLink[], void, DocumentLinkRegistrationOptions>('textDocument/documentLink');
}

/**
 * Request to resolve additional information for a given document link. The request's
 * parameter is of type [DocumentLink](#DocumentLink) the response
 * is of type [DocumentLink](#DocumentLink) or a Thenable that resolves to such.
 */
export namespace DocumentLinkResolveRequest {
	export const type = new RequestType<DocumentLink, DocumentLink, void, void>('documentLink/resolve');
}

//---- Command Execution -------------------------------------------

export interface ExecuteCommandParams {

	/**
	 * The identifier of the actual command handler.
	 */
	command: string;
	/**
	 * Arguments that the command should be invoked with.
	 */
	arguments?: any[];
}

/**
 * Execute command registration options.
 */
export interface ExecuteCommandRegistrationOptions extends ExecuteCommandOptions {
}

/**
 * A request send from the client to the server to execute a command. The request might return
 * a workspace edit which the client will apply to the workspace.
 */
export namespace ExecuteCommandRequest {
	export const type = new RequestType<ExecuteCommandParams, any, void, ExecuteCommandRegistrationOptions>('workspace/executeCommand');
}

//---- Apply Edit request ----------------------------------------

/**
 * The parameters passed via a apply workspace edit request.
 */
export interface ApplyWorkspaceEditParams {
	/**
	 * The edits to apply.
	 */
	edit: WorkspaceEdit;
}

/**
 * A reponse returned from the apply workspace edit request.
 */
export interface ApplyWorkspaceEditResponse {
	/**
	 * Indicates whether the edit was applied or not.
	 */
	applied: boolean;
}

/**
 * A request sent from the server to the client to modified certain resources.
 */
export namespace ApplyWorkspaceEditRequest {
	export const type = new RequestType<ApplyWorkspaceEditParams, ApplyWorkspaceEditResponse, void, void>('workspace/applyEdit');
}