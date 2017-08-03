<h1 align="center">
FAIC
</h1>

Standing for **Function Analysis in Codebases**, FAIC is a C++ program that makes heavy use of LLVM Clang's LibTooling to produce a function call graph from an undocumented C++ library. This works mainly by parsing the source code's Abstract Syntax Tree and analysing the function use and hierarchy to determine their relevance and scope to the library's user. This map is, in turn, passed into automata learning algorithms, which will perform an in-depth analysis of the code and provide the user with warnings on how to use the library.

### Author

Tiago Ferreira | <a href="https://twitter.com/tiferrei2000/">Twitter</a> | <a href="https://www.tiferrei.com/">Website</a>

### License

FAIC Copyright (C) 2017 Tiago Ferreira
