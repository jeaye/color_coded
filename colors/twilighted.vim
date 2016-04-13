" Example color scheme for color_coded
" Maintainer: Jeaye <contact@jeaye.com>
" Forked_from: twilight

let s:cpo_save = &cpo
set cpo&vim

hi clear
if exists("syntax_on")
    syntax reset
endif
let g:colors_name="twilighted"

let b:current_syntax = &cpo
set background=dark

" Lay out our palette
let s:grey_blue = '#8a9597'
let s:light_grey_blue = '#a0a8b0'
let s:dark_grey_blue = '#34383c'
let s:mid_grey_blue = '#64686c'
let s:beige = '#ceb67f'
let s:light_orange = '#ebc471'
let s:yellow = '#e3d796'
let s:violet = '#a982c8'
let s:magenta = '#a933ac'
let s:green = '#e0a96f'
let s:lightgreen = '#c2c98f'
let s:red = '#d08356'
let s:cyan = '#74dad9'
let s:darkgrey = '#1a1a1a'
let s:grey = '#303030'
let s:lightgrey = '#605958'
let s:white = '#fffedc'
let s:orange = '#d08356'

" We need to execute these indirectly to use our variables

" ====================== Important bits ======================
exe 'hi Normal guifg='.s:white .' guibg='.s:darkgrey
exe 'hi String guifg='.s:green .' guibg='.s:darkgrey .' gui=bold'
exe 'hi Function guifg='.s:violet .' guibg='.s:darkgrey .' gui=none'
exe 'hi Operator guifg='.s:light_orange .' guibg='.s:darkgrey .' gui=none'
exe 'hi Type guifg='.s:yellow .' guibg='.s:darkgrey .' gui=bold'
exe 'hi Number guifg='.s:orange .' guibg='.s:darkgrey .' gui=none'
exe 'hi Constant guifg='.s:red .' guibg='.s:darkgrey .' gui=none'
exe 'hi Macro guifg='.s:magenta .' guibg='.s:darkgrey .' gui=none'
exe 'hi PreProc guifg='.s:grey_blue .' guibg='.s:darkgrey .' gui=none'
exe 'hi Keyword guifg='.s:light_orange .' guibg='.s:darkgrey .' gui=none'
exe 'hi Comment guifg='.s:white .' guibg='.s:darkgrey .' gui=italic'
exe 'hi TODO guifg='.s:grey_blue .' guibg='.s:darkgrey .' gui=italic,bold'

" ~~~ These are specific to color_coded ~~~
exe 'hi Member guifg='.s:cyan .' guibg='.s:darkgrey .' gui=italic'
exe 'hi Variable guifg='.s:light_grey_blue .' guibg='.s:darkgrey .' gui=none'
exe 'hi Namespace guifg='.s:red .' guibg='.s:darkgrey .' gui=none'
exe 'hi EnumConstant guifg='.s:lightgreen .' guibg='.s:darkgrey .' gui=none'
" ====================== Important bits ======================

exe 'hi Underlined guifg='.s:white .' guibg='.s:darkgrey .' gui=underline'
exe 'hi NonText guifg='.s:lightgrey .' guibg='.s:grey
exe 'hi SpecialKey guifg='.s:grey .' guibg='.s:darkgrey
exe 'hi LineNr guifg='.s:mid_grey_blue .' guibg='.s:dark_grey_blue .' gui=none'
exe 'hi StatusLine guifg='.s:white .' guibg='.s:grey .' gui=italic,underline'
exe 'hi StatusLineNC guifg='.s:lightgrey .' guibg='.s:grey .' gui=italic,underline'
exe 'hi VertSplit guifg='.s:grey .' guibg='.s:grey .' gui=none'
exe 'hi Folded guifg='.s:grey_blue .' guibg='.s:dark_grey_blue .' gui=none'
exe 'hi FoldColumn guifg='.s:grey_blue .' guibg='.s:dark_grey_blue .' gui=none'
exe 'hi SignColumn guifg='.s:grey_blue .' guibg='.s:dark_grey_blue .' gui=none'
exe 'hi Title guifg='.s:red .' guibg='.s:darkgrey .' gui=underline'
exe 'hi Special guifg='.s:lightgreen .' guibg='.s:darkgrey .' gui=none'

hi CursorLine guibg=#262626
hi CursorColumn guibg=#262626
hi MatchParen guifg=white guibg=#80a090 gui=bold

" Tabpages
hi TabLine guifg=#a09998 guibg=#202020 gui=underline
hi TabLineFill guifg=#a09998 guibg=#202020 gui=underline
hi TabLineSel guifg=#a09998 guibg=#404850 gui=underline

" P-Menu (auto-completion)
hi Pmenu guifg=#a09998 guibg=#303030 gui=underline
hi PmenuSel guifg=#a09998 guibg=#404040 gui=underline

hi Visual guibg=#404040
hi Cursor guibg=#b0d0f0

hi Search guifg=#606000 guibg=#c0c000 gui=bold
hi Directory guifg=#dad085
hi Error guibg=#602020

let &cpo = s:cpo_save
unlet s:cpo_save
