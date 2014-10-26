" Vim global plugin for semantic highlighting using libclang
" Maintainer:	Jeaye <contact@jeaye.com>

" Setup
" ------------------------------------------------------------------------------

let s:color_coded_api_version = 7
let s:color_coded_valid = 1

function! s:color_coded_create_defaults()
  if !exists("g:color_coded_filetypes")
    let g:color_coded_filetypes = ['c', 'h', 'cpp', 'hpp', 'cc', 'm', 'mm']
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
      local name = color_coded_buffer_name()
      local line_count = #vim.buffer()
      local data = ''
      for i = 1,line_count do
        data = data .. vim.buffer()[i] .. "\n"
      end
      return name, data
    end
EOF
endfunction!

function! color_coded#setup()
  " Try to get the lua binding working
  lua << EOF
    package.cpath = package.cpath ..
                    ";" .. vim.eval("$VIMHOME") .. "/bin/color_coded.so"
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
  if index(g:color_coded_filetypes, &ft) < 0
    return
  endif
lua << EOF
  local name, data = color_coded_buffer_details()
  color_coded_push(name, data)
EOF
endfunction!

function! color_coded#pull()
  if index(g:color_coded_filetypes, &ft) < 0
    return
  endif
lua << EOF
  local name = color_coded_buffer_name()
  color_coded_pull(name)
EOF
endfunction!

function! color_coded#moved()
  if index(g:color_coded_filetypes, &ft) < 0
    return
  endif
lua << EOF
  local name = color_coded_buffer_name()
  color_coded_moved(name, vim.window().line, #vim.buffer(), vim.window().height)
EOF
endfunction!

function! color_coded#enter()
  if index(g:color_coded_filetypes, &ft) < 0
    return
  endif
lua << EOF
  local name, data = color_coded_buffer_details()
  color_coded_enter(name, data)
EOF
endfunction!

function! color_coded#destroy(file)
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

function! color_coded#last_error()
  if index(g:color_coded_filetypes, &ft) < 0
    return
  endif
lua << EOF
  vim.command("echo \"" .. string.gsub(color_coded_last_error(), "\"", "'") ..
              "\"")
EOF
endfunction!
