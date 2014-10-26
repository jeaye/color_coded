" Vim global plugin for semantic highlighting using libclang
" Maintainer:	Jeaye <contact@jeaye.com>

" ------------------------------------------------------------------------------
if v:version < 704 || !exists("*matchaddpos")
  echohl WarningMsg |
        \ echomsg "color_coded unavailable: requires Vim 7.4p330+" |
        \ echohl None
  finish
endif
if !has('lua')
  echohl WarningMsg |
        \ echomsg "color_coded unavailable: requires lua" |
        \ echohl None
  finish
endif
if exists("g:loaded_color_coded") || &cp
  finish
endif
" ------------------------------------------------------------------------------

let g:loaded_color_coded = 1
let $VIMHOME = expand('<sfile>:p:h:h')
let s:keepcpo = &cpo
set cpo&vim
" ------------------------------------------------------------------------------

" Only continue if the setup went well
let s:color_coded_valid = color_coded#setup()
if s:color_coded_valid == 1
  command! CCerror call color_coded#last_error()

  augroup color_coded
    au VimEnter,ColorScheme * source $VIMHOME/after/syntax/color_coded.vim
    au VimEnter,BufEnter * call color_coded#enter()
    au TextChanged,TextChangedI * call color_coded#push()
    au CursorMoved,CursorMovedI * call color_coded#moved()
    au CursorHold,CursorHoldI * call color_coded#pull()
    au BufLeave * call clearmatches()
    au BufDelete * call color_coded#destroy(expand('<afile>'))
  augroup END
endif

" ------------------------------------------------------------------------------
let &cpo = s:keepcpo
unlet s:keepcpo
