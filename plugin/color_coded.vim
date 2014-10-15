" Vim global plugin for semantic highlighting using libclang
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

if !exists("g:color_coded_filetypes")
  let g:color_coded_filetypes = ['c', 'h', 'cpp', 'hpp', 'cc', 'm', 'mm']
endif

let s:path = expand('<sfile>:p:h')
ruby << EOF
  require VIM::evaluate('s:path') + '/../bin/color_coded.so'

  def color_coded_buffer_name
    name = VIM::Buffer.current.name
    name = VIM::Buffer.current.number.to_s if name.nil?
    return name
  end

  def color_coded_buffer_details
    name = color_coded_buffer_name

    line_count = VIM::Buffer.current.count
    data = ''
    for i in 1..line_count do
      data << VIM::Buffer.current[i] + "\n"
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
  name = color_coded_buffer_name
  color_coded_pull(name)
EOF
endfunction!

function! s:color_coded_moved()
  if index(g:color_coded_filetypes, &ft) < 0
    return
  endif
ruby << EOF
  name = color_coded_buffer_name
  color_coded_moved(name,
                    VIM::Window.current.cursor[0],
                    VIM::Buffer.current.count,
                    VIM::Window.current.height)
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

function! s:color_coded_destroy(file)
  if index(g:color_coded_filetypes, &ft) < 0
    return
  endif
  let s:file = a:file
ruby << EOF
  name = VIM::evaluate('s:file')
  color_coded_destroy(name)
EOF
  unlet s:file
endfunction!

function! s:color_coded_last_error()
  if index(g:color_coded_filetypes, &ft) < 0
    return
  endif
ruby << EOF
  VIM::command("echo \"" + color_coded_last_error().gsub("\"", "'") + "\"")
EOF
endfunction!

command! CCerror call s:color_coded_last_error()

let $VIMHOME=expand('<sfile>:p:h:h')
augroup color_coded
  au VimEnter,ColorScheme * source $VIMHOME/after/syntax/color_coded.vim
  au VimEnter,BufEnter * call s:color_coded_enter()
  au TextChanged,TextChangedI * call s:color_coded_push()
  au CursorMoved,CursorMovedI * call s:color_coded_moved()
  au CursorHold,CursorHoldI * call s:color_coded_pull()
  au BufDelete * call s:color_coded_destroy(expand('<afile>'))
augroup END

" ------------------------------------------------------------------------------
let &cpo = s:keepcpo
unlet s:keepcpo
