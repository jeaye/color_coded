" Vim global plugin for semantic highlighting using libclang
" Maintainer:	Jeaye <contact@jeaye.com>

" Setup
" ------------------------------------------------------------------------------

" TODO: Version check.
"let s:color_coded_api_version = 0xba89eb5
let s:color_coded_valid = 1
let s:color_coded_unique_counter = 1
let g:color_coded_matches = {}
let s:rpc_push = "push"
let s:rpc_pull = "pull"
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
    echo "color_coded: jobid " . l:jobid
    let s:color_coded_job_id = l:jobid
  endif

  if 0 == s:color_coded_job_id
    echoerr "color_coded: cannot start rpc process"
    let s:color_coded_valid = 0
  elseif -1 == s:color_coded_job_id
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

function! color_coded#push()
  if index(g:color_coded_filetypes, &ft) < 0 || g:color_coded_enabled == 0
    return
  endif
  let [l:name, l:data] = color_coded#get_buffer_details()
  call rpcnotify(s:color_coded_job_id, s:rpc_push, l:name, &ft, l:data)
endfunction!

function! color_coded#pull()
  if index(g:color_coded_filetypes, &ft) < 0 || g:color_coded_enabled == 0
    return
  endif
  let l:name = color_coded#get_base_buffer_name()
  call rpcnotify(s:color_coded_job_id, s:rpc_pull, l:name)
endfunction!

function! color_coded#move()
  if index(g:color_coded_filetypes, &ft) < 0 || g:color_coded_enabled == 0
    return
  endif
  let l:name = color_coded#get_base_buffer_name()
  call rpcnotify(s:color_coded_job_id, s:rpc_move, l:name, line("w0"), line("w$"))
endfunction!

function! color_coded#enter_buffer()
  if index(g:color_coded_filetypes, &ft) < 0 || g:color_coded_enabled == 0
    return
  endif

  " Each new window controls highlighting separate from the buffer
  if !exists("w:color_coded_own_syntax") || w:color_coded_name != color_coded#get_buffer_name()
    " Preserve spell after ownsyntax clears it
    let s:keepspell = &spell
      if has("b:current_syntax")
        execute "ownsyntax " . b:current_syntax
      else
        execute "ownsyntax " . &ft
      endif
      let &spell = s:keepspell
    unlet s:keepspell

    let w:color_coded_own_syntax = 1

    " Each window has a unique ID
    let w:color_coded_unique_counter = s:color_coded_unique_counter
    let s:color_coded_unique_counter += 1

    " Windows can be reused; clear it out if needed
    if exists("w:color_coded_name")
      call color_coded#clear_matches(w:color_coded_name)
    endif
    let w:color_coded_name = color_coded#get_buffer_name()
    call color_coded#clear_matches(w:color_coded_name)
  endif

  let [l:name, l:data] = color_coded#get_buffer_details()
  call rpcnotify(s:color_coded_job_id, s:rpc_enter_buffer, l:name, &ft, l:data)
endfunction!

function! color_coded#delete_buffer()
  if index(g:color_coded_filetypes, &ft) < 0 || g:color_coded_enabled == 0
    return
  endif

  call rpcnotify(s:color_coded_job_id, s:rpc_delete_buffer, color_coded#get_base_buffer_name())
  call color_coded#clear_matches(color_coded#get_buffer_name())
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
    call color_coded#clear_all_matches()
    echo "color_coded: disabled"
  else
    call color_coded#enter_buffer()
    echo "color_coded: enabled"
  endif
endfunction!

" Utilities
" ------------------------------------------------------------------------------

function! color_coded#get_base_buffer_name()
  let s:name = nvim_buf_get_name(0)
  if s:name == ""
    let s:name = nvim_buf_get_number(0)
  endif
  return s:name
endfunction!

" We keep two sets of buffer names right now
" 1) color_coded#get_base_buffer_name
"   - Just the filename or buffer number
"   - Used for interfacing with native code
" 2) color_coded#get_buffer_name
"   - A combination of 1) and a unique window counter
"   - Used for storing per-window syntax matches
function! color_coded#get_buffer_name()
  let l:file = color_coded#get_base_buffer_name()
  if exists("w:color_coded_unique_counter")
    return l:file . w:color_coded_unique_counter
  else
    return l:file
  endif
endfunction!

function! color_coded#get_buffer_details()
  let l:file = color_coded#get_base_buffer_name()
  let l:data = join(nvim_buf_get_lines(0, 0, -1, 0), "\n")
  return [l:file, l:data]
endfunction!

function! color_coded#add_match(type, line, col, len)
  let l:file = color_coded#get_buffer_name()
  call add(g:color_coded_matches[l:file], matchaddpos(a:type, [[ a:line, a:col, a:len ]], -1))
endfunction!

" Clears color_coded matches only in the current buffer
function! color_coded#clear_matches(file)
  try
    if has_key(g:color_coded_matches, a:file) == 1
      for id in g:color_coded_matches[a:file]
        call matchdelete(id)
      endfor
    endif
  catch
    echoerr "color_coded caught: " . v:exception
  finally
    let g:color_coded_matches[a:file] = []
  endtry
endfunction!

" Clears color_coded matches in all open buffers
function! color_coded#clear_all_matches()
  let g:color_coded_matches = {}
endfunction!
