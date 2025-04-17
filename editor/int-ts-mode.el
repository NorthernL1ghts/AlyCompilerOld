;;; int-ts-mode.el --- Syntax highlighting like int-mode but better because tree-sitter -*- lexical-binding: t -*-

;; Author: Rylan Lens Kellogg
;; Maintainer: Rylan Lens Kellogg
;; Version: 0.0.1
;; Keywords   : Intercept languages tree-sitter


;; This file is not part of GNU Emacs

;; This program is free software: you can redistribute it and/or modify
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

;; commentary

;;; Code:

(require 'treesit)

(declare-function treesit-parser-create "treesit.c")
;(declare-function treesit-induce-sparse-tree "treesit.c")
;(declare-function treesit-node-parent "treesit.c")
;(declare-function treesit-node-start "treesit.c")
;(declare-function treesit-node-end "treesit.c")
;(declare-function treesit-node-child "treesit.c")
;(declare-function treesit-node-child-by-field-name "treesit.c")
;(declare-function treesit-node-type "treesit.c")


(defvar int-ts-mode--syntax-table
  (make-syntax-table)
  "Syntax table for int-ts-mode")

;; Set two semi-colons as comment starting characters.
(modify-syntax-entry
 ?\; ". 12"
 int-ts-mode--syntax-table)
;; Set newline as comment ending character.
(modify-syntax-entry
 ?\n ">"
 int-ts-mode--syntax-table)

;; Delimiters include comma
(modify-syntax-entry
 ?, "."
 int-ts-mode--syntax-table)

;; Allowed within symbols: _
(modify-syntax-entry
 ?_ "w"
 int-ts-mode--syntax-table)
(modify-syntax-entry
 ?\\ "w"
 int-ts-mode--syntax-table)

;; Parenthesis
(modify-syntax-entry
 ?\( "()"
 int-ts-mode--syntax-table)
(modify-syntax-entry
 ?\) ")("
 int-ts-mode--syntax-table)
(modify-syntax-entry
 ?\[ "(]"
 int-ts-mode--syntax-table)
(modify-syntax-entry
 ?\] ")["
 int-ts-mode--syntax-table)
(modify-syntax-entry
 ?{ "(}"
 int-ts-mode--syntax-table)
(modify-syntax-entry
 ?} "){"
 int-ts-mode--syntax-table)

;; Unary prefix operators
;; Addressof
(modify-syntax-entry
 ?& "'"
 int-ts-mode--syntax-table)
;; Dereference
(modify-syntax-entry
 ?@ "'"
 int-ts-mode--syntax-table)

(defvar int-ts-mode--keywords
  '("if" "else" "ext" "while" "for" "type" "macro" "emits" "endmacro" ;; keywords
    "discardable" "nomangle" ;; function attributes
    "alignas" ;; type attributes
    "module" "import" "export" ;; not really keywords, but that's fine
    )
  "Intercept keywords for tree-sitter font-locking.")

(defvar int-ts-mode--operators
  '("+" "-" "*" "/" "%"
    "<<" ">>" "&" "|" "^" "~"
    "<=" ">=" "!=" "=" "<" ">" "!"
    ":" ":=" "::" "@" "as"
    )
  "Intercept operators for tree-sitter font-locking.")

(defvar int-ts-mode--delimiters
  '("(" ")"
    "[" "]"
    "{" "}"
    ";")
  "Intercept delimiters for tree-sitter font-locking.")

(defun int-ts-mode--font-lock-settings ()
  "Tree-sitter font-lock settings."
  (treesit-font-lock-rules
   :language 'int
   :override t
   :feature 'comment
   `((comment) @font-lock-comment-face)

   :language 'int
   :feature 'variable
   `((identifier) @font-lock-variable-name-face
     (macro_arg) @font-lock-property-face)

   :language 'int
   :override t
   :feature 'function
   `((expr_decl name: (identifier) @font-lock-function-name-face
                type: (type_function))
     (expr_decl name: (identifier) @font-lock-function-name-face
                type: (type_pointer (type_function)))
     (expr_call callee: (identifier) @font-lock-function-name-face))

   :language 'int
   :override t
   :feature 'type
   `((type_base)          @font-lock-type-face
     (type_pointer)       @font-lock-type-face
     (type_reference)     @font-lock-type-face
     (type_arbitrary_int) @font-lock-type-face
     (type_function)      @font-lock-type-face
     (type_array)         @font-lock-type-face)

   :language 'int
   :override t
   :feature 'number
   `((number) @font-lock-number-face)

   :language 'int
   :override t
   :feature 'string
   `((string) @font-lock-string-face)

   :language 'int
   :override t
   :feature 'keyword
   `([,@int-ts-mode--keywords] @font-lock-keyword-face)

   :language 'int
   :feature 'operator
   `([,@int-ts-mode--operators] @font-lock-operator-face)

   :language 'int
   :feature 'delimiter
   `([,@int-ts-mode--delimiters] @font-lock-delimiter-face)
   ))

(defcustom int-ts-mode-indent-amount 2
  "The amount of space characters that each level of parenthesis nesting
in Intercept source code will be indented."
  :group 'int-ts-mode)

(defun int-ts--indent-line ()
  "Indent a line in Intercept."
  (let ((indent)
        (boi-predicate)
        (should-move-eol)
        (point (point)))
    (save-excursion
      ;; Move 'point' backwards to end of indentation, if any.
      (back-to-indentation)
      ;; Use the partial parsing syntax state or whatever to get
      ;; parenthesis nesting level. This value is calculated using
      ;; data from the syntax table up above.
      (setq indent (car (syntax-ppss))
            boi-predicate (= (point) point))
      (if boi-predicate
          ;; If we began at the beginning of the line (after indentation),
          ;; we need to move point to after inserted indentation.
          (setq should-move-eol t)
        ;; If not at beginning of indentation but at a newline, do not
        ;; indent anything at all. FIXME: Is this misleading?
        (when (eq (char-after) ?\n)
          (setq indent 0)))
      ;; Closing parenthesis characters are indented at parent level.
      (when (or
             (eq (char-after) ?\))
             (eq (char-after) ?\}))
        (setq indent (1- indent)))
      ;; Get rid of existing indentation, if any.
      (delete-region (line-beginning-position) (point))
      ;; Indent to proper amount based on customizable value.
      (indent-to (* indent int-ts-mode-indent-amount)))
    (when should-move-eol
      (move-end-of-line nil))))

;;;###autoload
(define-derived-mode int-ts-mode--base-mode prog-mode "int"
  "Major mode for editing Intercept, powered by tree-sitter. "
  :syntax-table int-ts-mode--syntax-table

  ;; Electric
  (setq-local electric-indent-chars
              (append "{}():," electric-indent-chars))

  (setq-local treesit-font-lock-feature-list
              '(( function variable comment string )
                ( keyword type )
                ( number operator delimiter )
                ( ))))

;;;###autoload
(define-derived-mode int-ts-mode int-ts-mode--base-mode "int"
  "Major mode for editing Intercept, powered by tree-sitter."
  :group 'int

  (unless (treesit-ready-p 'int)`
    (error "Tree-sitter for C isn't available"))

  (treesit-parser-create 'int)

  ;; Comments.
  (setq-local comment-start ";;")
  (setq-local comment-end   "")

  ;; TODO: Maybe actually write a tree sitter indent function.
  ;; Use `C-h f treesit-simple-indent RET`
  (setq-local treesit-simple-indent-rules nil)

  ;; Syntax table indent
  (setq indent-line-function #'int-ts--indent-line)

  ;; Font-lock.
  (setq-local treesit-font-lock-settings
              (int-ts-mode--font-lock-settings))

  (add-to-list 'auto-mode-alist '("\\.int\\'" . int-ts-mode))

  (treesit-major-mode-setup))


(provide 'int-ts-mode)

;;; int-ts-mode.el ends here