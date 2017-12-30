;;; cquery.el --- cquery client for lsp-mode     -*- lexical-binding: t; -*-

;; Copyright (C) 2017 Tobias Pisani

;; Author:  Tobias Pisani
;; Package-Version: 20171122.1
;; Version: 0.1
;; Homepage: https://github.com/jacobdufault/cquery
;; Package-Requires: ((emacs "25") (lsp-mode "3.0"))
;; Keywords: languages, lsp-mode, c++

;; Permission is hereby granted, free of charge, to any person obtaining a copy
;; of this software and associated documentation files (the "Software"), to deal
;; in the Software without restriction, including without limitation the rights
;; to use, copy, modify, merge, publish, distribute, sublicense, and-or sell
;; copies of the Software, and to permit persons to whom the Software is
;; furnished to do so, subject to the following conditions:

;; The above copyright notice and this permission notice shall be included in
;; all copies or substantial portions of the Software.

;; THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
;; IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
;; FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
;; AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
;; LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
;; OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
;; SOFTWARE.
;;
;;; Commentary:

;;
;; To enable, call (lsp-cquery-enable) in your c++-mode hook.
;;
;;  TODO:
;;
;;  - Rainbow variables with semantic highlighting
;;  - Better config options
;;

;;; Code:

(require 'cc-mode)
(require 'lsp-mode)
(require 'cl-lib)
(require 'subr-x)

;; ---------------------------------------------------------------------
;;   Customization
;; ---------------------------------------------------------------------

(defgroup cquery nil
  "Customization options for the cquery client"
  :group 'tools)

(defcustom cquery-executable
  "cquery"
  "cquery executable filename"
  :type 'file
  :group 'cquery)

(defcustom cquery-resource-dir
  nil
  "The clang resource directory."
  :type '(choice
          (const :tag "Use default resource directory" :value nil)
          (directory :tag "Custom resource directory"))
  :group 'cquery)

(defcustom cquery-indexer-count
  0
  "Number of workers cquery will use to index each project.
 When left at 0, cquery will computer this value automatically."
  :type 'number
  :group 'cquery)

(defcustom cquery-additional-arguments
  nil
  "Additional arguments passed to cquery."
  :type 'list
  :group 'cquery)

(defface cquery-inactive-region-face
  '((t :foreground "#666666"))
  "The face used to mark inactive regions"
  :group 'cquery)

(defface cquery-sem-type-face
  '((t :weight bold :inherit font-lock-type-face))
  "The face used to mark types"
  :group 'cquery)

(defface cquery-sem-member-func-face
  '((t :slant italic :inherit font-lock-function-name-face))
  "The face used to mark member functions"
  :group 'cquery)

(defface cquery-sem-free-func-face
  '((t :inherit font-lock-function-name-face))
  "The face used to mark free functions"
  :group 'cquery)

(defface cquery-sem-member-var-face
  '((t :slant italic :inherit font-lock-variable-name-face))
  "The face used to mark member variables"
  :group 'cquery)

(defface cquery-sem-free-var-face
  '((t :inherit font-lock-variable-name-face))
  "The face used to mark local and namespace scope variables"
  :group 'cquery)

(defface cquery-code-lens-face
  '((t :foreground "#777777"))
  "The face used for code lens overlays"
  :group 'cquery)

(defface cquery-code-lens-mouse-face
  '((t :box t))
  "The face used for code lens overlays"
  :group 'cquery)

(defcustom cquery-enable-sem-highlight
  t
  "Enable semantic highlighting"
  :type 'boolean
  :group 'cquery)

(defcustom cquery-sem-highlight-method
  'overlay
  "The method used to draw semantic highlighting. overlays are more
 accurate than font-lock, but slower."
  :group 'lsp-mode
  :type 'symbol
  :options '(overlay font-lock))

(defcustom cquery-cache-dir
  ".vscode/cquery_cached_index/"
  "Directory in which cquery will store its index cache. Relative
 to the project root directory."
  :type 'string
  :group 'cquery)

;; ---------------------------------------------------------------------
;;   Semantic highlighting
;; ---------------------------------------------------------------------

(defun cquery--clear-sem-highlights ()
  (pcase cquery-sem-highlight-method
    ('overlay
     (dolist (ov (overlays-in (point-min) (point-max)))
       (when (overlay-get ov 'cquery-sem-highlight)
         (delete-overlay ov))))
    ('font-lock
     (font-lock-ensure))))

(defun cquery--make-sem-highlight (region buffer face)
  (pcase cquery-sem-highlight-method
    ('overlay
     (let ((ov (make-overlay (car region) (cdr region) buffer)))
       (overlay-put ov 'face face)
       (overlay-put ov 'cquery-sem-highlight t)))
    ('font-lock
     (put-text-property (car region) (cdr region) 'font-lock-face face buffer))))

(defun cquery--publish-semantic-highlighting (_workspace params)
  (when cquery-enable-sem-highlight
    (let* ((file (cquery--uri-to-file (gethash "uri" params)))
           (buffer (find-buffer-visiting file))
           (symbols (gethash "symbols" params)))
      (when buffer
        (with-current-buffer buffer
          (save-excursion
           (with-silent-modifications
             (cquery--clear-sem-highlights)
             (dolist (symbol symbols)
               (let* ((type (gethash "type" symbol))
                      (is-type-member (gethash "isTypeMember" symbol))
                      (ranges (mapcar 'cquery--read-range (gethash "ranges" symbol)))
                      (face
                       (pcase type
                         ('0 'cquery-sem-type-face)
                         ('1 (if is-type-member 'cquery-sem-member-func-face 'cquery-sem-free-func-face))
                         ('2 (if is-type-member 'cquery-sem-member-var-face 'cquery-sem-free-var-face)))))
                 (when face
                   (dolist (range ranges)
                     (cquery--make-sem-highlight range buffer face))))))))))))

;; ---------------------------------------------------------------------
;;   Inactive regions
;; ---------------------------------------------------------------------

(defun cquery--clear-inactive-regions ()
  (dolist (ov (overlays-in (point-min) (point-max)))
    (when (overlay-get ov 'cquery-inactive)
      (delete-overlay ov))))

(defun cquery--set-inactive-regions (_workspace params)
  (let* ((file (cquery--uri-to-file (gethash "uri" params)))
         (regions (mapcar 'cquery--read-range (gethash "inactiveRegions" params)))
         (buffer (find-buffer-visiting file)))
    (when buffer
      (with-current-buffer buffer
        (save-excursion
          (cquery--clear-inactive-regions)
          (overlay-recenter (point-max))
          (dolist (region regions)
            (let ((ov (make-overlay (car region) (cdr region) buffer)))
              (overlay-put ov 'face 'cquery-inactive-region-face)
              (overlay-put ov 'cquery-inactive t))))))))

;; ---------------------------------------------------------------------
;;   Notification handlers
;; ---------------------------------------------------------------------

(defconst cquery--handlers
  '(("$cquery/setInactiveRegions" . (lambda (w p) (cquery--set-inactive-regions w p)))
    ("$cquery/publishSemanticHighlighting" . (lambda (w p) (cquery--publish-semantic-highlighting w p)))
    ("$cquery/progress" . (lambda (_w _p)))))

;; ---------------------------------------------------------------------
;;   Other cquery-specific methods
;; ---------------------------------------------------------------------

(defun cquery-xref-find-locations-with-position (method &optional display-action)
  "Find cquery-specific cross references.
Choices of METHOD include \"$cquery/base\", \"$cquery/callers\",
\"$cquery/derived\", \"$cquery/vars\".
Read document for all choices."
  (lsp--cur-workspace-check)
  (let ((xrefs (lsp--locations-to-xref-items
                (lsp--send-request
                 (lsp--make-request method
                                    (lsp--text-document-position-params))))))
    (unless xrefs
      (user-error "No %s found" method))
    (xref--show-xrefs xrefs display-action)))

;; ---------------------------------------------------------------------
;;   Codelens
;;
;;   Enable by calling `cquery-request-code-lens'
;;   Clear them away using `cquery-clear-code-lens'
;;
;;   TODO:
;;   - Find a better way to display them.
;;
;;   - Instead of adding multiple lenses to one symbol, append the text
;;     of the new one to the old. This will fix flickering when moving
;;     over lenses.
;;
;;   - Add a global option to request code lenses on automatically
;; ---------------------------------------------------------------------

(defun cquery-request-code-lens ()
  "Request code lens from cquery"
  (interactive)
  (lsp--cur-workspace-check)
  (lsp--send-request-async
   (lsp--make-request "textDocument/codeLens"
                      `(:textDocument (:uri ,(concat lsp--uri-file-prefix buffer-file-name))))
   'cquery--code-lens-callback))

(defun cquery-clear-code-lens ()
  "Clear all code lenses from this buffer"
  (interactive)
  (dolist (ov (overlays-in (point-min) (point-max)))
    (when (overlay-get ov 'cquery-code-lens)
      (delete-overlay ov))))

(define-minor-mode cquery-code-lens-mode
  "toggle code-lens overlays"
  :group 'cquery
  :global nil
  :init-value nil
  :lighter "Lens"
  (if cquery-code-lens-mode
      (cquery-request-code-lens)
    (cquery-clear-code-lens)))

(defun cquery--make-code-lens-string (command)
  (let ((map (make-sparse-keymap)))
    (define-key map [mouse-1] (lambda () (interactive) (cquery-execute-command command)))
    (propertize (gethash "title" command)
                'face 'cquery-code-lens-face
                'mouse-face 'cquery-code-lens-mouse-face
                'local-map map)))

(defun cquery--code-lens-callback (result)
  (overlay-recenter (point-max))
  (cquery-clear-code-lens)
  (let (buffers)
    (dolist (lens result)
      (let* ((range (cquery--read-range (gethash "range" lens)))
             (root (gethash "command" lens))
             (title (gethash "title" root))
             (command (gethash "command" root))
             (buffer (find-buffer-visiting (cquery--uri-to-file (car (gethash "arguments" root))))))
        (when buffer
          (with-current-buffer buffer
            (save-excursion
              (when (not (member buffer buffers))
                (cquery-clear-code-lens)
                (overlay-recenter (point-max))
                (setq buffers (cons buffer buffers)))
              (let ((ov (make-overlay (car range) (cdr range) buffer)))
                (overlay-put ov 'cquery-code-lens t)
                (overlay-put ov 'after-string (format " %s " (cquery--make-code-lens-string root)))))))))))

;; ---------------------------------------------------------------------
;;   CodeAction Commands
;; ---------------------------------------------------------------------

(defun cquery-select-codeaction ()
  "Show a list of codeactions using ivy, and pick one to apply"
  (interactive)
  (let ((name-func
         (lambda (action)
           (let ((edit (caadr (gethash "arguments" action))))
             (format "%s: %s" (lsp--position-to-point
                               (gethash "start" (gethash "range" edit)))
                     (gethash "title" action))))))
    (if (null lsp-code-actions)
        (message "No code actions avaliable")
      (ivy-read "Apply CodeAction: "
                (mapcar (lambda (action)
                          (funcall name-func action))
                        lsp-code-actions)
                :action (lambda (str)
                          (dolist (action lsp-code-actions)
                            (when (equal (funcall name-func action) str)
                              (cquery-execute-command action)
                              (lsp--text-document-code-action))))))))

(defun cquery-execute-command (action)
  "Execute a cquery command"
  (let* ((command (gethash "command" action))
         (arguments (gethash "arguments" action))
         (uri (car arguments))
         (data (cdr arguments)))
    (save-current-buffer
      (find-file (cquery--uri-to-file uri))
      (pcase command
        ;; Code actions
        ('"cquery._applyFixIt"
         (dolist (edit data)
           (cquery--apply-textedit (car edit))))
        ('"cquery._autoImplement"
         (dolist (edit data)
           (cquery--apply-textedit (car edit)))
         (goto-char (lsp--position-to-point
                     (gethash "start" (gethash "range" (caar data))))))
        ('"cquery._insertInclude"
         (cquery--select-textedit data "Include: "))
        ('"cquery.showReferences" ;; Used by code lenses
         (let ((pos (lsp--position-to-point (car data))))
           (goto-char pos)
           (xref-find-references (xref-backend-identifier-at-point (xref-find-backend)))))))))

(defun cquery--select-textedit (edit-list prompt)
  "Show a list of possible textedits, and apply the selected.
  Used by cquery._insertInclude"
  (let ((name-func (lambda (edit)
                     (concat (lsp--position-to-point
                              (gethash "start" (gethash "range" edit)))
                             ": "
                             (gethash "newText" edit)))))
    (ivy-read prompt
              (mapcar (lambda (edit)
                        (funcall name-func edit))
                      edit-list)
              :require-match t
              :action (lambda (str)
                        (cl-loop
                         for edit in edit-list
                         do (when (equal (funcall name-func edit) str)
                              (cquery--apply-textedit edit)))))))

(defun cquery--apply-textedit (edit)
  (let* ((range (gethash "range" edit))
         (start (lsp--position-to-point (gethash "start" range)))
         (end (lsp--position-to-point (gethash "end" range)))
         (newText (gethash "newText" edit)))
    (when (> end start)
      (delete-region start (- end 1)))
    (goto-char start)
    (insert newText)))

(defun cquery--uri-to-file (uri)
  (string-remove-prefix lsp--uri-file-prefix uri))

(defun cquery--read-range (range)
  (cons (lsp--position-to-point (gethash "start" range))
        (lsp--position-to-point (gethash "end" range))))

;; ---------------------------------------------------------------------
;;  Register lsp client
;; ---------------------------------------------------------------------

(defun cquery--make-renderer (mode)
  `(lambda (str)
     (with-temp-buffer
       (delay-mode-hooks (,(intern (format "%s-mode" mode))))
       (insert str)
       (font-lock-ensure)
       (buffer-string))))

(defun cquery--initialize-client (client)
  (dolist (p cquery--handlers)
    (lsp-client-on-notification client (car p) (cdr p)))
  (lsp-provide-marked-string-renderer client "c" (cquery--make-renderer "c"))
  (lsp-provide-marked-string-renderer client "cpp" (cquery--make-renderer "c++"))
  (lsp-provide-marked-string-renderer client "objectivec" (cquery--make-renderer "objc")))

(defun cquery--get-init-params (workspace)
  (let ((json-false :json-false))
    `(:cacheDirectory ,(file-name-as-directory
                        (expand-file-name cquery-cache-dir (lsp--workspace-root workspace)))
                      ,@(when cquery-resource-dir
                          `(:resourceDirectory ,(expand-file-name cquery-resource-dir)))
                      :indexerCount ,cquery-indexer-count
                      :enableProgressReports ,json-false))) ; TODO: prog reports for modeline

(defun cquery--get-root ()
  "Return the root directory of a cquery project."
  (expand-file-name (or (locate-dominating-file default-directory "compile_commands.json")
                        (locate-dominating-file default-directory ".cquery")
                        (user-error "Could not find cquery project root"))))

(lsp-define-stdio-client
 lsp-cquery "cpp" #'cquery--get-root
 `(,cquery-executable "--language-server" ,@cquery-additional-arguments)
 :initialize #'cquery--initialize-client
 :extra-init-params #'cquery--get-init-params)

(provide 'cquery)
;;; cquery.el ends here
