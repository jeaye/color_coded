let s:root_dir = expand("<sfile>:p:h:h")
exe "set rtp+=" . s:root_dir

" TODO: Remove once this has been moved up out of the rust dir.
let s:parent_dir = expand("<sfile>:p:h:h:h")
exe "set rtp+=" . s:parent_dir

source plugin/color_coded.vim
