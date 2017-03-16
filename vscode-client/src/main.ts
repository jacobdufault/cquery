/* --------------------------------------------------------------------------------------------
 * Copyright (c) Microsoft Corporation. All rights reserved.
 * Licensed under the MIT License. See License.txt in the project root for license information.
 * ------------------------------------------------------------------------------------------ */
'use strict';

import * as cp from 'child_process';
import ChildProcess = cp.ChildProcess;

import {
	workspace as Workspace, window as Window, languages as Languages, commands as Commands,
	TextDocumentChangeEvent, TextDocument, Disposable, OutputChannel,
	FileSystemWatcher, DiagnosticCollection, Uri,
	CancellationToken, Position as VPosition, Location as VLocation, Range as VRange,
	CompletionItem as VCompletionItem, CompletionList as VCompletionList, SignatureHelp as VSignatureHelp, Definition as VDefinition, DocumentHighlight as VDocumentHighlight,
	SymbolInformation as VSymbolInformation, CodeActionContext as VCodeActionContext, Command as VCommand, CodeLens as VCodeLens,
	FormattingOptions as VFormattingOptions, TextEdit as VTextEdit, WorkspaceEdit as VWorkspaceEdit, MessageItem,
	Hover as VHover,
	DocumentLink as VDocumentLink, TextDocumentWillSaveEvent
} from 'vscode';

import {
	Message, MessageType as RPCMessageType, Logger, createMessageConnection, ErrorCodes, ResponseError,
	RequestType, RequestType0, RequestHandler, RequestHandler0, GenericRequestHandler,
	NotificationType, NotificationType0,
	NotificationHandler, NotificationHandler0, GenericNotificationHandler,
	MessageReader, IPCMessageReader, MessageWriter, IPCMessageWriter, Trace, Tracer, Event, Emitter,
	createClientPipeTransport, generateRandomPipeName
} from 'vscode-jsonrpc';

import {
	WorkspaceEdit
} from 'vscode-languageserver-types';


import {
	ClientCapabilities,
	RegistrationRequest, RegistrationParams, UnregistrationRequest, UnregistrationParams, TextDocumentRegistrationOptions,
	InitializeRequest, InitializeParams, InitializeResult, InitializeError, ServerCapabilities, TextDocumentSyncKind, TextDocumentSyncOptions,
	InitializedNotification, ShutdownRequest, ExitNotification,
	LogMessageNotification, LogMessageParams, MessageType,
	ShowMessageNotification, ShowMessageParams, ShowMessageRequest,
	TelemetryEventNotification,
	DidChangeConfigurationNotification, DidChangeConfigurationParams,
	DocumentSelector,
	DidOpenTextDocumentNotification, DidOpenTextDocumentParams,
	DidChangeTextDocumentNotification, DidChangeTextDocumentParams, TextDocumentChangeRegistrationOptions,
	DidCloseTextDocumentNotification, DidCloseTextDocumentParams,
	DidSaveTextDocumentNotification, DidSaveTextDocumentParams, TextDocumentSaveRegistrationOptions,
	WillSaveTextDocumentNotification, WillSaveTextDocumentWaitUntilRequest, WillSaveTextDocumentParams,
	DidChangeWatchedFilesNotification, DidChangeWatchedFilesParams, FileEvent, FileChangeType,
	PublishDiagnosticsNotification, PublishDiagnosticsParams,
	CompletionRequest, CompletionResolveRequest, CompletionRegistrationOptions,
	HoverRequest,
	SignatureHelpRequest, SignatureHelpRegistrationOptions, DefinitionRequest, ReferencesRequest, DocumentHighlightRequest,
	DocumentSymbolRequest, WorkspaceSymbolRequest,
	CodeActionRequest, CodeActionParams,
	CodeLensRequest, CodeLensResolveRequest, CodeLensRegistrationOptions,
	DocumentFormattingRequest, DocumentFormattingParams, DocumentRangeFormattingRequest, DocumentRangeFormattingParams,
	DocumentOnTypeFormattingRequest, DocumentOnTypeFormattingParams, DocumentOnTypeFormattingRegistrationOptions,
	RenameRequest, RenameParams,
	DocumentLinkRequest, DocumentLinkResolveRequest, DocumentLinkRegistrationOptions,
	ExecuteCommandRequest, ExecuteCommandParams, ExecuteCommandRegistrationOptions,
	ApplyWorkspaceEditRequest, ApplyWorkspaceEditParams, ApplyWorkspaceEditResponse
} from './protocol';

import * as c2p from './codeConverter';
import * as p2c from './protocolConverter';

import * as is from './utils/is';
import * as electron from './utils/electron';
import { terminate } from './utils/processes';
import { Delayer } from './utils/async'
import * as UUID from './utils/uuid';

export {
	ResponseError, InitializeError, ErrorCodes,
	RequestType, RequestType0, RequestHandler, RequestHandler0, GenericRequestHandler,
	NotificationType, NotificationType0, NotificationHandler, NotificationHandler0, GenericNotificationHandler
}
export { Converter as Code2ProtocolConverter } from './codeConverter';
export { Converter as Protocol2CodeConverter } from './protocolConverter';

export * from 'vscode-languageserver-types';
export * from './protocol';

declare var v8debug: any;

interface IConnection {

	listen(): void;

	sendRequest<R, E, RO>(type: RequestType0<R, E, RO>, token?: CancellationToken): Thenable<R>;
	sendRequest<P, R, E, RO>(type: RequestType<P, R, E, RO>, params: P, token?: CancellationToken): Thenable<R>;
	sendRequest<R>(method: string, token?: CancellationToken): Thenable<R>;
	sendRequest<R>(method: string, param: any, token?: CancellationToken): Thenable<R>;
	sendRequest<R>(type: string | RPCMessageType, ...params: any[]): Thenable<R>;

	onRequest<R, E, RO>(type: RequestType0<R, E, RO>, handler: RequestHandler0<R, E>): void;
	onRequest<P, R, E, RO>(type: RequestType<P, R, E, RO>, handler: RequestHandler<P, R, E>): void;
	onRequest<R, E>(method: string, handler: GenericRequestHandler<R, E>): void;
	onRequest<R, E>(method: string | RPCMessageType, handler: GenericRequestHandler<R, E>): void;

	sendNotification<RO>(type: NotificationType0<RO>): void;
	sendNotification<P, RO>(type: NotificationType<P, RO>, params?: P): void;
	sendNotification(method: string): void;
	sendNotification(method: string, params: any): void;
	sendNotification(method: string | RPCMessageType, params?: any): void;

	onNotification<RO>(type: NotificationType0<RO>, handler: NotificationHandler0): void;
	onNotification<P, RO>(type: NotificationType<P, RO>, handler: NotificationHandler<P>): void;
	onNotification(method: string, handler: GenericNotificationHandler): void;
	onNotification(method: string | RPCMessageType, handler: GenericNotificationHandler): void;

	trace(value: Trace, tracer: Tracer, sendNotification?: boolean): void;

	initialize(params: InitializeParams): Thenable<InitializeResult>;
	shutdown(): Thenable<void>;
	exit(): void;

	onLogMessage(handle: NotificationHandler<LogMessageParams>): void;
	onShowMessage(handler: NotificationHandler<ShowMessageParams>): void;
	onTelemetry(handler: NotificationHandler<any>): void;

	didChangeConfiguration(params: DidChangeConfigurationParams): void;
	didChangeWatchedFiles(params: DidChangeWatchedFilesParams): void;

	didOpenTextDocument(params: DidOpenTextDocumentParams): void;
	didChangeTextDocument(params: DidChangeTextDocumentParams): void;
	didCloseTextDocument(params: DidCloseTextDocumentParams): void;
	didSaveTextDocument(params: DidSaveTextDocumentParams): void;
	onDiagnostics(handler: NotificationHandler<PublishDiagnosticsParams>): void;

	dispose(): void;
}

class ConsoleLogger implements Logger {
	public error(message: string): void {
		console.error(message);
	}
	public warn(message: string): void {
		console.warn(message);
	}
	public info(message: string): void {
		console.info(message);
	}
	public log(message: string): void {
		console.log(message);
	}
}

interface ConnectionErrorHandler {
	(error: Error, message: Message, count: number): void;
}

interface ConnectionCloseHandler {
	(): void;
}
function createConnection(inputStream: NodeJS.ReadableStream, outputStream: NodeJS.WritableStream, errorHandler: ConnectionErrorHandler, closeHandler: ConnectionCloseHandler): IConnection;
function createConnection(reader: MessageReader, writer: MessageWriter, errorHandler: ConnectionErrorHandler, closeHandler: ConnectionCloseHandler): IConnection;
function createConnection(input: any, output: any, errorHandler: ConnectionErrorHandler, closeHandler: ConnectionCloseHandler): IConnection {
	let logger = new ConsoleLogger();
	let connection = createMessageConnection(input, output, logger);
	connection.onError((data) => { errorHandler(data[0], data[1], data[2]) });
	connection.onClose(closeHandler);
	let result: IConnection = {

		listen: (): void => connection.listen(),

		sendRequest: <R>(type: string | RPCMessageType, ...params: any[]): Thenable<R> => connection.sendRequest(is.string(type) ? type : type.method, ...params),
		onRequest: <R, E>(type: string | RPCMessageType, handler: GenericRequestHandler<R, E>): void => connection.onRequest(is.string(type) ? type : type.method, handler),

		sendNotification: (type: string | RPCMessageType, params?: any): void => connection.sendNotification(is.string(type) ? type : type.method, params),
		onNotification: (type: string | RPCMessageType, handler: GenericNotificationHandler): void => connection.onNotification(is.string(type) ? type : type.method, handler),

		trace: (value: Trace, tracer: Tracer, sendNotification: boolean = false): void => connection.trace(value, tracer, sendNotification),

		initialize: (params: InitializeParams) => connection.sendRequest(InitializeRequest.type, params),
		shutdown: () => connection.sendRequest(ShutdownRequest.type, undefined),
		exit: () => connection.sendNotification(ExitNotification.type),

		onLogMessage: (handler: NotificationHandler<LogMessageParams>) => connection.onNotification(LogMessageNotification.type, handler),
		onShowMessage: (handler: NotificationHandler<ShowMessageParams>) => connection.onNotification(ShowMessageNotification.type, handler),
		onTelemetry: (handler: NotificationHandler<any>) => connection.onNotification(TelemetryEventNotification.type, handler),

		didChangeConfiguration: (params: DidChangeConfigurationParams) => connection.sendNotification(DidChangeConfigurationNotification.type, params),
		didChangeWatchedFiles: (params: DidChangeWatchedFilesParams) => connection.sendNotification(DidChangeWatchedFilesNotification.type, params),

		didOpenTextDocument: (params: DidOpenTextDocumentParams) => connection.sendNotification(DidOpenTextDocumentNotification.type, params),
		didChangeTextDocument: (params: DidChangeTextDocumentParams) => connection.sendNotification(DidChangeTextDocumentNotification.type, params),
		didCloseTextDocument: (params: DidCloseTextDocumentParams) => connection.sendNotification(DidCloseTextDocumentNotification.type, params),
		didSaveTextDocument: (params: DidSaveTextDocumentParams) => connection.sendNotification(DidSaveTextDocumentNotification.type, params),

		onDiagnostics: (handler: NotificationHandler<PublishDiagnosticsParams>) => connection.onNotification(PublishDiagnosticsNotification.type, handler),

		dispose: () => connection.dispose()
	}

	return result;
}

export interface StreamInfo {
	writer: NodeJS.WritableStream;
	reader: NodeJS.ReadableStream;
}

export interface ExecutableOptions {
	cwd?: string;
	stdio?: string | string[];
	env?: any;
	detached?: boolean;
}

export interface Executable {
	command: string;
	args?: string[];
	options?: ExecutableOptions;
}

export interface ForkOptions {
	cwd?: string;
	env?: any;
	encoding?: string;
	execArgv?: string[];
}

export enum TransportKind {
	stdio,
	ipc,
	pipe
}

export interface NodeModule {
	module: string;
	transport?: TransportKind;
	args?: string[];
	runtime?: string;
	options?: ForkOptions;
}

export type ServerOptions = Executable | { run: Executable; debug: Executable; } | { run: NodeModule; debug: NodeModule } | NodeModule | (() => Thenable<ChildProcess | StreamInfo>);

/**
 * An action to be performed when the connection is producing errors.
 */
export enum ErrorAction {
	/**
	 * Continue running the server.
	 */
	Continue = 1,
	/**
	 * Shutdown the server.
	 */
	Shutdown = 2
}

/**
 * An action to be performed when the connection to a server got closed.
 */
export enum CloseAction {
	/**
	 * Don't restart the server. The connection stays closed.
	 */
	DoNotRestart = 1,
	/**
	 * Restart the server.
	 */
	Restart = 2,
}


/**
 * A pluggable error handler that is invoked when the connection is either
 * producing errors or got closed.
 */
export interface ErrorHandler {
	/**
	 * An error has occurred while writing or reading from the connection.
	 *
	 * @param error - the error received
	 * @param message - the message to be delivered to the server if know.
	 * @param count - a count indicating how often an error is received. Will
	 *  be reset if a message got successfully send or received.
	 */
	error(error: Error, message: Message, count: number): ErrorAction;

	/**
	 * The connection to the server got closed.
	 */
	closed(): CloseAction
}

class DefaultErrorHandler implements ErrorHandler {

	private restarts: number[];

	constructor(private name: string) {
		this.restarts = [];
	}

	public error(_error: Error, _message: Message, count: number): ErrorAction {
		if (count && count <= 3) {
			return ErrorAction.Continue;
		}
		return ErrorAction.Shutdown;
	}
	public closed(): CloseAction {
		this.restarts.push(Date.now());
		if (this.restarts.length < 5) {
			return CloseAction.Restart;
		} else {
			let diff = this.restarts[this.restarts.length - 1] - this.restarts[0];
			if (diff <= 3 * 60 * 1000) {
				Window.showErrorMessage(`The ${this.name} server crashed 5 times in the last 3 minutes. The server will not be restarted.`);
				return CloseAction.DoNotRestart;
			} else {
				this.restarts.shift();
				return CloseAction.Restart;
			}
		}
	}
}

export interface InitializationFailedHandler {
	(error: ResponseError<InitializeError> | Error | any): boolean;
}

export interface SynchronizeOptions {
	configurationSection?: string | string[];
	fileEvents?: FileSystemWatcher | FileSystemWatcher[];
}

export enum RevealOutputChannelOn {
	Info = 1,
	Warn = 2,
	Error = 3,
	Never = 4
}

export interface LanguageClientOptions {
	documentSelector?: DocumentSelector | string[];
	synchronize?: SynchronizeOptions;
	diagnosticCollectionName?: string;
	outputChannelName?: string;
	revealOutputChannelOn?: RevealOutputChannelOn;
	/**
	 * The encoding use to read stdout and stderr. Defaults
	 * to 'utf8' if ommitted.
	 */
	stdioEncoding?: string;
	initializationOptions?: any | (() => any);
	initializationFailedHandler?: InitializationFailedHandler;
	errorHandler?: ErrorHandler;
	uriConverters?: {
		code2Protocol: c2p.URIConverter,
		protocol2Code: p2c.URIConverter
	};
}

interface ResolvedClientOptions {
	documentSelector?: DocumentSelector;
	synchronize: SynchronizeOptions;
	diagnosticCollectionName?: string;
	outputChannelName: string;
	revealOutputChannelOn: RevealOutputChannelOn;
	stdioEncoding: string;
	initializationOptions?: any | (() => any);
	initializationFailedHandler?: InitializationFailedHandler;
	errorHandler: ErrorHandler;
	uriConverters?: {
		code2Protocol: c2p.URIConverter,
		protocol2Code: p2c.URIConverter
	};
}

export enum State {
	Stopped = 1,
	Running = 2
}

export interface StateChangeEvent {
	oldState: State;
	newState: State;
}

enum ClientState {
	Initial,
	Starting,
	StartFailed,
	Running,
	Stopping,
	Stopped
}

interface RegistrationData<T> {
	id: string;
	registerOptions: T;
}

interface FeatureHandler<T> {
	register(data: RegistrationData<T>): void;
	unregister(id: string): void;
	dispose(): void;
}

interface CreateParamsSignature<E, P> {
	(data: E): P;
}

class DocumentNotifiactions<P, E> implements FeatureHandler<TextDocumentRegistrationOptions> {

	private _listener: Disposable | undefined;
	protected _selectors: Map<string, DocumentSelector> = new Map<string, DocumentSelector>();

	public static textDocumentFilter(selectors: IterableIterator<DocumentSelector>, textDocument: TextDocument): boolean {
		for (const selector of selectors) {
			if (Languages.match(selector, textDocument)) {
				return true;
			}
		}
		return false;
	}

	constructor(
		protected _client: LanguageClient, private _event: Event<E>,
		protected _type: NotificationType<P, DocumentSelector>, protected _createParams: CreateParamsSignature<E, P>,
		protected _selectorFilter?: (selectors: IterableIterator<DocumentSelector>, data: E) => boolean) {
	}

	public register(data: RegistrationData<TextDocumentRegistrationOptions>): void {
		if (!data.registerOptions.documentSelector) {
			return;
		}
		if (!this._listener) {
			this._listener = this._event(this.callback, this);
		}
		this._selectors.set(data.id, data.registerOptions.documentSelector);
	}

	private callback(data: E): void {
		if (!this._selectorFilter || this._selectorFilter(this._selectors.values(), data)) {
			this._client.sendNotification(this._type, this._createParams(data));
			this.notificationSent(data);
		}
	}

	protected notificationSent(_data: E): void {
	}

	public unregister(id: string): void {
		this._selectors.delete(id);
		if (this._selectors.size === 0 && this._listener) {
			this._listener.dispose();
			this._listener = undefined;
		}
	}

	public dispose(): void {
		if (this._listener) {
			this._listener.dispose();
		}
	}
}

class DidOpenTextDocumentFeature extends DocumentNotifiactions<DidOpenTextDocumentParams, TextDocument> {
	constructor(client: LanguageClient, private _syncedDocuments: Map<string, TextDocument>) {
		super(
			client, Workspace.onDidOpenTextDocument, DidOpenTextDocumentNotification.type,
			(textDocument) => client.code2ProtocolConverter.asOpenTextDocumentParams(textDocument),
			DocumentNotifiactions.textDocumentFilter
		);
	}

	public register(data: RegistrationData<TextDocumentRegistrationOptions>): void {
		super.register(data);
		if (!data.registerOptions.documentSelector) {
			return;
		}
		let documentSelector = data.registerOptions.documentSelector;
		Workspace.textDocuments.forEach((textDocument) => {
			let uri: string = textDocument.uri.toString();
			if (this._syncedDocuments.has(uri)) {
				return;
			}
			if (Languages.match(documentSelector, textDocument)) {
				this._client.sendNotification(this._type, this._createParams(textDocument));
				this._syncedDocuments.set(uri, textDocument);
			}
		});
	}

	protected notificationSent(textDocument: TextDocument): void {
		super.notificationSent(textDocument);
		this._syncedDocuments.set(textDocument.uri.toString(), textDocument);
	}
}

class DidCloseTextDocumentFeature extends DocumentNotifiactions<DidCloseTextDocumentParams, TextDocument> {

	constructor(client: LanguageClient, private _syncedDocuments: Map<string, TextDocument>) {
		super(
			client, Workspace.onDidCloseTextDocument, DidCloseTextDocumentNotification.type,
			(textDocument) => client.code2ProtocolConverter.asCloseTextDocumentParams(textDocument),
			DocumentNotifiactions.textDocumentFilter
		);
	}

	protected notificationSent(textDocument: TextDocument): void {
		super.notificationSent(textDocument);
		this._syncedDocuments.delete(textDocument.uri.toString());
	}

	public unregister(id: string): void {
		let selector = this._selectors.get(id)!;
		super.unregister(id);
		let selectors = this._selectors.values();
		this._syncedDocuments.forEach((textDocument) => {
			if (Languages.match(selector, textDocument) && !this._selectorFilter!(selectors, textDocument)) {
				this._client.sendNotification(this._type, this._createParams(textDocument));
				this._syncedDocuments.delete(textDocument.uri.toString());
			}
		});
	}
}

interface DidChangeTextDocumentData {
	documentSelector: DocumentSelector;
	syncKind: 0 | 1 | 2;
}

class DidChangeTextDocumentFeature implements FeatureHandler<TextDocumentChangeRegistrationOptions> {

	private _listener: Disposable | undefined;
	private _changeData: Map<string, DidChangeTextDocumentData> = new Map<string, DidChangeTextDocumentData>();
	private _forcingDelivery: boolean = false;
	private _changeDelayer: { uri: string; delayer: Delayer<void> } | undefined;

	constructor(private _client: LanguageClient) {
	}

	public register(data: RegistrationData<TextDocumentChangeRegistrationOptions>): void {
		if (!data.registerOptions.documentSelector) {
			return;
		}
		if (!this._listener) {
			this._listener = Workspace.onDidChangeTextDocument(this.callback, this);
		}
		this._changeData.set(
			data.id,
			{
				documentSelector: data.registerOptions.documentSelector,
				syncKind: data.registerOptions.syncKind
			}
		);
	}

	private callback(event: TextDocumentChangeEvent): void {
		for (const changeData of this._changeData.values()) {
			if (Languages.match(changeData.documentSelector, event.document)) {
				if (changeData.syncKind === TextDocumentSyncKind.Incremental) {
					this._client.sendNotification(DidChangeTextDocumentNotification.type, this._client.code2ProtocolConverter.asChangeTextDocumentParams(event));
					break;
				} else if (changeData.syncKind === TextDocumentSyncKind.Full) {
					if (this._changeDelayer) {
						if (this._changeDelayer.uri !== event.document.uri.toString()) {
							// Use this force delivery to track boolean state. Otherwise we might call two times.
							this.forceDelivery();
							this._changeDelayer.uri = event.document.uri.toString();
						}
						this._changeDelayer.delayer.trigger(() => {
							this._client.sendNotification(DidChangeTextDocumentNotification.type, this._client.code2ProtocolConverter.asChangeTextDocumentParams(event.document));
						});
					} else {
						this._changeDelayer = {
							uri: event.document.uri.toString(),
							delayer: new Delayer<void>(200)
						}
						this._changeDelayer.delayer.trigger(() => {
							this._client.sendNotification(DidChangeTextDocumentNotification.type, this._client.code2ProtocolConverter.asChangeTextDocumentParams(event.document));
						}, -1);
					}
					break;
				}
			}
		}
	}

	public unregister(id: string): void {
		this._changeData.delete(id);
		if (this._changeData.size === 0 && this._listener) {
			this._listener.dispose();
			this._listener = undefined;
		}
	}

	public dispose(): void {
		if (this._listener) {
			this._listener.dispose();
			this._listener = undefined;
		}
	}

	public forceDelivery() {
		if (this._forcingDelivery || !this._changeDelayer) {
			return;
		}
		try {
			this._forcingDelivery = true;
			this._changeDelayer.delayer.forceDelivery();
		} finally {
			this._forcingDelivery = false;
		}
	}
}

class WillSaveWaitUntilFeature implements FeatureHandler<TextDocumentRegistrationOptions> {

	private _listener: Disposable | undefined;
	private _selectors: Map<string, DocumentSelector> = new Map<string, DocumentSelector>();

	constructor(private _client: LanguageClient) {
	}

	public register(data: RegistrationData<TextDocumentRegistrationOptions>): void {
		if (!data.registerOptions.documentSelector) {
			return;
		}
		if (!this._listener) {
			this._listener = Workspace.onWillSaveTextDocument(this.callback, this);
		}
		this._selectors.set(data.id, data.registerOptions.documentSelector);
	}

	private callback(event: TextDocumentWillSaveEvent): void {
		if (DocumentNotifiactions.textDocumentFilter(this._selectors.values(), event.document)) {
			event.waitUntil(
				this._client.sendRequest(
					WillSaveTextDocumentWaitUntilRequest.type,
					this._client.code2ProtocolConverter.asWillSaveTextDocumentParams(event)
				).then((edits) => {
					return this._client.protocol2CodeConverter.asTextEdits(edits);
				})
			);
		}
	}

	public unregister(id: string): void {
		this._selectors.delete(id);
		if (this._selectors.size === 0 && this._listener) {
			this._listener.dispose();
			this._listener = undefined;
		}
	}

	public dispose(): void {
		if (this._listener) {
			this._listener.dispose();
			this._listener = undefined;
		}
	}
}

class DidSaveTextDocumentFeature extends DocumentNotifiactions<DidSaveTextDocumentParams, TextDocument> {

	private _includeText: boolean;

	constructor(client: LanguageClient) {
		super(
			client, Workspace.onDidSaveTextDocument, DidSaveTextDocumentNotification.type,
			(textDocument) => client.code2ProtocolConverter.asSaveTextDocumentParams(textDocument, this._includeText),
			DocumentNotifiactions.textDocumentFilter
		);
	}

	public register(data: RegistrationData<TextDocumentSaveRegistrationOptions>): void {
		this._includeText = !!data.registerOptions.includeText;
		super.register(data);
	}
}

interface CreateProviderSignature<T extends TextDocumentRegistrationOptions> {
	(options: T): Disposable;
}

class LanguageFeature<T extends TextDocumentRegistrationOptions> implements FeatureHandler<T> {

	protected _providers: Map<string, Disposable> = new Map<string, Disposable>();

	constructor(private _createProvider: CreateProviderSignature<T>) {
	}

	public register(data: RegistrationData<T>): void {
		if (!data.registerOptions.documentSelector) {
			return;
		}
		let provider = this._createProvider(data.registerOptions);
		if (provider) {
			this._providers.set(data.id, provider);
		}
	}

	public unregister(id: string): void {
		let provider = this._providers.get(id);
		if (provider) {
			provider.dispose();
		}
	}

	public dispose(): void {
		this._providers.forEach((value) => {
			value.dispose();
		});
	}
}


class ExecuteCommandFeature implements FeatureHandler<ExecuteCommandRegistrationOptions> {

	private _commands: Map<string, Disposable[]> = new Map<string, Disposable[]>();

	constructor(private _client: LanguageClient, private _logger: (type: RPCMessageType, error?: any) => void) {
	}

	public register(data: RegistrationData<ExecuteCommandRegistrationOptions>): void {
		if (data.registerOptions.commands) {
			let disposeables: Disposable[] = [];
			for (const command of data.registerOptions.commands) {
				disposeables.push(Commands.registerCommand(command, (...args: any[]) => {
					let params: ExecuteCommandParams = {
						command,
						arguments: args
					};
					this._client.sendRequest(ExecuteCommandRequest.type, params).then(undefined, (error) => { this._logger(ExecuteCommandRequest.type, error); });
				}));
			}
			this._commands.set(data.id, disposeables);
		}
	}

	public unregister(id: string): void {
		let disposeables = this._commands.get(id);
		if (disposeables) {
			disposeables.forEach(disposable => disposable.dispose());
		}
	}

	public dispose(): void {
		this._commands.forEach((value) => {
			value.forEach(disposable => disposable.dispose());
		});
	}
}

interface FeatureHandlerMap extends Map<string, FeatureHandler<any>> {
	get(key: string): FeatureHandler<any>;
}

const clientCapabilities: ClientCapabilities = {
	workspace: {
		applyEdit: true,
		didChangeConfiguration: {
			dynamicRegistration: false
		},
		didChangeWatchedFiles: {
			dynamicRegistration: false
		},
		symbol: {
			dynamicRegistration: true
		},
		executeCommand: {
			dynamicRegistration: true
		}
	},
	textDocument: {
		synchronization: {
			dynamicRegistration: true,
			willSave: true,
			willSaveWaitUntil: true,
			didSave: true
		},
		completion: {
			dynamicRegistration: true,
			completionItem: {
				snippetSupport: true
			}
		},
		hover: {
			dynamicRegistration: true
		},
		signatureHelp: {
			dynamicRegistration: true
		},
		references: {
			dynamicRegistration: true
		},
		documentHighlight: {
			dynamicRegistration: true
		},
		documentSymbol: {
			dynamicRegistration: true
		},
		formatting: {
			dynamicRegistration: true
		},
		rangeFormatting: {
			dynamicRegistration: true
		},
		onTypeFormatting: {
			dynamicRegistration: true
		},
		definition: {
			dynamicRegistration: true
		},
		codeAction: {
			dynamicRegistration: true
		},
		codeLens: {
			dynamicRegistration: true
		},
		documentLink: {
			dynamicRegistration: true
		},
		rename: {
			dynamicRegistration: true
		}
	}
}

export class LanguageClient {

	private _id: string;
	private _name: string;
	private _serverOptions: ServerOptions;
	private _clientOptions: ResolvedClientOptions;
	private _forceDebug: boolean;

	private _state: ClientState;
	private _onReady: Promise<void>;
	private _onReadyCallbacks: { resolve: () => void; reject: (error: any) => void; };
	private _connectionPromise: Thenable<IConnection> | undefined;
	private _resolvedConnection: IConnection | undefined;
	private _childProcess: ChildProcess | undefined;
	private _outputChannel: OutputChannel | undefined;
	private _capabilites: ServerCapabilities;

	private _listeners: Disposable[] | undefined;
	private _providers: Disposable[] | undefined;
	private _diagnostics: DiagnosticCollection | undefined;

	private _fileEvents: FileEvent[];
	private _fileEventDelayer: Delayer<void>;

	private _telemetryEmitter: Emitter<any>;
	private _stateChangeEmitter: Emitter<StateChangeEvent>;

	private _trace: Trace;
	private _tracer: Tracer;

	private _c2p: c2p.Converter;
	private _p2c: p2c.Converter;

	public constructor(name: string, serverOptions: ServerOptions, clientOptions: LanguageClientOptions, forceDebug?: boolean);
	public constructor(id: string, name: string, serverOptions: ServerOptions, clientOptions: LanguageClientOptions, forceDebug?: boolean);
	public constructor(arg1: string, arg2: ServerOptions | string, arg3: LanguageClientOptions | ServerOptions, arg4?: boolean | LanguageClientOptions, arg5?: boolean) {
		let clientOptions: LanguageClientOptions;
		let forceDebug: boolean;
		if (is.string(arg2)) {
			this._id = arg1;
			this._name = arg2;
			this._serverOptions = arg3 as ServerOptions;
			clientOptions = arg4 as LanguageClientOptions;
			forceDebug = !!arg5;
		} else {
			this._id = arg1.toLowerCase();
			this._name = arg1;
			this._serverOptions = arg2 as ServerOptions;
			clientOptions = arg3 as LanguageClientOptions;
			forceDebug = arg4 as boolean;
		}
		if (forceDebug === void 0) { forceDebug = false; }

		clientOptions = clientOptions || {};
		this._clientOptions = {
			documentSelector: is.typedArray<string>(clientOptions.documentSelector, is.string)
				? clientOptions.documentSelector.map(element => { return { language: element }; })
				: clientOptions.documentSelector,
			synchronize: clientOptions.synchronize || {},
			diagnosticCollectionName: clientOptions.diagnosticCollectionName,
			outputChannelName: clientOptions.outputChannelName || this._name,
			revealOutputChannelOn: clientOptions.revealOutputChannelOn || RevealOutputChannelOn.Error,
			stdioEncoding: clientOptions.stdioEncoding || 'utf8',
			initializationOptions: clientOptions.initializationOptions,
			initializationFailedHandler: clientOptions.initializationFailedHandler,
			errorHandler: clientOptions.errorHandler || new DefaultErrorHandler(this._name),
			uriConverters: clientOptions.uriConverters
		};
		this._clientOptions.synchronize = this._clientOptions.synchronize || {};
		this._forceDebug = forceDebug;

		this.state = ClientState.Initial;
		this._connectionPromise = undefined;
		this._resolvedConnection = undefined;
		this._childProcess = undefined;
		this._outputChannel = undefined;

		this._listeners = undefined;
		this._providers = undefined;
		this._diagnostics = undefined;

		this._fileEvents = [];
		this._fileEventDelayer = new Delayer<void>(250);
		this._onReady = new Promise<void>((resolve, reject) => {
			this._onReadyCallbacks = { resolve, reject };
		});
		this._telemetryEmitter = new Emitter<any>();
		this._stateChangeEmitter = new Emitter<StateChangeEvent>();
		this._tracer = {
			log: (message: string, data?: string) => {
				this.logTrace(message, data);
			}
		};
		this._c2p = c2p.createConverter(clientOptions.uriConverters ? clientOptions.uriConverters.code2Protocol : undefined);
		this._p2c = p2c.createConverter(clientOptions.uriConverters ? clientOptions.uriConverters.protocol2Code : undefined);
	}

	private get state(): ClientState {
		return this._state;
	}

	private set state(value: ClientState) {
		let oldState = this.getPublicState();
		this._state = value;
		let newState = this.getPublicState();
		if (newState !== oldState) {
			this._stateChangeEmitter.fire({ oldState, newState });
		}
	}

	private getPublicState(): State {
		if (this.state === ClientState.Running) {
			return State.Running;
		} else {
			return State.Stopped;
		}
	}

	public sendRequest<R, E, RO>(type: RequestType0<R, E, RO>, token?: CancellationToken): Thenable<R>;
	public sendRequest<P, R, E, RO>(type: RequestType<P, R, E, RO>, params: P, token?: CancellationToken): Thenable<R>;
	public sendRequest<R>(method: string, token?: CancellationToken): Thenable<R>;
	public sendRequest<R>(method: string, param: any, token?: CancellationToken): Thenable<R>;
	public sendRequest<R>(type: string | RPCMessageType, ...params: any[]): Thenable<R> {
		if (!this.isConnectionActive()) {
			throw new Error('Language client is not ready yet');
		}
		this.forceDocumentSync();
		try {
			return this._resolvedConnection!.sendRequest<R>(type, ...params);
		} catch (error) {
			this.error(`Sending request ${is.string(type) ? type : type.method} failed.`, error);
			throw error;
		}
	}

	public onRequest<R, E, RO>(type: RequestType0<R, E, RO>, handler: RequestHandler0<R, E>): void;
	public onRequest<P, R, E, RO>(type: RequestType<P, R, E, RO>, handler: RequestHandler<P, R, E>): void;
	public onRequest<R, E>(method: string, handler: GenericRequestHandler<R, E>): void;
	public onRequest<R, E>(type: string | RPCMessageType, handler: GenericRequestHandler<R, E>): void {
		if (!this.isConnectionActive()) {
			throw new Error('Language client is not ready yet');
		}
		try {
			this._resolvedConnection!.onRequest(type, handler);
		} catch (error) {
			this.error(`Registering request handler ${is.string(type) ? type : type.method} failed.`, error);
			throw error;
		}
	}

	public sendNotification<RO>(type: NotificationType0<RO>): void;
	public sendNotification<P, RO>(type: NotificationType<P, RO>, params?: P): void;
	public sendNotification(method: string): void;
	public sendNotification(method: string, params: any): void;
	public sendNotification<P>(type: string | RPCMessageType, params?: P): void {
		if (!this.isConnectionActive()) {
			throw new Error('Language client is not ready yet');
		}
		this.forceDocumentSync();
		try {
			this._resolvedConnection!.sendNotification(type, params);
		} catch (error) {
			this.error(`Sending notification ${is.string(type) ? type : type.method} failed.`, error);
			throw error;
		}
	}

	public onNotification<RO>(type: NotificationType0<RO>, handler: NotificationHandler0): void;
	public onNotification<P, RO>(type: NotificationType<P, RO>, handler: NotificationHandler<P>): void;
	public onNotification(method: string, handler: GenericNotificationHandler): void;
	public onNotification(type: string | RPCMessageType, handler: GenericNotificationHandler): void {
		if (!this.isConnectionActive()) {
			throw new Error('Language client is not ready yet');
		}
		try {
			this._resolvedConnection!.onNotification(type, handler);
		} catch (error) {
			this.error(`Registering notification handler ${is.string(type) ? type : type.method} failed.`, error);
			throw error;
		}
	}

	public get protocol2CodeConverter(): p2c.Converter {
		return this._p2c;
	}

	public get code2ProtocolConverter(): c2p.Converter {
		return this._c2p;
	}

	public get onTelemetry(): Event<any> {
		return this._telemetryEmitter.event;
	}

	public get onDidChangeState(): Event<StateChangeEvent> {
		return this._stateChangeEmitter.event;
	}

	public get outputChannel(): OutputChannel {
		if (!this._outputChannel) {
			this._outputChannel = Window.createOutputChannel(this._clientOptions.outputChannelName ? this._clientOptions.outputChannelName : this._name);
		}
		return this._outputChannel;
	}

	public get diagnostics(): DiagnosticCollection | undefined {
		return this._diagnostics;
	}

	public createDefaultErrorHandler(): ErrorHandler {
		return new DefaultErrorHandler(this._name);
	}

	public set trace(value: Trace) {
		this._trace = value;
		this.onReady().then(() => {
			this.resolveConnection().then((connection) => {
				connection.trace(value, this._tracer);
			})
		}, () => {
		});
	}

	private data2String(data: any): string {
		if (data instanceof ResponseError) {
			const responseError = data as ResponseError<any>;
			return `  Message: ${responseError.message}\n  Code: ${responseError.code} ${responseError.data ? '\n' + responseError.data.toString() : ''}`
		}
		if (data instanceof Error) {
			if (is.string(data.stack)) {
				return data.stack;
			}
			return (data as Error).message;
		}
		if (is.string(data)) {
			return data;
		}
		return data.toString();
	}

	public info(message: string, data?: any): void {
		this.outputChannel.appendLine(`[Info  - ${(new Date().toLocaleTimeString())}] ${message}`);
		if (data) {
			this.outputChannel.appendLine(this.data2String(data));
		}
		if (this._clientOptions.revealOutputChannelOn <= RevealOutputChannelOn.Info) {
			this.outputChannel.show(true);
		}
	}

	public warn(message: string, data?: any): void {
		this.outputChannel.appendLine(`[Warn  - ${(new Date().toLocaleTimeString())}] ${message}`);
		if (data) {
			this.outputChannel.appendLine(this.data2String(data));
		}
		if (this._clientOptions.revealOutputChannelOn <= RevealOutputChannelOn.Warn) {
			this.outputChannel.show(true);
		}
	}

	public error(message: string, data?: any): void {
		this.outputChannel.appendLine(`[Error - ${(new Date().toLocaleTimeString())}] ${message}`);
		if (data) {
			this.outputChannel.appendLine(this.data2String(data));
		}
		if (this._clientOptions.revealOutputChannelOn <= RevealOutputChannelOn.Error) {
			this.outputChannel.show(true);
		}
	}

	private logTrace(message: string, data?: any): void {
		this.outputChannel.appendLine(`[Trace - ${(new Date().toLocaleTimeString())}] ${message}`);
		if (data) {
			this.outputChannel.appendLine(this.data2String(data));
		}
		this.outputChannel.show(true);
	}

	public needsStart(): boolean {
		return this.state === ClientState.Initial || this.state === ClientState.Stopping || this.state === ClientState.Stopped;
	}

	public needsStop(): boolean {
		return this.state === ClientState.Starting || this.state === ClientState.Running;
	}

	public onReady(): Promise<void> {
		return this._onReady;
	}

	private isConnectionActive(): boolean {
		return this.state === ClientState.Running && !!this._resolvedConnection;
	}

	public start(): Disposable {
		this._listeners = [];
		this._providers = [];
		// If we restart then the diagnostics collection is reused.
		if (!this._diagnostics) {
			this._diagnostics = this._clientOptions.diagnosticCollectionName
				? Languages.createDiagnosticCollection(this._clientOptions.diagnosticCollectionName)
				: Languages.createDiagnosticCollection();
		}

		this.state = ClientState.Starting;
		this.resolveConnection().then((connection) => {
			connection.onLogMessage((message) => {
				switch (message.type) {
					case MessageType.Error:
						this.error(message.message);
						break;
					case MessageType.Warning:
						this.warn(message.message);
						break;
					case MessageType.Info:
						this.info(message.message);
						break;
					default:
						this.outputChannel.appendLine(message.message);
				}
			});
			connection.onShowMessage((message) => {
				switch (message.type) {
					case MessageType.Error:
						Window.showErrorMessage(message.message);
						break;
					case MessageType.Warning:
						Window.showWarningMessage(message.message);
						break;
					case MessageType.Info:
						Window.showInformationMessage(message.message);
						break;
					default:
						Window.showInformationMessage(message.message);
				}
			});
			connection.onRequest(ShowMessageRequest.type, (params) => {
				let messageFunc: <T extends MessageItem>(message: string, ...items: T[]) => Thenable<T>;
				switch (params.type) {
					case MessageType.Error:
						messageFunc = Window.showErrorMessage;
						break;
					case MessageType.Warning:
						messageFunc = Window.showWarningMessage;
						break;
					case MessageType.Info:
						messageFunc = Window.showInformationMessage;
						break;
					default:
						messageFunc = Window.showInformationMessage;
				}
				let actions = params.actions || [];
				return messageFunc(params.message, ...actions);
			});
			connection.onTelemetry((data) => {
				this._telemetryEmitter.fire(data);
			});
			this.initRegistrationHandlers(connection);
			connection.listen();
			// Error is handled in the intialize call.
			this.initialize(connection).then(undefined, () => { });
		}, (error) => {
			this.state = ClientState.StartFailed;
			this._onReadyCallbacks.reject(error);
			this.error('Starting client failed', error);
			Window.showErrorMessage(`Couldn't start client ${this._name}`);
		});
		return new Disposable(() => {
			if (this.needsStop()) {
				this.stop();
			}
		});
	}

	private resolveConnection(): Thenable<IConnection> {
		if (!this._connectionPromise) {
			this._connectionPromise = this.createConnection();
		}
		return this._connectionPromise;
	}

	private initialize(connection: IConnection): Thenable<InitializeResult> {
		this.refreshTrace(connection, false);
		let initOption = this._clientOptions.initializationOptions;
		let initParams: InitializeParams = {
			processId: process.pid,
			rootPath: Workspace.rootPath ? Workspace.rootPath : null,
			rootUri: Workspace.rootPath ? Uri.file(Workspace.rootPath).toString() : null,
			capabilities: clientCapabilities,
			initializationOptions: is.func(initOption) ? initOption() : initOption,
			trace: Trace.toString(this._trace)
		};
		return connection.initialize(initParams).then((result) => {
			this._resolvedConnection = connection;
			this.state = ClientState.Running;

			this._capabilites = result.capabilities;

			connection.onDiagnostics(params => this.handleDiagnostics(params));
			connection.onRequest(RegistrationRequest.type, params => this.handleRegistrationRequest(params));
			connection.onRequest(UnregistrationRequest.type, params => this.handleUnregistrationRequest(params));
			connection.onRequest(ApplyWorkspaceEditRequest.type, params => this.handleApplyWorkspaceEdit(params));
			connection.sendNotification(InitializedNotification.type, {});

			this.hookFileEvents(connection);
			this.hookConfigurationChanged(connection);
			if (this._clientOptions.documentSelector) {
				let selectorOptions: TextDocumentRegistrationOptions = { documentSelector: this._clientOptions.documentSelector };
				let textDocumentSyncOptions: TextDocumentSyncOptions | undefined = undefined;
				if (is.number(this._capabilites.textDocumentSync) && this._capabilites.textDocumentSync !== TextDocumentSyncKind.None) {
					textDocumentSyncOptions = {
						openClose: true,
						change: this._capabilites.textDocumentSync,
						save: {
							includeText: false
						}
					};
				} else if (this._capabilites.textDocumentSync !== void 0 && this._capabilites.textDocumentSync === null) {
					textDocumentSyncOptions = this._capabilites.textDocumentSync as TextDocumentSyncOptions;
				}
				if (textDocumentSyncOptions) {
					let registeredHandlers: FeatureHandlerMap = this._registeredHandlers as FeatureHandlerMap;
					if (textDocumentSyncOptions.openClose) {
						registeredHandlers.get(DidOpenTextDocumentNotification.type.method).register(
							{ id: UUID.generateUuid(), registerOptions: selectorOptions }
						);
						registeredHandlers.get(DidCloseTextDocumentNotification.type.method).register(
							{ id: UUID.generateUuid(), registerOptions: selectorOptions }
						);
					}
					if (textDocumentSyncOptions.change !== TextDocumentSyncKind.None) {
						registeredHandlers.get(DidChangeTextDocumentNotification.type.method).register(
							{
								id: UUID.generateUuid(),
								registerOptions: Object.assign({}, selectorOptions, { syncKind: textDocumentSyncOptions.change }) as TextDocumentChangeRegistrationOptions
							}
						);
					}
					if (textDocumentSyncOptions.willSave) {
						registeredHandlers.get(WillSaveTextDocumentNotification.type.method).register(
							{ id: UUID.generateUuid(), registerOptions: selectorOptions }
						);
					}
					if (textDocumentSyncOptions.willSaveWaitUntil) {
						registeredHandlers.get(WillSaveTextDocumentWaitUntilRequest.type.method).register(
							{ id: UUID.generateUuid(), registerOptions: selectorOptions }
						);
					}
					if (textDocumentSyncOptions.save) {
						registeredHandlers.get(DidSaveTextDocumentNotification.type.method).register(
							{
								id: UUID.generateUuid(),
								registerOptions: Object.assign({}, selectorOptions, { includeText: !!textDocumentSyncOptions.save.includeText }) as TextDocumentSaveRegistrationOptions
							}
						);
					}
				}
			}
			this.hookCapabilities(connection);
			this._onReadyCallbacks.resolve();
			return result;
		}, (error: any) => {
			if (this._clientOptions.initializationFailedHandler) {
				if (this._clientOptions.initializationFailedHandler(error)) {
					this.initialize(connection);
				} else {
					this.stop();
					this._onReadyCallbacks.reject(error);
				}
			} else if (error instanceof ResponseError && error.data && error.data.retry) {
				Window.showErrorMessage(error.message, { title: 'Retry', id: "retry" }).then(item => {
					if (item && item.id === 'retry') {
						this.initialize(connection);
					} else {
						this.stop();
						this._onReadyCallbacks.reject(error);
					}
				});
			} else {
				if (error && error.message) {
					Window.showErrorMessage(error.message);
				}
				this.error('Server initialization failed.', error);
				this.stop();
				this._onReadyCallbacks.reject(error);
			}
		});
	}

	public stop(): Thenable<void> {
		if (!this._connectionPromise) {
			this.state = ClientState.Stopped;
			return Promise.resolve();
		}
		this.state = ClientState.Stopping;
		this.cleanUp();
		// unkook listeners
		return this.resolveConnection().then(connection => {
			return connection.shutdown().then(() => {
				connection.exit();
				connection.dispose();
				this.state = ClientState.Stopped;
				this._connectionPromise = undefined;
				this._resolvedConnection = undefined;
				let toCheck = this._childProcess;
				this._childProcess = undefined;
				// Remove all markers
				this.checkProcessDied(toCheck);
			})
		});
	}

	private cleanUp(diagnostics: boolean = true): void {
		if (this._listeners) {
			this._listeners.forEach(listener => listener.dispose());
			this._listeners = undefined;
		}
		if (this._providers) {
			this._providers.forEach(provider => provider.dispose());
			this._providers = undefined;
		}
		if (diagnostics && this._diagnostics) {
			this._diagnostics.dispose();
			this._diagnostics = undefined;
		}
	}

	private notifyFileEvent(event: FileEvent): void {
		this._fileEvents.push(event);
		this._fileEventDelayer.trigger(() => {
			this.onReady().then(() => {
				this.resolveConnection().then(connection => {
					if (this.isConnectionActive()) {
						connection.didChangeWatchedFiles({ changes: this._fileEvents });
					}
					this._fileEvents = [];
				})
			}, (error) => {
				this.error(`Notify file events failed.`, error);
			});
		});
	}

	private forceDocumentSync(): void {
		(this._registeredHandlers.get(DidChangeTextDocumentNotification.type.method) as DidChangeTextDocumentFeature).forceDelivery();
	}

	private handleDiagnostics(params: PublishDiagnosticsParams) {
		if (!this._diagnostics) {
			return;
		}
		let uri = this._p2c.asUri(params.uri);
		let diagnostics = this._p2c.asDiagnostics(params.diagnostics);
		this._diagnostics.set(uri, diagnostics);
	}

	private createConnection(): Thenable<IConnection> {
		function getEnvironment(env: any): any {
			if (!env) {
				return process.env;
			}
			let result: any = Object.create(null);
			Object.keys(process.env).forEach(key => result[key] = process.env[key]);
			Object.keys(env).forEach(key => result[key] = env[key]);
		}

		function startedInDebugMode(): boolean {
			let args: string[] = (process as any).execArgv;
			if (args) {
				return args.some((arg) => /^--debug=?/.test(arg) || /^--debug-brk=?/.test(arg));
			};
			return false;
		}

		let encoding = this._clientOptions.stdioEncoding;

		let errorHandler = (error: Error, message: Message, count: number) => {
			this.handleConnectionError(error, message, count);
		}

		let closeHandler = () => {
			this.handleConnectionClosed();
		}

		let server = this._serverOptions;
		// We got a function.
		if (is.func(server)) {
			return server().then((result) => {
				let info = result as StreamInfo;
				if (info.writer && info.reader) {
					return createConnection(info.reader, info.writer, errorHandler, closeHandler);
				} else {
					let cp = result as ChildProcess;
					return createConnection(cp.stdout, cp.stdin, errorHandler, closeHandler);
				}
			});
		}
		let json: { command?: string; module?: string };
		let runDebug = <{ run: any; debug: any; }>server;
		if (runDebug.run || runDebug.debug) {
			// We are under debugging. So use debug as well.
			if (typeof v8debug === 'object' || this._forceDebug || startedInDebugMode()) {
				json = runDebug.debug;
			} else {
				json = runDebug.run;
			}
		} else {
			json = server;
		}
		if (json.module) {
			let node: NodeModule = <NodeModule>json;
			if (node.runtime) {
				let args: string[] = [];
				let options: ForkOptions = node.options || Object.create(null);
				if (options.execArgv) {
					options.execArgv.forEach(element => args.push(element));
				}
				args.push(node.module);
				if (node.args) {
					node.args.forEach(element => args.push(element));
				}
				let execOptions: ExecutableOptions = Object.create(null);
				execOptions.cwd = options.cwd || Workspace.rootPath;
				execOptions.env = getEnvironment(options.env);
				let pipeName: string | undefined = undefined;
				if (node.transport === TransportKind.ipc) {
					// exec options not correctly typed in lib
					execOptions.stdio = <any>[null, null, null, 'ipc'];
					args.push('--node-ipc');
				} else if (node.transport === TransportKind.stdio) {
					args.push('--stdio');
				} else if (node.transport === TransportKind.pipe) {
					pipeName = generateRandomPipeName();
					args.push(`--pipe=${pipeName}`);
				}
				if (node.transport === TransportKind.ipc || node.transport === TransportKind.stdio) {
					let process = cp.spawn(node.runtime, args, execOptions);
					if (!process || !process.pid) {
						return Promise.reject<IConnection>(`Launching server using runtime ${node.runtime} failed.`);
					}
					this._childProcess = process;
					process.stderr.on('data', data => this.outputChannel.append(is.string(data) ? data : data.toString(encoding)));
					if (node.transport === TransportKind.ipc) {
						process.stdout.on('data', data => this.outputChannel.append(is.string(data) ? data : data.toString(encoding)));
						return Promise.resolve(createConnection(new IPCMessageReader(process), new IPCMessageWriter(process), errorHandler, closeHandler));
					} else {
						return Promise.resolve(createConnection(process.stdout, process.stdin, errorHandler, closeHandler));
					}
				} else if (node.transport == TransportKind.pipe) {
					return createClientPipeTransport(pipeName!).then((transport) => {
						let process = cp.spawn(node.runtime!, args, execOptions);
						if (!process || !process.pid) {
							return Promise.reject<IConnection>(`Launching server using runtime ${node.runtime} failed.`);
						}
						this._childProcess = process;
						process.stderr.on('data', data => this.outputChannel.append(is.string(data) ? data : data.toString(encoding)));
						process.stdout.on('data', data => this.outputChannel.append(is.string(data) ? data : data.toString(encoding)));
						return transport.onConnected().then((protocol) => {
							return createConnection(protocol[0], protocol[1], errorHandler, closeHandler);
						});
					})
				}
			} else {
				let pipeName: string | undefined = undefined;
				return new Promise<IConnection>((resolve, reject) => {
					let args = node.args && node.args.slice() || [];
					if (node.transport === TransportKind.ipc) {
						args.push('--node-ipc');
					} else if (node.transport === TransportKind.stdio) {
						args.push('--stdio');
					} else if (node.transport === TransportKind.pipe) {
						pipeName = generateRandomPipeName();
						args.push(`--pipe=${pipeName}`);
					}
					let options: ForkOptions = node.options || Object.create(null);
					options.execArgv = options.execArgv || [];
					options.cwd = options.cwd || Workspace.rootPath;
					if (node.transport === TransportKind.ipc || node.transport === TransportKind.stdio) {
						electron.fork(node.module, args || [], options, (error, cp) => {
							if (error || !cp) {
								reject(error);
							} else {
								this._childProcess = cp;
								cp.stderr.on('data', data => this.outputChannel.append(is.string(data) ? data : data.toString(encoding)));
								if (node.transport === TransportKind.ipc) {
									cp.stdout.on('data', data => this.outputChannel.append(is.string(data) ? data : data.toString(encoding)));
									resolve(createConnection(new IPCMessageReader(this._childProcess), new IPCMessageWriter(this._childProcess), errorHandler, closeHandler));
								} else {
									resolve(createConnection(cp.stdout, cp.stdin, errorHandler, closeHandler));
								}
							}
						});
					}  else if (node.transport === TransportKind.pipe) {
						createClientPipeTransport(pipeName!).then((transport) => {
							electron.fork(node.module, args || [], options, (error, cp) => {
								if (error || !cp) {
									reject(error);
								} else {
									this._childProcess = cp;
									cp.stderr.on('data', data => this.outputChannel.append(is.string(data) ? data : data.toString(encoding)));
									cp.stdout.on('data', data => this.outputChannel.append(is.string(data) ? data : data.toString(encoding)));
									transport.onConnected().then((protocol) => {
										resolve(createConnection(protocol[0], protocol[1], errorHandler, closeHandler));
									});
								}
							});
						});
					}
				});
			}
		} else if (json.command) {
			let command: Executable = <Executable>json;
			let options = command.options || {};
			options.cwd = options.cwd || Workspace.rootPath;
			let process = cp.spawn(command.command, command.args, command.options);
			if (!process || !process.pid) {
				return Promise.reject<IConnection>(`Launching server using command ${command.command} failed.`);
			}
			process.stderr.on('data', data => this.outputChannel.append(is.string(data) ? data : data.toString(encoding)));
			this._childProcess = process;
			return Promise.resolve(createConnection(process.stdout, process.stdin, errorHandler, closeHandler));
		}
		return Promise.reject<IConnection>(new Error(`Unsupported server configuartion ` + JSON.stringify(server, null, 4)));
	}

	private handleConnectionClosed() {
		// Check whether this is a normal shutdown in progress or the client stopped normally.
		if (this.state === ClientState.Stopping || this.state === ClientState.Stopped) {
			return;
		}
		this._connectionPromise = undefined;
		this._resolvedConnection = undefined;
		this._childProcess = undefined;
		let action = this._clientOptions.errorHandler!.closed();
		if (action === CloseAction.DoNotRestart) {
			this.error('Connection to server got closed. Server will not be restarted.');
			this.state = ClientState.Stopped;
			this.cleanUp();
		} else if (action === CloseAction.Restart) {
			this.info('Connection to server got closed. Server will restart.');
			this.cleanUp(false);
			this.state = ClientState.Initial;
			this.start();
		}
	}

	private handleConnectionError(error: Error, message: Message, count: number) {
		let action = this._clientOptions.errorHandler!.error(error, message, count);
		if (action === ErrorAction.Shutdown) {
			this.error('Connection to server is erroring. Shutting down server.')
			this.stop();
		}
	}

	private checkProcessDied(childProcess: ChildProcess | undefined): void {
		if (!childProcess) {
			return;
		}
		setTimeout(() => {
			// Test if the process is still alive. Throws an exception if not
			try {
				process.kill(childProcess.pid, <any>0);
				terminate(childProcess);
			} catch (error) {
				// All is fine.
			}
		}, 2000);
	}

	private hookConfigurationChanged(connection: IConnection): void {
		if (!this._clientOptions.synchronize!.configurationSection) {
			return;
		}
		Workspace.onDidChangeConfiguration(() => this.onDidChangeConfiguration(connection), this, this._listeners);
		this.onDidChangeConfiguration(connection);
	}

	private refreshTrace(connection: IConnection, sendNotification: boolean = false): void {
		let config = Workspace.getConfiguration(this._id);
		let trace: Trace = Trace.Off;
		if (config) {
			trace = Trace.fromString(config.get('trace.server', 'off'));
		}
		this._trace = trace;
		connection.trace(this._trace, this._tracer, sendNotification);
	}

	private onDidChangeConfiguration(connection: IConnection): void {
		this.refreshTrace(connection, true);
		let keys: string[] | undefined;
		let configurationSection = this._clientOptions.synchronize!.configurationSection;
		if (is.string(configurationSection)) {
			keys = [configurationSection];
		} else if (is.stringArray(configurationSection)) {
			keys = configurationSection;
		}
		if (keys) {
			if (this.isConnectionActive()) {
				connection.didChangeConfiguration({ settings: this.extractSettingsInformation(keys) });
			}
		}
	}

	private extractSettingsInformation(keys: string[]): any {
		function ensurePath(config: any, path: string[]): any {
			let current = config;
			for (let i = 0; i < path.length - 1; i++) {
				let obj = current[path[i]];
				if (!obj) {
					obj = Object.create(null);
					current[path[i]] = obj;
				}
				current = obj;
			}
			return current;
		}
		let result = Object.create(null);
		for (let i = 0; i < keys.length; i++) {
			let key = keys[i];
			let index: number = key.indexOf('.');
			let config: any = null;
			if (index >= 0) {
				config = Workspace.getConfiguration(key.substr(0, index)).get(key.substr(index + 1));
			} else {
				config = Workspace.getConfiguration(key);
			}
			if (config) {
				let path = keys[i].split('.');
				ensurePath(result, path)[path[path.length - 1]] = config;
			}
		}
		return result;
	}

	private hookFileEvents(_connection: IConnection): void {
		let fileEvents = this._clientOptions.synchronize!.fileEvents;
		if (!fileEvents) {
			return;
		}
		let watchers: FileSystemWatcher[];
		if (is.array(fileEvents)) {
			watchers = <FileSystemWatcher[]>fileEvents;
		} else {
			watchers = [<FileSystemWatcher>fileEvents];
		}
		if (!watchers) {
			return;
		}
		watchers.forEach(watcher => {
			watcher.onDidCreate((resource) => this.notifyFileEvent(
				{
					uri: resource.toString(),
					type: FileChangeType.Created
				}
			), null, this._listeners);
			watcher.onDidChange((resource) => this.notifyFileEvent(
				{
					uri: resource.toString(),
					type: FileChangeType.Changed
				}

			), null, this._listeners);
			watcher.onDidDelete((resource) => this.notifyFileEvent(
				{
					uri: resource.toString(),
					type: FileChangeType.Deleted
				}
			), null, this._listeners);
		})
	}

	private _registeredHandlers: Map<string, FeatureHandler<any>> = new Map<string, FeatureHandler<any>>();
	private initRegistrationHandlers(_connection: IConnection) {
		const syncedDocuments: Map<string, TextDocument> = new Map<string, TextDocument>();
		const logger = (type: RPCMessageType, error: any): void => { this.logFailedRequest(type, error); };
		this._registeredHandlers.set(
			DidOpenTextDocumentNotification.type.method,
			new DidOpenTextDocumentFeature(this, syncedDocuments)
		);
		this._registeredHandlers.set(
			DidChangeTextDocumentNotification.type.method,
			new DidChangeTextDocumentFeature(this)
		);
		this._registeredHandlers.set(
			WillSaveTextDocumentNotification.type.method,
			new DocumentNotifiactions<WillSaveTextDocumentParams, TextDocumentWillSaveEvent>(
				this, Workspace.onWillSaveTextDocument, WillSaveTextDocumentNotification.type,
				(willSaveEvent) => this._c2p.asWillSaveTextDocumentParams(willSaveEvent),
				(selectors, willSaveEvent) => DocumentNotifiactions.textDocumentFilter(selectors, willSaveEvent.document)
			)
		);
		this._registeredHandlers.set(
			WillSaveTextDocumentWaitUntilRequest.type.method,
			new WillSaveWaitUntilFeature(this)
		);
		this._registeredHandlers.set(
			DidSaveTextDocumentNotification.type.method,
			new DidSaveTextDocumentFeature(this)
		);
		this._registeredHandlers.set(
			DidCloseTextDocumentNotification.type.method,
			new DidCloseTextDocumentFeature(this, syncedDocuments)
		);
		this._registeredHandlers.set(
			CompletionRequest.type.method,
			new LanguageFeature<CompletionRegistrationOptions>((options) => this.createCompletionProvider(options))
		);
		this._registeredHandlers.set(
			HoverRequest.type.method,
			new LanguageFeature<TextDocumentRegistrationOptions>((options) => this.createHoverProvider(options))
		);
		this._registeredHandlers.set(
			SignatureHelpRequest.type.method,
			new LanguageFeature<SignatureHelpRegistrationOptions>((options) => this.createSignatureHelpProvider(options))
		);
		this._registeredHandlers.set(
			DefinitionRequest.type.method,
			new LanguageFeature<TextDocumentRegistrationOptions>((options) => this.createDefinitionProvider(options))
		);
		this._registeredHandlers.set(
			ReferencesRequest.type.method,
			new LanguageFeature<TextDocumentRegistrationOptions>((options) => this.createReferencesProvider(options))
		);
		this._registeredHandlers.set(
			DocumentHighlightRequest.type.method,
			new LanguageFeature<TextDocumentRegistrationOptions>((options) => this.createDocumentHighlightProvider(options))
		);
		this._registeredHandlers.set(
			DocumentSymbolRequest.type.method,
			new LanguageFeature<TextDocumentRegistrationOptions>((options) => this.createDocumentSymbolProvider(options))
		);
		this._registeredHandlers.set(
			WorkspaceSymbolRequest.type.method,
			new LanguageFeature<TextDocumentRegistrationOptions>((options) => this.createWorkspaceSymbolProvider(options))
		);
		this._registeredHandlers.set(
			CodeActionRequest.type.method,
			new LanguageFeature<TextDocumentRegistrationOptions>((options) => this.createCodeActionsProvider(options))
		);
		this._registeredHandlers.set(
			CodeLensRequest.type.method,
			new LanguageFeature<CodeLensRegistrationOptions>((options) => this.createCodeLensProvider(options))
		);
		this._registeredHandlers.set(
			DocumentFormattingRequest.type.method,
			new LanguageFeature<TextDocumentRegistrationOptions>((options) => this.createDocumentFormattingProvider(options))
		);
		this._registeredHandlers.set(
			DocumentRangeFormattingRequest.type.method,
			new LanguageFeature<TextDocumentRegistrationOptions>((options) => this.createDocumentRangeFormattingProvider(options))
		);
		this._registeredHandlers.set(
			DocumentOnTypeFormattingRequest.type.method,
			new LanguageFeature<DocumentOnTypeFormattingRegistrationOptions>((options) => this.createDocumentOnTypeFormattingProvider(options))
		);
		this._registeredHandlers.set(
			RenameRequest.type.method,
			new LanguageFeature<TextDocumentRegistrationOptions>((options) => this.createRenameProvider(options))
		);
		this._registeredHandlers.set(
			DocumentLinkRequest.type.method,
			new LanguageFeature<DocumentLinkRegistrationOptions>((options) => this.createDocumentLinkProvider(options))
		);
		this._registeredHandlers.set(
			ExecuteCommandRequest.type.method,
			new ExecuteCommandFeature(this, logger)
		);
	}

	private handleRegistrationRequest(params: RegistrationParams): Thenable<void> {
		return new Promise<void>((resolve, _reject) => {
			params.registrations.forEach((element) => {
				const handler = this._registeredHandlers.get(element.method);
				const options = element.registerOptions || {};
				options.documentSelector = options.documentSelector || this._clientOptions.documentSelector;
				const data: RegistrationData<any> = {
					id: element.id,
					registerOptions: options
				}
				if (handler) {
					handler.register(data);
				}
			});
			resolve();
		});
	}

	private handleUnregistrationRequest(params: UnregistrationParams): Thenable<void> {
		return new Promise<void>((resolve, _reject) => {
			params.unregisterations.forEach((element) => {
				const handler = this._registeredHandlers.get(element.method);
				if (handler) {
					handler.unregister(element.id);
				}
			});
			resolve();
		});
	}

	private handleApplyWorkspaceEdit(params: ApplyWorkspaceEditParams): Thenable<ApplyWorkspaceEditResponse> {
		// This is some sort of workaround since the version check should be done by VS Code in the Workspace.applyEdit.
		// However doing it here adds some safety since the server can lag more behind then an extension.
		let workspaceEdit: WorkspaceEdit = params.edit;
		let openTextDocuments: Map<string, TextDocument> = new Map<string, TextDocument>();
		Workspace.textDocuments.forEach((document) => openTextDocuments.set(document.uri.toString(), document));
		let versionMismatch = false;
		if (workspaceEdit.changes) {
			for (const change of workspaceEdit.changes) {
				if (change.textDocument.version && change.textDocument.version >= 0) {
					let textDocument = openTextDocuments.get(change.textDocument.uri);
					if (textDocument && textDocument.version !== change.textDocument.version) {
						versionMismatch = true;
						break;
					}
				}
			}
		}
		if (versionMismatch) {
			return Promise.resolve({ applied: false });
		}
		return Workspace.applyEdit(this._p2c.asWorkspaceEdit(params.edit)).then((value) => { return { applied: value }; });
	};

	private hookCapabilities(_connection: IConnection): void {
		let documentSelector = this._clientOptions.documentSelector;
		if (!documentSelector) {
			return;
		}
		let selectorOptions: TextDocumentRegistrationOptions = { documentSelector: documentSelector };
		let registeredHandlers: FeatureHandlerMap = this._registeredHandlers as FeatureHandlerMap;
		if (this._capabilites.completionProvider) {
			let options: CompletionRegistrationOptions = Object.assign({}, selectorOptions, this._capabilites.completionProvider);
			registeredHandlers.get(CompletionRequest.type.method).register(
				{ id: UUID.generateUuid(), registerOptions: options }
			);
		}
		if (this._capabilites.hoverProvider) {
			registeredHandlers.get(HoverRequest.type.method).register(
				{ id: UUID.generateUuid(), registerOptions: Object.assign({}, selectorOptions) }
			);
		}
		if (this._capabilites.signatureHelpProvider) {
			let options: SignatureHelpRegistrationOptions = Object.assign({}, selectorOptions, this._capabilites.signatureHelpProvider);
			registeredHandlers.get(SignatureHelpRequest.type.method).register(
				{ id: UUID.generateUuid(), registerOptions: options }
			);
		}

		if (this._capabilites.definitionProvider) {
			registeredHandlers.get(DefinitionRequest.type.method).register(
				{ id: UUID.generateUuid(), registerOptions: Object.assign({}, selectorOptions) }
			);
		}

		if (this._capabilites.referencesProvider) {
			registeredHandlers.get(ReferencesRequest.type.method).register(
				{ id: UUID.generateUuid(), registerOptions: Object.assign({}, selectorOptions) }
			);
		}

		if (this._capabilites.documentHighlightProvider) {
			registeredHandlers.get(DocumentHighlightRequest.type.method).register(
				{ id: UUID.generateUuid(), registerOptions: Object.assign({}, selectorOptions) }
			);
		}

		if (this._capabilites.documentSymbolProvider) {
			registeredHandlers.get(DocumentSymbolRequest.type.method).register(
				{ id: UUID.generateUuid(), registerOptions: Object.assign({}, selectorOptions) }
			);
		}

		if (this._capabilites.workspaceSymbolProvider) {
			registeredHandlers.get(WorkspaceSymbolRequest.type.method).register(
				{ id: UUID.generateUuid(), registerOptions: Object.assign({}, selectorOptions) }
			);
		}

		if (this._capabilites.codeActionProvider) {
			registeredHandlers.get(CodeActionRequest.type.method).register(
				{ id: UUID.generateUuid(), registerOptions: Object.assign({}, selectorOptions) }
			);
		}

		if (this._capabilites.codeLensProvider) {
			let options: CodeLensRegistrationOptions = Object.assign({}, selectorOptions, this._capabilites.codeLensProvider);
			registeredHandlers.get(CodeLensRequest.type.method).register(
				{ id: UUID.generateUuid(), registerOptions: options }
			);
		}

		if (this._capabilites.documentFormattingProvider) {
			registeredHandlers.get(DocumentFormattingRequest.type.method).register(
				{ id: UUID.generateUuid(), registerOptions: Object.assign({}, selectorOptions) }
			);
		}

		if (this._capabilites.documentRangeFormattingProvider) {
			registeredHandlers.get(DocumentRangeFormattingRequest.type.method).register(
				{ id: UUID.generateUuid(), registerOptions: Object.assign({}, selectorOptions) }
			);
		}

		if (this._capabilites.documentOnTypeFormattingProvider) {
			let options: DocumentOnTypeFormattingRegistrationOptions = Object.assign({}, selectorOptions, this._capabilites.documentOnTypeFormattingProvider);
			registeredHandlers.get(DocumentOnTypeFormattingRequest.type.method).register(
				{ id: UUID.generateUuid(), registerOptions: options }
			);
		}

		if (this._capabilites.renameProvider) {
			registeredHandlers.get(RenameRequest.type.method).register(
				{ id: UUID.generateUuid(), registerOptions: Object.assign({}, selectorOptions) }
			);
		}

		if (this._capabilites.documentLinkProvider) {
			let options: DocumentLinkRegistrationOptions = Object.assign({}, selectorOptions, this._capabilites.documentLinkProvider);
			registeredHandlers.get(DocumentLinkRequest.type.method).register(
				{ id: UUID.generateUuid(), registerOptions: options }
			);
		}
		if (this._capabilites.executeCommandProvider) {
			let options: ExecuteCommandRegistrationOptions = Object.assign({}, this._capabilites.executeCommandProvider);
			registeredHandlers.get(ExecuteCommandRequest.type.method).register(
				{ id: UUID.generateUuid(), registerOptions: options }
			);
		}
	}

	protected logFailedRequest(type: RPCMessageType, error: any): void {
		this.error(`Request ${type.method} failed.`, error);
	}

	private createCompletionProvider(options: CompletionRegistrationOptions): Disposable {
		let triggerCharacters = options.triggerCharacters || [];
		return Languages.registerCompletionItemProvider(options.documentSelector!, {
			provideCompletionItems: (document: TextDocument, position: VPosition, token: CancellationToken): Thenable<VCompletionList | VCompletionItem[]> => {
				return this.sendRequest(CompletionRequest.type, this._c2p.asTextDocumentPositionParams(document, position), token).then(
					this._p2c.asCompletionResult,
					(error) => {
						this.logFailedRequest(CompletionRequest.type, error);
						return Promise.resolve([]);
					}
				);
			},
			resolveCompletionItem: options.resolveProvider
				? (item: VCompletionItem, token: CancellationToken): Thenable<VCompletionItem> => {
					return this.sendRequest(CompletionResolveRequest.type, this._c2p.asCompletionItem(item), token).then(
						this._p2c.asCompletionItem,
						(error) => {
							this.logFailedRequest(CompletionResolveRequest.type, error);
							return Promise.resolve(item);
						}
					);
				}
				: undefined
		}, ...triggerCharacters);
	}

	private createHoverProvider(options: TextDocumentRegistrationOptions): Disposable {
		return Languages.registerHoverProvider(options.documentSelector!, {
			provideHover: (document: TextDocument, position: VPosition, token: CancellationToken): Thenable<VHover> => {
				return this.sendRequest(HoverRequest.type, this._c2p.asTextDocumentPositionParams(document, position), token).then(
					this._p2c.asHover,
					(error) => {
						this.logFailedRequest(HoverRequest.type, error);
						return Promise.resolve(null);
					}
				);
			}
		});
	}

	private createSignatureHelpProvider(options: SignatureHelpRegistrationOptions): Disposable {
		let triggerCharacters = options.triggerCharacters || [];
		return Languages.registerSignatureHelpProvider(options.documentSelector!, {
			provideSignatureHelp: (document: TextDocument, position: VPosition, token: CancellationToken): Thenable<VSignatureHelp> => {
				return this.sendRequest(SignatureHelpRequest.type, this._c2p.asTextDocumentPositionParams(document, position), token).then(
					this._p2c.asSignatureHelp,
					(error) => {
						this.logFailedRequest(SignatureHelpRequest.type, error);
						return Promise.resolve(null);
					}
				);
			}
		}, ...triggerCharacters);
	}

	private createDefinitionProvider(options: TextDocumentRegistrationOptions): Disposable {
		return Languages.registerDefinitionProvider(options.documentSelector!, {
			provideDefinition: (document: TextDocument, position: VPosition, token: CancellationToken): Thenable<VDefinition> => {
				return this.sendRequest(DefinitionRequest.type, this._c2p.asTextDocumentPositionParams(document, position), token).then(
					this._p2c.asDefinitionResult,
					(error) => {
						this.logFailedRequest(DefinitionRequest.type, error);
						return Promise.resolve(null);
					}
				);
			}
		});
	}

	private createReferencesProvider(options: TextDocumentRegistrationOptions): Disposable {
		return Languages.registerReferenceProvider(options.documentSelector!, {
			provideReferences: (document: TextDocument, position: VPosition, options: { includeDeclaration: boolean; }, token: CancellationToken): Thenable<VLocation[]> => {
				return this.sendRequest(ReferencesRequest.type, this._c2p.asReferenceParams(document, position, options), token).then(
					this._p2c.asReferences,
					(error) => {
						this.logFailedRequest(ReferencesRequest.type, error);
						return Promise.resolve([]);
					}
				);
			}
		});
	}

	private createDocumentHighlightProvider(options: TextDocumentRegistrationOptions): Disposable {
		return Languages.registerDocumentHighlightProvider(options.documentSelector!, {
			provideDocumentHighlights: (document: TextDocument, position: VPosition, token: CancellationToken): Thenable<VDocumentHighlight[]> => {
				return this.sendRequest(DocumentHighlightRequest.type, this._c2p.asTextDocumentPositionParams(document, position), token).then(
					this._p2c.asDocumentHighlights,
					(error) => {
						this.logFailedRequest(DocumentHighlightRequest.type, error);
						return Promise.resolve([]);
					}
				);
			}
		});
	}

	private createDocumentSymbolProvider(options: TextDocumentRegistrationOptions): Disposable {
		return Languages.registerDocumentSymbolProvider(options.documentSelector!, {
			provideDocumentSymbols: (document: TextDocument, token: CancellationToken): Thenable<VSymbolInformation[]> => {
				return this.sendRequest(DocumentSymbolRequest.type, this._c2p.asDocumentSymbolParams(document), token).then(
					this._p2c.asSymbolInformations,
					(error) => {
						this.logFailedRequest(DocumentSymbolRequest.type, error);
						return Promise.resolve([]);
					}
				);
			}
		});
	}

	private createWorkspaceSymbolProvider(_options: TextDocumentRegistrationOptions): Disposable {
		return Languages.registerWorkspaceSymbolProvider({
			provideWorkspaceSymbols: (query: string, token: CancellationToken): Thenable<VSymbolInformation[]> => {
				return this.sendRequest(WorkspaceSymbolRequest.type, { query }, token).then(
					this._p2c.asSymbolInformations,
					(error) => {
						this.logFailedRequest(WorkspaceSymbolRequest.type, error);
						return Promise.resolve([]);
					}
				);
			}
		});
	}

	private createCodeActionsProvider(options: TextDocumentRegistrationOptions): Disposable {
		return Languages.registerCodeActionsProvider(options.documentSelector!, {
			provideCodeActions: (document: TextDocument, range: VRange, context: VCodeActionContext, token: CancellationToken): Thenable<VCommand[]> => {
				let params: CodeActionParams = {
					textDocument: this._c2p.asTextDocumentIdentifier(document),
					range: this._c2p.asRange(range),
					context: this._c2p.asCodeActionContext(context)
				};
				return this.sendRequest(CodeActionRequest.type, params, token).then(
					this._p2c.asCommands,
					(error) => {
						this.logFailedRequest(CodeActionRequest.type, error);
						return Promise.resolve([]);
					}
				);
			}
		});
	}

	private createCodeLensProvider(options: CodeLensRegistrationOptions): Disposable {
		return Languages.registerCodeLensProvider(options.documentSelector!, {
			provideCodeLenses: (document: TextDocument, token: CancellationToken): Thenable<VCodeLens[]> => {
				return this.sendRequest(CodeLensRequest.type, this._c2p.asCodeLensParams(document), token).then(
					this._p2c.asCodeLenses,
					(error) => {
						this.logFailedRequest(CodeLensRequest.type, error);
						return Promise.resolve([]);
					}
				);
			},
			resolveCodeLens: (options.resolveProvider)
				? (codeLens: VCodeLens, token: CancellationToken): Thenable<VCodeLens> => {
					return this.sendRequest(CodeLensResolveRequest.type, this._c2p.asCodeLens(codeLens), token).then(
						this._p2c.asCodeLens,
						(error) => {
							this.logFailedRequest(CodeLensResolveRequest.type, error);
							return codeLens;
						}
					);
				}
				: undefined
		});
	}

	private createDocumentFormattingProvider(options: TextDocumentRegistrationOptions): Disposable {
		return Languages.registerDocumentFormattingEditProvider(options.documentSelector!, {
			provideDocumentFormattingEdits: (document: TextDocument, options: VFormattingOptions, token: CancellationToken): Thenable<VTextEdit[]> => {
				let params: DocumentFormattingParams = {
					textDocument: this._c2p.asTextDocumentIdentifier(document),
					options: this._c2p.asFormattingOptions(options)
				};
				return this.sendRequest(DocumentFormattingRequest.type, params, token).then(
					this._p2c.asTextEdits,
					(error) => {
						this.logFailedRequest(DocumentFormattingRequest.type, error);
						return Promise.resolve([]);
					}
				);
			}
		});
	}

	private createDocumentRangeFormattingProvider(options: TextDocumentRegistrationOptions): Disposable {
		return Languages.registerDocumentRangeFormattingEditProvider(options.documentSelector!, {
			provideDocumentRangeFormattingEdits: (document: TextDocument, range: VRange, options: VFormattingOptions, token: CancellationToken): Thenable<VTextEdit[]> => {
				let params: DocumentRangeFormattingParams = {
					textDocument: this._c2p.asTextDocumentIdentifier(document),
					range: this._c2p.asRange(range),
					options: this._c2p.asFormattingOptions(options)
				};
				return this.sendRequest(DocumentRangeFormattingRequest.type, params, token).then(
					this._p2c.asTextEdits,
					(error) => {
						this.logFailedRequest(DocumentRangeFormattingRequest.type, error);
						return Promise.resolve([]);
					}
				);
			}
		});
	}

	private createDocumentOnTypeFormattingProvider(options: DocumentOnTypeFormattingRegistrationOptions): Disposable {
		let moreTriggerCharacter = options.moreTriggerCharacter || [];
		return Languages.registerOnTypeFormattingEditProvider(options.documentSelector!, {
			provideOnTypeFormattingEdits: (document: TextDocument, position: VPosition, ch: string, options: VFormattingOptions, token: CancellationToken): Thenable<VTextEdit[]> => {
				let params: DocumentOnTypeFormattingParams = {
					textDocument: this._c2p.asTextDocumentIdentifier(document),
					position: this._c2p.asPosition(position),
					ch: ch,
					options: this._c2p.asFormattingOptions(options)
				};
				return this.sendRequest(DocumentOnTypeFormattingRequest.type, params, token).then(
					this._p2c.asTextEdits,
					(error) => {
						this.logFailedRequest(DocumentOnTypeFormattingRequest.type, error);
						return Promise.resolve([]);
					}
				);
			}
		}, options.firstTriggerCharacter, ...moreTriggerCharacter);
	}

	private createRenameProvider(options: TextDocumentRegistrationOptions): Disposable {
		return Languages.registerRenameProvider(options.documentSelector!, {
			provideRenameEdits: (document: TextDocument, position: VPosition, newName: string, token: CancellationToken): Thenable<VWorkspaceEdit> => {
				let params: RenameParams = {
					textDocument: this._c2p.asTextDocumentIdentifier(document),
					position: this._c2p.asPosition(position),
					newName: newName
				};
				return this.sendRequest(RenameRequest.type, params, token).then(
					this._p2c.asWorkspaceEdit,
					(error: ResponseError<void>) => {
						this.logFailedRequest(RenameRequest.type, error);
						Promise.resolve(new Error(error.message));
					}
				)
			}
		});
	}

	private createDocumentLinkProvider(options: DocumentLinkRegistrationOptions): Disposable {
		return Languages.registerDocumentLinkProvider(options.documentSelector!, {
			provideDocumentLinks: (document: TextDocument, token: CancellationToken): Thenable<VDocumentLink[]> => {
				return this.sendRequest(DocumentLinkRequest.type, this._c2p.asDocumentLinkParams(document), token).then(
					this._p2c.asDocumentLinks,
					(error: ResponseError<void>) => {
						this.logFailedRequest(DocumentLinkRequest.type, error);
						Promise.resolve(new Error(error.message));
					}
				)
			},
			resolveDocumentLink: options.resolveProvider
				? (link: VDocumentLink, token: CancellationToken): Thenable<VDocumentLink> => {
					return this.sendRequest(DocumentLinkResolveRequest.type, this._c2p.asDocumentLink(link), token).then(
						this._p2c.asDocumentLink,
						(error: ResponseError<void>) => {
							this.logFailedRequest(DocumentLinkResolveRequest.type, error);
							Promise.resolve(new Error(error.message));
						}
					)
				}
				: undefined
		});
	}
}

export class SettingMonitor {

	private _listeners: Disposable[];

	constructor(private _client: LanguageClient, private _setting: string) {
		this._listeners = [];
	}

	public start(): Disposable {
		Workspace.onDidChangeConfiguration(this.onDidChangeConfiguration, this, this._listeners);
		this.onDidChangeConfiguration();
		return new Disposable(() => {
			if (this._client.needsStop()) {
				this._client.stop();
			}
		});
	}

	private onDidChangeConfiguration(): void {
		let index = this._setting.indexOf('.');
		let primary = index >= 0 ? this._setting.substr(0, index) : this._setting;
		let rest = index >= 0 ? this._setting.substr(index + 1) : undefined;
		let enabled = rest ? Workspace.getConfiguration(primary).get(rest, false) : Workspace.getConfiguration(primary);
		if (enabled && this._client.needsStart()) {
			this._client.start();
		} else if (!enabled && this._client.needsStop()) {
			this._client.stop();
		}
	}
}