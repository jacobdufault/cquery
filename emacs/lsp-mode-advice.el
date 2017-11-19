;;; lsp-mode-advice.el --- GPL Licensed advice for lsp-cquery  -*- lexical-binding: t; -*-

;; Copyright (C) 2017  Vibhav Pant <vibhavp@gmail.com>

;; This program is free software; you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation, either version 3 of the License, or
;; (at your option) any later version.

;; This program is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.

;; You should have received a copy of the GNU General Public License
;; along with this program.  If not, see <https://www.gnu.org/licenses/>.

;;; Commentary:

;; At some point this should be possible using lsp-mode directly. For now we use
;; this hack. Nearly all code is directly from lsp-mode.

;;; Code:

(defun cquery//lsp--start (_orig-func client)
  (when lsp--cur-workspace
    (user-error "LSP mode is already enabled for this buffer"))
  (let* ((root (funcall (lsp--client-get-root client)))
         (workspace (gethash root lsp--workspaces))
         new-conn response init-params
         parser proc cmd-proc)
    (if workspace
        (setq lsp--cur-workspace workspace)

      (setf
       parser (make-lsp--parser)
       lsp--cur-workspace (make-lsp--workspace
                           :parser parser
                           :file-versions (make-hash-table :test 'equal)
                           :root root
                           :client client)
       (lsp--parser-workspace parser) lsp--cur-workspace
       new-conn (funcall
                 (lsp--client-new-connection client)
                 (lsp--parser-make-filter parser (lsp--client-ignore-regexps client))
                 (lsp--make-sentinel (current-buffer)))
       ;; the command line process invoked
       cmd-proc (if (consp new-conn) (car new-conn) new-conn)
       ;; the process we actually communicate with
       proc (if (consp new-conn) (cdr new-conn) new-conn)

       (lsp--workspace-proc lsp--cur-workspace) proc
       (lsp--workspace-cmd-proc lsp--cur-workspace) cmd-proc)

      (puthash root lsp--cur-workspace lsp--workspaces)
      (run-hooks 'lsp-before-initialize-hook)
      (setq init-params `(
                          :processId ,(emacs-pid) :rootPath ,root
                          :rootUri ,(concat "file://" root)
                          :initializationOptions ,`(
                                                    :cacheDirectory ,(concat root ".vscode/cquery_cached_index")
                                                    :resourceDirectory ,(concat cquery/root-dir "clang_resource_dir"))
                          :capabilities ,(lsp--client-capabilities)))
      (setf response (lsp--send-request (lsp--make-request "initialize" init-params)))
      (unless response
        (signal 'lsp-empty-response-error (list "initialize")))
      (setf (lsp--workspace-server-capabilities lsp--cur-workspace)
            (gethash "capabilities" response))
      ;; Version 3.0 now sends an "initialized" notification to allow registration
      ;; of server capabilities
      (lsp--send-notification (lsp--make-notification "initialized" nil))
      (run-hooks 'lsp-after-initialize-hook))
    (lsp--text-document-did-open)))

(advice-add 'lsp--start :around 'cquery//lsp--start)

(provide 'lsp-mode-advice)
;;; lsp-mode-advice.el ends here
