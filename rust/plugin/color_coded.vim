" Vim global plugin for semantic highlighting using libclang
" Maintainer:	Jeaye <contact@jeaye.com>

" ------------------------------------------------------------------------------
if !has('nvim')
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
let $VIMHOME = expand('<sfile>:p:h:h')
let s:keepcpo = &cpo
set cpo&vim

" ------------------------------------------------------------------------------
let s:color_coded_valid = color_coded#setup()
if s:color_coded_valid == 1
  command! CCLog call color_coded#open_log()
  command! CCtoggle call color_coded#toggle()

  augroup color_coded
    au VimEnter,ColorScheme * source $VIMHOME/after/syntax/color_coded.vim
    au BufEnter * call color_coded#open()
    au WinEnter * call color_coded#open()
    au TextChanged,TextChangedI * call color_coded#push()
    au CursorMoved,CursorMovedI * call color_coded#move()
    au CursorHold,CursorHoldI * call color_coded#move()
    " Resized events trigger midway through a vim state change; the buffer
    " name will still be the previous buffer, yet the window-specific
    " variables won't be available.
    "au VimResized * call color_coded#move()

    " Leaving a color_coded buffer requires removing matched positions
    au BufLeave * call color_coded#clear_matches(color_coded#get_buffer_name())

    " There is a rogue BufDelete at the start of vim; the buffer name ends up
    " being relative, so it's not a bother, but it's certainly odd.
    au BufDelete * call color_coded#close()
  augroup END

  nnoremap <silent> <ScrollWheelUp>
        \ <ScrollWheelUp>:call color_coded#move()<CR>
  inoremap <silent> <ScrollWheelUp>
        \ <ScrollWheelUp><ESC>:call color_coded#move()<CR><INS>
  nnoremap <silent> <ScrollWheelDown>
        \ <ScrollWheelDown>:call color_coded#move()<CR>
  inoremap <silent> <ScrollWheelDown>
        \ <ScrollWheelDown><ESC>:call color_coded#move()<CR><INS>

  nnoremap <silent> <S-ScrollWheelUp>
        \ <S-ScrollWheelUp>:call color_coded#move()<CR>
  inoremap <silent> <S-ScrollWheelUp>
        \ <S-ScrollWheelUp><ESC>:call color_coded#move()<CR><INS>
  nnoremap <silent> <S-ScrollWheelDown>
        \ <S-ScrollWheelDown>:call color_coded#move()<CR>
  inoremap <silent> <S-ScrollWheelDown>
        \ <S-ScrollWheelDown><ESC>:call color_coded#move()<CR><INS>

  nnoremap <silent> <C-ScrollWheelUp>
        \ <C-ScrollWheelUp>:call color_coded#move()<CR>
  inoremap <silent> <C-ScrollWheelUp>
        \ <C-ScrollWheelUp><ESC>:call color_coded#move()<CR><INS>
  nnoremap <silent> <C-ScrollWheelDown>
        \ <C-ScrollWheelDown>:call color_coded#move()<CR>
  inoremap <silent> <C-ScrollWheelDown>
        \ <C-ScrollWheelDown><ESC>:call color_coded#move()<CR><INS>
endif

" ------------------------------------------------------------------------------
let &cpo = s:keepcpo
unlet s:keepcpo
