" Vim global plugin for semantic highlighting using libclang
" Maintainer:	Jeaye <contact@jeaye.com>

" ------------------------------------------------------------------------------
if !has("nvim")
  echohl WarningMsg |
        \ echomsg "color_coded unavailable: only neovim is supported" |
        \ echohl None
  finish
endif

if exists("g:loaded_color_coded") || &cp
  finish
endif

if !exists("g:color_coded_enabled")
  let g:color_coded_enabled = 1
elseif g:color_coded_enabled == 0
  finish
endif
" ------------------------------------------------------------------------------

let g:loaded_color_coded = 1
let s:root_dir = expand("<sfile>:p:h:h")
let s:keepcpo = &cpo
set cpo&vim

" ------------------------------------------------------------------------------
let s:color_coded_valid = color_coded#setup()
if s:color_coded_valid == 1
  command! CCLog call color_coded#open_log()
  command! CCToggle call color_coded#toggle()

  augroup color_coded
    " TODO: Update path.
    au VimEnter,ColorScheme * exe "source " . s:root_dir . "/../after/syntax/color_coded.vim"
    au BufEnter * call color_coded#enter_buffer(expand("<afile>"))
    au WinEnter * call color_coded#enter_buffer(expand("<afile>"))
    " We only recompile when in normal mode, since recompiling for every key
    " press in insert mode causes way too much churn and can cause flickering.
    au TextChanged,InsertLeave * call color_coded#recompile(expand("<afile>"))

    au CursorMoved,CursorMovedI * call color_coded#move(expand("<afile>"))
    au CursorHold,CursorHoldI * call color_coded#move(expand("<afile>"))

    " TODO: Maybe restore this.
    " Leaving a color_coded buffer requires removing matched positions.
    "au BufLeave * call color_coded#clear_matches(color_coded#get_buffer_name())
  augroup END
endif

" ------------------------------------------------------------------------------
let &cpo = s:keepcpo
unlet s:keepcpo
