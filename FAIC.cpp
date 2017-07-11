#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/AST/DeclarationName.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"

using namespace clang::ast_matchers;
using namespace clang::tooling;
using namespace clang;
using namespace llvm;

class MyPrinter : public MatchFinder::MatchCallback {
 public:
  virtual void run(const MatchFinder::MatchResult &Result) {
    ASTContext *Context = Result.Context;
	SourceManager *SManager = Result.SourceManager;
    if (const CallExpr *E =
            Result.Nodes.getNodeAs<clang::CallExpr>("functions")) {
      FullSourceLoc FullLocation = Context->getFullLoc(E->getLocStart());
      if (FullLocation.isValid()) {
		StringRef filename = SManager->getFilename(FullLocation);
		std::string functionName = E->getDirectCallee()->getNameAsString();
        llvm::outs() << "Found call " << functionName << " at " << FullLocation.getSpellingLineNumber()
                     << ":" << FullLocation.getSpellingColumnNumber()
					 << " in file " << filename << "\n";
      }
    }
  }
};

// Apply a custom category to all command-line options so that they are the
// only ones displayed.
static llvm::cl::OptionCategory MyToolCategory("my-tool options");

// CommonOptionsParser declares HelpMessage with a description of the common
// command-line options related to the compilation database and input files.
// It's nice to have this help message in all tools.
static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);

// A help message for this specific tool can be added afterwards.
static cl::extrahelp MoreHelp("\nMore help text...");

int main(int argc, const char **argv) {
  CommonOptionsParser OptionsParser(argc, argv, MyToolCategory);
  ClangTool Tool(OptionsParser.getCompilations(),
                 OptionsParser.getSourcePathList());

  MyPrinter Printer;
  MatchFinder Finder;

  StatementMatcher functionMatcher =
      callExpr(callee(functionDecl())).bind("functions");

  Finder.addMatcher(functionMatcher, &Printer);

  return Tool.run(newFrontendActionFactory(&Finder).get());
}
