" Vim global plugin for semantic highlighting using libclang
" Maintainer:	Jeaye <contact@jeaye.com>

" Setup
" ------------------------------------------------------------------------------

" TODO: Version check.
"let s:color_coded_api_version = 0xba89eb5
let s:color_coded_valid = 1

let s:rpc_push = "push"
let s:rpc_enter_buffer = "enter_buffer"
let s:rpc_delete_buffer = "delete_buffer"
let s:rpc_move = "move"
let s:rpc_open_log = "open_log"

let s:root_dir = expand("<sfile>:p:h:h")
let s:clang_dir = s:root_dir . "/build-latest/clang-llvm"
let s:clang_lib_dir = s:clang_dir . "/lib"
let s:bin = s:root_dir . "/bin/color_coded"

if !exists("s:color_coded_job_id")
  let s:color_coded_job_id = 0
endif

function! s:color_coded_create_defaults()
  if !exists("g:color_coded_filetypes")
    let g:color_coded_filetypes = ["c", "cpp", "objc"]
  endif
endfunction!

function! color_coded#setup()
  "echo "color_coded: setup"
  if s:color_coded_job_id == 0
    let l:jobid = jobstart("LD_LIBRARY_PATH=" . s:clang_lib_dir . " " .  s:bin . " " . s:clang_dir, { "rpc": v:true })
    "echo "color_coded: jobid " . l:jobid
    let s:color_coded_job_id = l:jobid
  endif

  if s:color_coded_job_id == 0
    echoerr "color_coded: cannot start rpc process"
    let s:color_coded_valid = 0
  elseif s:color_coded_job_id == -1
    echoerr "color_coded: rpc process is not executable"
    let s:color_coded_valid = 0
  else
    "echo "color_coded: job started"
    call s:color_coded_create_defaults()

    " TODO: Update path.
    exe "source " . s:root_dir . "/../after/syntax/color_coded.vim"
  endif

  return s:color_coded_valid
endfunction!

" Events
" ------------------------------------------------------------------------------

function! color_coded#push(buffer_file)
  if index(g:color_coded_filetypes, &ft) < 0 || g:color_coded_enabled == 0
    return
  endif

  let l:buffer_number = bufnr(a:buffer_file)
  let [l:name, l:data] = color_coded#get_buffer_details(l:buffer_number)
  call rpcnotify(s:color_coded_job_id, s:rpc_push, l:name, &ft, l:data, l:buffer_number)
endfunction!

function! color_coded#move(buffer_file)
  if index(g:color_coded_filetypes, &ft) < 0 || g:color_coded_enabled == 0
    return
  endif

  let l:buffer_number = bufnr(a:buffer_file)
  let l:name = color_coded#get_buffer_name(l:buffer_number)
  call rpcnotify(s:color_coded_job_id, s:rpc_move, l:name, line("w0"), line("w$"), l:buffer_number)
endfunction!

function! color_coded#enter_buffer(buffer_file)
  if index(g:color_coded_filetypes, &ft) < 0 || g:color_coded_enabled == 0
    return
  endif

  " Each new window controls highlighting separate from the buffer.
  let l:buffer_number = bufnr(a:buffer_file)
  if !exists("w:color_coded_own_syntax")
    " Preserve spell after ownsyntax clears it.
    let s:keepspell = &spell
      if has("b:current_syntax")
        execute "ownsyntax " . b:current_syntax
      else
        execute "ownsyntax " . &ft
      endif
      let &spell = s:keepspell
    unlet s:keepspell

    let w:color_coded_own_syntax = 1
  endif

  let [l:name, l:data] = color_coded#get_buffer_details(l:buffer_number)
  call rpcnotify(s:color_coded_job_id, s:rpc_enter_buffer, l:name, &ft, l:data, l:buffer_number)
endfunction!

function! color_coded#delete_buffer(buffer_file)
  if index(g:color_coded_filetypes, &ft) < 0 || g:color_coded_enabled == 0
    return
  endif

  let l:buffer_number = bufnr(a:buffer_file)
  call rpcnotify(s:color_coded_job_id, s:rpc_delete_buffer, color_coded#get_buffer_name(), l:buffer_number)
endfunction!

" Commands
" ------------------------------------------------------------------------------

function! color_coded#open_log()
  "echo "color_coded: open log job id " . s:color_coded_job_id
  call rpcnotify(s:color_coded_job_id, s:rpc_open_log)
endfunction!

function! color_coded#toggle()
  let g:color_coded_enabled = g:color_coded_enabled ? 0 : 1
  if g:color_coded_enabled == 0
    " TODO: Clear all matches.
    " TODO: Stop the job.
    echo "color_coded: disabled"
  else
    call color_coded#enter_buffer()
    echo "color_coded: enabled"
  endif
endfunction!

" Utilities
" ------------------------------------------------------------------------------

function! color_coded#get_buffer_name(buffer_number)
  let s:name = nvim_buf_get_name(a:buffer_number)
  if s:name == ""
    let s:name = nvim_buf_get_number(a:buffer_number)
  endif
  return s:name
endfunction!

function! color_coded#get_buffer_details(buffer_number)
  let l:file = color_coded#get_buffer_name(a:buffer_number)
  let l:data = join(nvim_buf_get_lines(a:buffer_number, 0, -1, 0), "\n")
  return [l:file, l:data]
endfunction!
