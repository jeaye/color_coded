" Vim global plugin for semantic highlighting using libclang
" Maintainer:	Jeaye <contact@jeaye.com>

" ------------------------------------------------------------------------------
if !has("nvim")
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
let s:root_dir = expand("<sfile>:p:h:h")
let s:keepcpo = &cpo
set cpo&vim

" ------------------------------------------------------------------------------
let s:color_coded_valid = color_coded#setup()
if s:color_coded_valid == 1
  command! CCLog call color_coded#open_log()
  command! CCToggle call color_coded#toggle()

  augroup color_coded
    " TODO: Update path.
    au VimEnter,ColorScheme * exe "source " . s:root_dir . "/../after/syntax/color_coded.vim"
    au BufEnter * call color_coded#enter_buffer(expand("<afile>"))
    au WinEnter * call color_coded#enter_buffer(expand("<afile>"))
    " We only recompile when in normal mode, since recompiling for every key
    " press in insert mode causes way too much churn and can cause flickering.
    au TextChanged,InsertLeave * call color_coded#recompile(expand("<afile>"))
    " TODO: When moving, highlight the new area outside of the old area without clearing.
    au CursorMoved,CursorMovedI * call color_coded#move(expand("<afile>"))
    au CursorHold,CursorHoldI * call color_coded#move(expand("<afile>"))
    " Resized events trigger midway through a vim state change; the buffer
    " name will still be the previous buffer, yet the window-specific
    " variables won't be available.
    "au VimResized * call color_coded#move()

    " TODO: Maybe restore this.
    " Leaving a color_coded buffer requires removing matched positions.
    "au BufLeave * call color_coded#clear_matches(color_coded#get_buffer_name())

    " There is a rogue BufDelete at the start of vim; the buffer name ends up
    " being relative, so it's not a bother, but it's certainly odd.
    au BufDelete * call color_coded#delete_buffer(expand("<afile>"))
  augroup END

  nnoremap <silent> <ScrollWheelUp>
        \ <ScrollWheelUp>:call color_coded#move(expand("%"))<CR>
  inoremap <silent> <ScrollWheelUp>
        \ <ScrollWheelUp><ESC>:call color_coded#move(expand("%"))<CR><INS>
  nnoremap <silent> <ScrollWheelDown>
        \ <ScrollWheelDown>:call color_coded#move(expand("%"))<CR>
  inoremap <silent> <ScrollWheelDown>
        \ <ScrollWheelDown><ESC>:call color_coded#move(expand("%"))<CR><INS>

  nnoremap <silent> <S-ScrollWheelUp>
        \ <S-ScrollWheelUp>:call color_coded#move(expand("%"))<CR>
  inoremap <silent> <S-ScrollWheelUp>
        \ <S-ScrollWheelUp><ESC>:call color_coded#move(expand("%"))<CR><INS>
  nnoremap <silent> <S-ScrollWheelDown>
        \ <S-ScrollWheelDown>:call color_coded#move(expand("%"))<CR>
  inoremap <silent> <S-ScrollWheelDown>
        \ <S-ScrollWheelDown><ESC>:call color_coded#move(expand("%"))<CR><INS>

  nnoremap <silent> <C-ScrollWheelUp>
        \ <C-ScrollWheelUp>:call color_coded#move(expand("%"))<CR>
  inoremap <silent> <C-ScrollWheelUp>
        \ <C-ScrollWheelUp><ESC>:call color_coded#move(expand("%"))<CR><INS>
  nnoremap <silent> <C-ScrollWheelDown>
        \ <C-ScrollWheelDown>:call color_coded#move(expand("%"))<CR>
  inoremap <silent> <C-ScrollWheelDown>
        \ <C-ScrollWheelDown><ESC>:call color_coded#move(expand("%"))<CR><INS>
endif

" ------------------------------------------------------------------------------
let &cpo = s:keepcpo
unlet s:keepcpo
