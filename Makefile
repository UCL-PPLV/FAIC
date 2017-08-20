OBJDIR := obj
BINDIR := bin
OBJS := $(addprefix $(OBJDIR)/,FAIC.o FSManager.o FunctionParser.o StringToArgVC.o)
BINS := $(addprefix $(BINDIR)/,FAIC)

LLVMDIR := /usr/local/Cellar/llvm/4.0.1
CFLAGS := -std=c++14 -I./include -I/usr/local/Cellar/llvm/4.0.1/include
LLVMFLAGS := $(shell $(LLVMDIR)/bin/llvm-config --cxxflags && $(LLVMDIR)/bin/llvm-config --ldflags --libs --system-libs) -I./include -lclangAST -lclangASTMatchers -lclangAnalysis -lclangBasic -lclangDriver -lclangEdit -lclangFrontend -lclangFrontendTool -lclangLex -lclangParse -lclangSema -lclangEdit -lclangRewrite -lclangRewriteFrontend -lclangStaticAnalyzerFrontend -lclangStaticAnalyzerCheckers -lclangStaticAnalyzerCore -lclangSerialization -lclangToolingCore -lclangTooling -lclangFormat -lboost_filesystem

.PHONY: all
all: $(BINS)

$(OBJDIR)/%.o: src/%.cpp | $(OBJDIR)
	clang++ $< $(CFLAGS)  -c -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

bin/FAIC: $(OBJS) | $(BINDIR)
	clang++ $(LLVMFLAGS) $(OBJS) -o bin/FAIC

$(BINDIR):
	mkdir -p $(BINDIR)

.PHONY: test
test:
	./bin/FAIC tests/test.cpp

.PHONY: clean
clean:
	rm -rf FAIC obj bin out
