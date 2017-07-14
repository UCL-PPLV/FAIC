LLVMDIR := /usr/local/Cellar/llvm/4.0.1
CFLAGS := $(shell $(LLVMDIR)/bin/llvm-config --cxxflags && $(LLVMDIR)/bin/llvm-config --ldflags --libs --system-libs) -lclangAST -lclangASTMatchers -lclangAnalysis -lclangBasic -lclangDriver -lclangEdit -lclangFrontend -lclangFrontendTool -lclangLex -lclangParse -lclangSema -lclangEdit -lclangRewrite -lclangRewriteFrontend -lclangStaticAnalyzerFrontend -lclangStaticAnalyzerCheckers -lclangStaticAnalyzerCore -lclangSerialization -lclangToolingCore -lclangTooling -lclangFormat -lboost_filesystem

all: FAIC.cpp FSManager.cpp FunctionParser.cpp StringToArgVC.cpp
	clang++ $< $(CFLAGS) -o FAIC

test:
	./FAIC test.cpp --

clean:
	rm -rf FAIC FAIC.dSYM
