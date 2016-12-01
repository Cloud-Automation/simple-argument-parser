" Plugin 'altercation/vim-colors-solarized'

let g:solarized_termcolors=256
syntax enable
set background=dark
colorscheme solarized

" syntastic settings

set statusline+=%#warningmsg#
set statusline+=%{SyntasticStatuslineFlag()}
set statusline+=%*


let g:sytastic_c_checkers = ['gcc']
let g:syntastic_always_populate_loc_list = 1
let g:syntastic_auto_loc_list = 1
let g:syntastic_check_on_open = 1
let g:syntastic_check_on_wq = 0

let g:syntastic_c_include_dirs = [ 'src' ]
let g:syntastic_c_check_header = 1

let g:syntastic_cpp_include_dirs = [ 'src' ]
let g:syntastic_cpp_check_header = 1


" let g:syntastic_debug = 1

" let g:syntastic_c_checkers = ['clang_check']
