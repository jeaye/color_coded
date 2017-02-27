" Vim global plugin for semantic highlighting using libclang
" Maintainer:	Jeaye <contact@jeaye.com>

" Setup
" ------------------------------------------------------------------------------

let s:color_coded_api_version = 0xba89eb5
let s:color_coded_valid = 1
let s:color_coded_unique_counter = 1
let g:color_coded_matches = {}

function! s:color_coded_create_defaults()
  if !exists("g:color_coded_filetypes")
    let g:color_coded_filetypes = ['c', 'cpp', 'objc']
  endif
endfunction!

function! s:color_coded_define_lua_helpers()
  lua << EOF
    function color_coded_buffer_name()
      local name = vim.buffer().fname
      if (name == nil or name == '') then
        name = tostring(vim.buffer().number)
      end
      return name
    end

    function color_coded_buffer_details()
      local line_count = #vim.buffer()
      local buffer = vim.buffer()
      local data = {}
      for i = 1,#buffer do
        -- NOTE: buffer is a userdata; must be copied into array
        data[i] = buffer[i]
      end
      return color_coded_buffer_name(), table.concat(data, '\n')
    end
EOF
endfunction!

function! color_coded#setup()
  " Try to get the lua binding working
  lua << EOF
    package.cpath = vim.eval("$VIMHOME") .. "/color_coded.so"
    local loaded = pcall(require, "color_coded")
    if not loaded then
      vim.command('echohl WarningMsg | ' ..
            'echomsg "color_coded unavailable: you need to compile it ' ..
            '(see README.md)" | ' ..
            'echohl None')
      vim.command("let s:color_coded_valid = 0")
      return
    else
      local version = color_coded_api_version()
      if version ~= vim.eval("s:color_coded_api_version") then
        vim.command('echohl WarningMsg | ' ..
            'echomsg "color_coded has been updated: you need to recompile it ' ..
            '(see README.md)" | ' ..
            'echohl None')
        vim.command("let s:color_coded_valid = 0")
      end
    end
EOF

  " Lua is prepared, finish setup
  call s:color_coded_create_defaults()
  call s:color_coded_define_lua_helpers()

  return s:color_coded_valid
endfunction!

" Events
" ------------------------------------------------------------------------------

function! color_coded#push()
  if index(g:color_coded_filetypes, &ft) < 0 || g:color_coded_enabled == 0
    return
  endif
lua << EOF
  local name, data = color_coded_buffer_details()
  color_coded_push(name, vim.eval('&ft'), data)
EOF
endfunction!

function! color_coded#pull()
  if index(g:color_coded_filetypes, &ft) < 0 || g:color_coded_enabled == 0
    return
  endif
lua << EOF
  local name = color_coded_buffer_name()
  color_coded_pull(name)
EOF
endfunction!

function! color_coded#moved()
  if index(g:color_coded_filetypes, &ft) < 0 || g:color_coded_enabled == 0
    return
  endif
lua << EOF
  local name = color_coded_buffer_name()
  color_coded_moved(name, vim.eval("line(\"w0\")"), vim.eval("line(\"w$\")"))
EOF
endfunction!

function! color_coded#enter()
  if index(g:color_coded_filetypes, &ft) < 0 || g:color_coded_enabled == 0
    return
  endif

  " Each new window controls highlighting separate from the buffer
  if !exists("w:color_coded_own_syntax") || w:color_coded_name != color_coded#get_buffer_name()
    " Preserve spell after ownsyntax clears it
    let s:keepspell = &spell
      if has('b:current_syntax')
        execute 'ownsyntax ' . b:current_syntax
      else
        execute 'ownsyntax ' . &ft
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

lua << EOF
  local name, data = color_coded_buffer_details()
  color_coded_enter(name, vim.eval('&ft'), data)
EOF
endfunction!

function! color_coded#destroy()
  if index(g:color_coded_filetypes, &ft) < 0 || g:color_coded_enabled == 0
    return
  endif
lua << EOF
  color_coded_destroy(color_coded_buffer_name())
EOF

  call color_coded#clear_matches(color_coded#get_buffer_name())
endfunction!

function! color_coded#exit()
  if g:color_coded_enabled == 0
    return
  endif
lua << EOF
  color_coded_exit()
EOF
endfunction!

" Commands
" ------------------------------------------------------------------------------

function! color_coded#last_error()
lua << EOF
  vim.command(
    "echo \"" .. string.gsub(color_coded_last_error(), "\"", "'") ..  "\""
  )
EOF
endfunction!

function! color_coded#toggle()
  let g:color_coded_enabled = g:color_coded_enabled ? 0 : 1
  if g:color_coded_enabled == 0
    call color_coded#clear_all_matches()
    echo "color_coded: disabled"
  else
    call color_coded#enter()
    echo "color_coded: enabled"
  endif
endfunction!

" Utilities
" ------------------------------------------------------------------------------

" We keep two sets of buffer names right now
" 1) Lua's color_coded_buffer_name
"   - Just the filename or buffer number
"   - Used for interfacing with C++
" 2) VimL's color_coded#get_buffer_name
"   - A combination of 1) and a unique window counter
"   - Used for storing per-window syntax matches
function! color_coded#get_buffer_name()
lua << EOF
  local name = color_coded_buffer_name()
  vim.command("let s:file = '" .. name .. "'")
EOF
  if exists("w:color_coded_unique_counter")
    return s:file . w:color_coded_unique_counter
  else
    return s:file
  endif
endfunction!

function! color_coded#add_match(type, line, col, len)
  let s:file = color_coded#get_buffer_name()
  call add(g:color_coded_matches[s:file],
          \matchaddpos(a:type, [[ a:line, a:col, a:len ]], -1))
  unlet s:file
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
    echomsg "color_coded caught: " . v:exception
  finally
    let g:color_coded_matches[a:file] = []
  endtry
endfunction!

" Clears color_coded matches in all open buffers
function! color_coded#clear_all_matches()
  let g:color_coded_matches = {}
endfunction!
