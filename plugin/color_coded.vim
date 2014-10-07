" Vim global plugin for semantic highlighting using libclang
" Maintainer:	Jeaye <contact@jeaye.com>

" ------------------------------------------------------------------------------
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

if !exists("g:color_coded_filetypes")
  let g:color_coded_filetypes = ['c', 'h', 'cpp', 'hpp', 'cc', 'm', 'mm']
endif

let s:path = expand('<sfile>:p:h')
ruby << EOF
  require VIM::evaluate('s:path') + '/../bin/color_coded.so'

  def color_coded_buffer_details()
    name = VIM::Buffer.current.name
    name = VIM::Buffer.current.number.to_s if name.nil?

    line_count = VIM::Buffer.current.count
    data = ''
    for i in 1..line_count do
      data += VIM::Buffer.current[i] + "\n"
    end
    return name, data.nil? ? "" : data
  end
EOF

function! s:color_coded_push()
  if index(g:color_coded_filetypes, &ft) < 0
    return
  endif
ruby << EOF
  name, data = color_coded_buffer_details
  color_coded_push(name, data)
EOF
endfunction!

function! s:color_coded_pull()
  if index(g:color_coded_filetypes, &ft) < 0
    return
  endif
ruby << EOF
  color_coded_pull
EOF
endfunction!

function! s:color_coded_enter()
  if index(g:color_coded_filetypes, &ft) < 0
    return
  endif
ruby << EOF
  name, data = color_coded_buffer_details
  color_coded_enter(name, data)
EOF
endfunction!

let $VIMHOME=expand('<sfile>:p:h:h')
augroup color_coded
  au BufNewFile,BufRead * source $VIMHOME/after/syntax/color_coded.vim
  au BufEnter * call s:color_coded_enter()
  au VimEnter * call s:color_coded_enter()
  au TextChanged * call s:color_coded_push()
  au TextChangedI * call s:color_coded_push()
  au CursorMoved * call s:color_coded_pull()
  au CursorMovedI * call s:color_coded_pull()
  au CursorHold * call s:color_coded_pull()
  au CursorHoldI * call s:color_coded_pull()
augroup END

" ------------------------------------------------------------------------------
let &cpo = s:keepcpo
unlet s:keepcpo
