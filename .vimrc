" Plugin 'altercation/vim-colors-solarized'

Plugin 'vhdirk/vim-cmake'

let g:cmake_build_type = 'debug'

let g:solarized_termcolors=256
syntax enable
set background=dark
colorscheme solarized

" Autoformat

au BufWrite * :Autoformat

let g:autoformat_autoindent = 0
let g:autoformat_retab = 0
let g:autoformat_remove_trailing_spaces = 0
let g:formatdef_astyle_google_cpp = '"astyle --style=google"'
let g:formatters_cpp = ['astyle_google_cpp']

" syntastic settings

"set statusline+=%#warningmsg#
"set statusline+=%{SyntasticStatuslineFlag()}
"set statusline+=%*

let g:sytastic_c_checkers = ['gcc']
let g:syntastic_always_populate_loc_list = 1
let g:syntastic_auto_loc_list = 1
let g:syntastic_check_on_open = 1
let g:syntastic_check_on_wq = 0

let g:syntastic_c_include_dirs = [ 'src' ]
let g:syntastic_c_check_header = 1

let g:syntastic_cpp_compile = 'cmake'
let g:syntastic_cpp_checkers = ['gcc']
let g:syntastic_cpp_include_dirs = [ 'system/include', 'axis/include', 'common/include', 'tools/include', 'Debug/gtest-install/include', 'Debug/sap_1_0_latest-install/include' ]
let g:syntastic_cpp_compiler_options = '-std=c++11 -Wall'
let g:syntastic_cpp_check_header = 1


" let g:syntastic_debug = 1

" let g:syntastic_c_checkers = ['clang_check']
