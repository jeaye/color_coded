" Vim global plugin for semantic highlighting using libclang
" Last Change: Sept. 2014
" Maintainer:	Jeaye <contact@jeaye.com>

" ------------------------------------------------------------------------------
if v:version < 704 || !exists("*matchaddpos")
  echohl WarningMsg |
        \ echomsg "color_coded unavailable: requires Vim 7.4p330+" |
        \ echohl None
  finish
endif
if !has('ruby')
  echohl WarningMsg |
        \ echomsg "color_coded unavailable: requires ruby" |
        \ echohl None
  finish
endif
if exists("g:loaded_color_coded") || &cp
  finish
endif
" ------------------------------------------------------------------------------

let g:loaded_color_coded = 1
let s:keepcpo = &cpo
set cpo&vim
" ------------------------------------------------------------------------------

ruby require './color_coded.so'
set ft=cpp

function! s:try_append()
ruby << EOF
  cc_append("|")
EOF
endfunction!

function! s:try_work()
ruby << EOF
  cc_work(VIM::Buffer.current.line)
EOF
endfunction!

"au CursorMoved * call s:try_append()
au TextChanged * call s:try_work()
au TextChangedI * call s:try_work()
au CursorMoved * call s:try_work()

" ------------------------------------------------------------------------------
let &cpo = s:keepcpo
unlet s:keepcpo
