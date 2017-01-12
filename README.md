color_coded: semantic highlighting with vim [![Build Status](https://travis-ci.org/jeaye/color_coded.svg?branch=master)](https://travis-ci.org/jeaye/color_coded)
---
color_coded is a vim plugin that provides realtime (fast), tagless code highlighting for C++, C, and Objective C using libclang.

  * Environmentally friendly
  * Lua binding for VimL -> C++
  * Exhaustive customization possibilities
  * Unintrusive highlighting; self-contained
  * Utilizes self-contained clang 3.9.0

Before color_coded | After color_coded
:------------------:|:------------------:
<img src="https://raw.githubusercontent.com/jeaye/color_coded/master/pics/trimmed_cc_t_no_3.png" width="100%"/> | <img src="https://raw.githubusercontent.com/jeaye/color_coded/master/pics/trimmed_cc_t_yes_3.png" width="100%"/>
<img src="https://raw.githubusercontent.com/jeaye/color_coded/master/pics/trimmed_cc_t_no_1.png" width="100%"/> | <img src="https://raw.githubusercontent.com/jeaye/color_coded/master/pics/trimmed_cc_t_yes_1.png" width="100%"/>

Installation
---
#### OS X
Before installing color_coded on OS X, please read the following. The vim that ships with (even the latest) OS X is not new enough to support color_coded. Furthermore, the default macvim from [homebrew](http://brew.sh/) doesn't have lua enabled. To ensure you have a valid macvim install on OS X, please do the following:
```bash
brew update
brew install macvim --with-lua --with-override-system-vim
brew linkapps macvim
brew install xz cmake
# Spawn a new shell to use the new system vim
```

To get proper highlighting, you'll also want the Xcode command line tools, which
will give you the appropriate system headers:
```bash
xcode-select --install
```

#### Ubuntu/Debian
First, install all required dependencies.
```bash
[sudo] apt-get install build-essential libclang-3.9-dev libncurses-dev libz-dev cmake xz-utils libpthread-workqueue-dev
```

For lua, you must install the version that your version of vim is compiled for.
```bash
vim --version | grep lua
```
Find your version number `-llua5.x` and use it to install the correct version.
```bash
[sudo] apt-get install liblua5.x-dev lua5.x
```

You also need to ensure you have GCC 4.9 (or higher). If you don't, you can try the following (tested on Ubuntu 14.04).
```bash
# Install GCC 4.9
[sudo] add-apt-repository ppa:ubuntu-toolchain-r/test
[sudo] apt-get update
[sudo] apt-get install g++-4.9

# Prefer 4.9 to other versions
[sudo] update-alternatives --remove-all g++
[sudo] update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.9 50
```

#### FreeBSD / PCBSD

(Tested in FreeBSD 10.1)

```bash
$ [sudo] pkg install clang36 llvm36 cmake
```

```bash
$ cd ~/.vim/bundle/color_coded
$ mkdir build && cd build
$ cmake -DCUSTOM_CLANG=1 -DLLVM_ROOT_PATH=/usr/local/llvm36  ..
$ make && make install
```

#### Vundle and Pathogen
Installation has been tested using [vundle](https://github.com/gmarik/Vundle.vim), but should also be compatible with [pathogen](https://github.com/tpope/vim-pathogen). To install using vundle (add the line to your `~/.vimrc`, restart vim, run `:PluginInstall`):

```viml
Plugin 'jeaye/color_coded'
```

#### NeoBundle

Installation with [NeoBundle](https://github.com/Shougo/neobundle.vim) supports automatically building and lazy-loading the plugin:

```viml
NeoBundleLazy 'jeaye/color_coded', {
  \ 'build': {
    \   'unix': 'cmake . && make && make install',
  \ },
  \ 'autoload': { 'filetypes' : ['c', 'cpp', 'objc', 'objcpp'] },
  \ 'build_commands' : ['cmake', 'make']
\}
```

#### All

Since color_coded has a compiled component, you'll need to manually compile when installing and updating (unless your vim package manager does it for you). Compilation works as follows, assuming usage of vundle (see the [dependencies](https://github.com/jeaye/color_coded#dependencies) section to ensure you can `make` properly):

```bash
cd ~/.vim/bundle/color_coded
mkdir build && cd build
cmake ..
make && make install # Compiling with GCC is preferred, ironically
# Clang works on OS X, but has mixed success on Linux and the BSDs

# Cleanup afterward; frees several hundred megabytes
make clean && make clean_clang
```

For various compatibility reasons, color_coded will attempt to download a known version of clang. This may add time to your configuration process, but it offers more stability across multiple platforms. Avoiding this is not officially supported, but documented [here](https://github.com/jeaye/color_coded#how-can-i-use-a-custom-llvmclang-setup).

**ANOTHER NOTE:** color_coded doesn't reliably support luajit. More informatively, [luajit doesn't reliably support being embedded in shared libraries](https://www.freelists.org/post/luajit/OSx-load-luajit-64bit-from-plugin).

Usage
---
Once color_coded is installed and compiled, it will automatically begin working the next time you start vim. In order for color_coded to know how your code must be compiled, you _may_ need to create a file describing the required compiler options. Note, color_coded assumes sane defaults and tries to allow for immediate usage.

That said, in any non-trivial case, you'll find yourself needing to supply a `.color_coded` file for your project. For each file, color_coded will search from the file's directory all the way up to the root of the filesystem looking for a `.color_coded` file or a `.color_coded_foo` file (where `foo` is the filetype; i.e. `c`, `cpp`, etc). This makes it possible for you to have one in your home directory, for example, and then in each of your projects' directories. If you don't specify one in a project directory, the one in your home directory is used. It also makes it possible to provide different flags for differen file types (C, C++, etc) and also work in subprojects with different flags. Again, if no such files are found, sane defaults will be applied.

**color_coded will try its hardest to highlight your code for you, even if there are errors in the translation unit.** This allows for better highlighting while typing and highlighting of valid code when color_coded doesn't know everything about your project.

### .color_coded file contents
The contents of a `.color_coded` or `.color_coded_foo` file is simply a line-separated list of compiler options. For example, the `.color_coded` file for color_coded is (color_coded puts the rest in by default when it detects C or C++ files):

```
-Iinclude
-Ilib/jest/include
```

The `.color_coded` file can be automatically generated using [YCM-Generator](https://github.com/rdnetto/YCM-Generator).

**NOTE:** Out of simplicity, no other data is permitted in the `.color_coded` file. That is to say, comments are not supported.

Options
---

#### `g:color_coded_enabled`
This option controls whether or not color_coded will function. Invoking the `:CCtoggle` command will modify this option to achieve its results. You may also specify it as `0` in your `~/.vimrc` to globally disable color_coded.

Default:
```viml
let g:color_coded_enabled = 1
```

#### `g:color_coded_filetypes`
This option controls the filetypes which color_coded will try to compile.

Default:
```viml
let g:color_coded_filetypes = ['c', 'cpp', 'objc']
```

Highlighting
---
There are many new highlighting groups which color_coded adds. They are designed to follow [libclang's internals](http://clang.llvm.org/doxygen/group__CINDEX.html#gaaccc432245b4cd9f2d470913f9ef0013) as closely as possible. To tie things together, some wrapper groups have been created that will allow more sweeping changes. The clang groups, by default, are mapped to use these, along with vim's normal groups (`Function`, `Macro`, `Number`, etc).

#### Generic groups
```viml
" To get full highlighting, specify these in your colorscheme.
" See colors/twilighted.vim for example usage

hi Member " Any non-static member variable
hi Variable " Any non-member variable
hi Namespace
hi EnumConstant
```

#### Example of clang groups
```viml
hi link StructDecl Type
hi link UnionDecl Type
hi link ClassDecl Type
hi link EnumDecl Type
```

#### Sample colorscheme
color_coded ships with a sample color scheme demonstrating support for the extended groups. To use it, invoke `:colorscheme twilighted`. As the name may suggest, the colorscheme is forked from vim's own twilight theme and has been appropriately punned into that which you'll see in the repo. To examine the implementation, visit `colors/twilighted.vim`. Note that color_coded can work with any colorscheme; this is just a sample.

For more information on all of the supported groups, see `after/syntax/color_coded.vim` and the [clang documentation](http://clang.llvm.org/doxygen/group__CINDEX.html#gaaccc432245b4cd9f2d470913f9ef0013).

Commands
---

#### `:CCerror`
This command outputs the last compilation error message from libclang. If your highlighting is not working properly, you may have a misconfigured `.color_coded` file or you may have syntax errors in your source. When in doubt, check here first.

#### `:CCtoggle`
This command enables color_coded, if it's currently disabled, or disables it, if it's currently enabled. This enable/disabling is done via the `g:color_coded_enabled` option.

Dependencies
---
Compilation of color_coded requires:
  * GCC ≥ 4.9
  * Lua ≥ 5.1.x (using the version with which vim was compiled)

Usage of color_coded requires vim:
  * Version: 7.4p330+
  * Compiled with Lua support (+lua)

Troubleshooting/FAQ
---

#### As I type, the highlighting becomes messed up
color_coded tries to compile your coded as you type it. Due to how vim works, color_coded can only update the highlighting once you do something (move the cursor, make a change, etc). When you're done typing and the highlighting has not finished updating, assuming your `.color_coded` file is sufficient, moving the cursor (or causing events some other way) will allow color_coded to finish.

Note, there is a hold event in vim which triggers *after* you stop typing and *after* some delay. color_coded also hooks into this and will use it to apply highlighting if possible.

#### Some bits aren't highlighted or are highlighted incorrectly
Yup. Believe it or not, these are almost certainly libclang bugs. I've been sorting out a few of them and I'm maintaining my own fork of libclang. If you would like to report such an issue, check out [this ticket](https://github.com/jeaye/color_coded/issues/2).

#### The highlighting isn't refreshed in a new buffer until I move the cursor
This is intentional. The first time you open a buffer, color_coded doesn't know if it's going to compile properly and it doesn't want you to wait while it tries to figure this out. color_coded will always compile in the background and events like moving the cursor or changing text will poll for updates. **Note, however,** that, once a buffer has highlighting, leaving that buffer and coming back to it will synchronously apply the previous highlighting.

#### "color_coded unavailable: you need to compile it"
When you install color_coded, you need to manually compile it before you can successfully use it. See [Installation](https://github.com/jeaye/color_coded#installation) for instructions.

#### "color_coded has been updated: you need to recompile it"
Assuming you've updated a working installation of color_coded, you'll get this error if the update requires you to recompile color_coded (i.e. there have been changes to the native API). To recompile, follow the [same exact steps](https://github.com/jeaye/color_coded#installation) you took to compile initially.

#### Does color_coded work with neovim?
For now, at least, color_coded is not supporting neovim. There is [chromatica.vim](https://github.com/arakashic/chromatica.nvim), however, which aims to provide similar highlighting, specifically for neovim.

#### 'stdarg.h' file not found
This happens on certain operating systems which require additional include
paths; the solution is to just make sure these paths are in your `.color_coded`
file, prefixed with `-isystem`. To find out those paths, run the following:

```bash
echo | clang -v -E -x c++ -
```

See [this issue comment](https://github.com/jeaye/color_coded/issues/140#issuecomment-255614113) for details. color_coded tries to help by assuming some of these, as shown in `post_constants()` [here](https://github.com/jeaye/color_coded/blob/master/include/conf/defaults.hpp).

#### color_coded crashes on startup?!?!
You're likely using luajit, which doesn't embed well in shared libraries. If you aren't sure, see if this turns up anything:
```bash
vim --version | grep jit
```
[More information is here](https://www.freelists.org/post/luajit/OSx-load-luajit-64bit-from-plugin).

#### How can I disable color_coded in diff mode?
You can use this in your `.vimrc`:

```viml
" Disable color_coded in diff mode
if &diff
  let g:color_coded_enabled = 0
endif
```

#### How can I use a custom LLVM/Clang setup?
You may specify `DOWNLOAD_CLANG=0` to `cmake`. Depending on your platform, you
may also need to specify the path to your llvm-config binary using
`LLVM_CONFIG=`. This can also allow you to use different versions of Clang/LLVM.

Example:
```bash
cmake . -DDOWNLOAD_CLANG=0
```

#### "no version information available (required by gvim)"
You may run into this on Debian-based systems. [The solution](https://github.com/jeaye/color_coded/issues/83) has been to install `liblua-5.x-0-dbg`.

#### "E315: ml_get: invalid lnum: 32"
This is caused by a bug in vim; you can work around it by using a vim with at
least patch 1691. See [this issue](https://github.com/jeaye/color_coded/issues/112) for details.

#### How can I get support?
Feel free to make an issue on Github or email me or catch me on IRC: Freenode @ `#color_coded`

## Donate
Feel free to shoot Bitcoins my way: **1BRbkEpuQYmPptHdkfkDfNnnnMJd9GdCTW**

For more information regarding how I use donations, see
[here](http://jeaye.com/donate/).

License
---
color_coded is under the MIT open-source license.  
See the `LICENSE` file or http://opensource.org/licenses/MIT
