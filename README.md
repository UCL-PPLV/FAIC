<h1 align="center">
FAIC
</h1>

Standing for **Function Analysis in Codebases**, FAIC is a C++ program that makes heavy use of LLVM Clang's LibTooling to produce a function call graph from an undocumented C++ library. This works mainly by parsing the source code's Abstract Syntax Tree and analysing the function use and hierarchy to determine their relevance and scope to the library's user. This map is, in turn, passed into automata learning algorithms, which will perform an in-depth analysis of the code and provide the user with warnings on how to use the library.

#### Getting Started (macOS)

Make sure you have the following dependencies:
* Cmake
* Make
* LLVM
* Clang
* Clang Headers
* Boost

Most of these come included with the Xcode CLI Tools. To install them download Xcode and run:
```
xcode-select --install
```

We can install other dependencies using Homebrew:
```
brew install cmake llvm clang boost
```

Make sure the new LLVM bins are added to your path:
```
export PATH="/usr/local/opt/llvm/bin:$PATH"
export CC=clang
export CXX=clang++
```

Let's now clone the source code:
```
git clone https://github.com/UCL-PPLV/FAIC && cd FAIC
```

#### Getting Started (Linux)

```
sudo apt-get update
sudo apt-get install git cmake make libcurses-dev
```

On Linux the only way I've found to get all the required Clang dependencies is by building from source. For this we have to clone LLVM and Clang:
```
git clone https://github.com/llvm-mirror/llvm.git
cd llvm/tools
git clone https://github.com/llvm-mirror/clang.git
cd ../..
```

LLVM does not support building in the source tree, so create a build foder by its side:
```
mkdir build
cd build
```

Now compile LLVM and Clang:
* Make sure you have enough memory for this. We are literally compiling a compiler, my Linux VM needed at least 16GB of memory.
* This might take some hours, if you haven't had your daily dose of caffeine, this is your chance. ☕
```
cmake -G "Unix Makefiles" ../llvm && make
```

To make sure everything went as planned:
```
make check-clang
```

If everything seems to be fine install it with:
```
sudo make install
```

#### Building FAIC
FAIC uses CMake to manage the build process. We recommend building FAIC in a subfolder:
```
mkdir build && cd build
```

Depending on your environment and preference you may pass CMake different flags.
To simply setup the build or use a CLI editor:
```
cmake .. && make
```

To setup the build and use an IDE (Xcode for example):
```
cmake -G Xcode .. && open FAIC.xcodeproj
```

### Author

Tiago Ferreira | <a href="https://twitter.com/tiferrei2000/">Twitter</a> | <a href="https://www.tiferrei.com/">Website</a>

### License

FAIC Copyright (C) 2017 Tiago Ferreira

Please check the <a href="https://github.com/tiferrei/FAIC/blob/master/LICENSE">license file</a> embed in this project for more details.
