set tabstop=4
set softtabstop=4
set shiftwidth=4
set noexpandtab

let &path.="./include,/usr/include,/usr/local/include"

if isdirectory('/usr/include/c++/4.6')
    set path+=/usr/include/c++/4.6
elseif isdirectory('/usr/local/Cellar/llvm/4.0.1')
	set path+=/usr/local/Cellar/llvm/4.0.1/include
endif

set includeexpr=substitute(v:fname,'\\.','/','g')
