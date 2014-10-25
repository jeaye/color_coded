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
let s:color_coded_valid = 1
let s:keepcpo = &cpo
set cpo&vim
" ------------------------------------------------------------------------------

if !exists("g:color_coded_filetypes")
  let g:color_coded_filetypes = ['c', 'h', 'cpp', 'hpp', 'cc', 'm', 'mm']
endif

let s:path = expand('<sfile>:p:h')
lua << EOF
  package.cpath = package.cpath ..
                  ";" .. vim.eval("s:path") .. "/../bin/color_coded.so"
  local loaded = pcall(require, "color_coded")
  if not loaded then
    vim.command('echohl WarningMsg | ' ..
          'echomsg "color_coded unavailable: you need to compile it ' ..
          '(see README.md)" | ' ..
          'echohl None')
    vim.command("let s:color_coded_valid = 0")
    return
  end
  function color_coded_buffer_name()
    local name = vim.buffer().fname
    if (name == nil or name == '') then
      name = tostring(vim.buffer().number)
    end
    return name
  end

  function color_coded_buffer_details()
    local name = color_coded_buffer_name()

    local line_count = #vim.buffer()
    local data = ''
    for i = 1,line_count do
      data = data .. vim.buffer()[i] .. "\n"
    end
    return name, data
  end
EOF

" Verify everything is ok
if s:color_coded_valid == 0
  finish
endif

function! s:color_coded_push()
  if index(g:color_coded_filetypes, &ft) < 0
    return
  endif
lua << EOF
  local name, data = color_coded_buffer_details()
  color_coded_push(name, data)
EOF
endfunction!

function! s:color_coded_pull()
  if index(g:color_coded_filetypes, &ft) < 0
    return
  endif
lua << EOF
  local name = color_coded_buffer_name()
  color_coded_pull(name)
EOF
endfunction!

function! s:color_coded_moved()
  if index(g:color_coded_filetypes, &ft) < 0
    return
  endif
lua << EOF
  local name = color_coded_buffer_name()
  color_coded_moved(name, vim.window().line, #vim.buffer(), vim.window().height)
EOF
endfunction!

function! s:color_coded_enter()
  if index(g:color_coded_filetypes, &ft) < 0
    return
  endif
lua << EOF
  local name, data = color_coded_buffer_details()
  color_coded_enter(name, data)
EOF
endfunction!

function! s:color_coded_destroy(file)
  if index(g:color_coded_filetypes, &ft) < 0
    return
  endif
  let s:file = a:file
lua << EOF
  local name = vim.eval('s:file')
  color_coded_destroy(name)
EOF
  unlet s:file
endfunction!

function! s:color_coded_last_error()
  if index(g:color_coded_filetypes, &ft) < 0
    return
  endif
lua << EOF
  vim.command("echo \"" .. string.gsub(color_coded_last_error(), "\"", "'") .. "\"")
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
