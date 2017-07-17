//
//  FunctionParser.cpp
//  Function Analysis In Codebases
//
//  Created by Tiago Ferreira on 08/06/2017.
//  Copyright 2017 Tiago Ferreira
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
//  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/AST/DeclarationName.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"
#include "StringToArgVC.cpp"
#include <sstream>

using namespace clang::ast_matchers;
using namespace clang::tooling;
using namespace clang;
using namespace llvm;
using std::string;
using std::vector;

namespace FuncParser {

    struct Function {
        string name;
        string declFile;
        int numberOfCalls;
        vector<string> callers;
        vector<string> callersFile;
    };

    static llvm::cl::OptionCategory FAICCategory("FAIC Options");
    static cl::extrahelp FAICHelp(CommonOptionsParser::HelpMessage);
    static cl::extrahelp MoreHelp("\nMore help text...");

    class MyPrinter : public MatchFinder::MatchCallback {
        public: virtual void run(const MatchFinder::MatchResult &Result) {

            ASTContext *Context = Result.Context;
            SourceManager *SManager = Result.SourceManager;

            if (const CallExpr *E = Result.Nodes.getNodeAs<clang::CallExpr>("functions")) {
                FullSourceLoc FullLocation = Context->getFullLoc(E->getLocStart());

                if (FullLocation.isValid()) {
                    StringRef filename = SManager->getFilename(FullLocation);
                    std::string functionName = E->getDirectCallee()->getNameAsString();

                    llvm::outs() << "Found call " << functionName
                    << " at " << FullLocation.getSpellingLineNumber()
                    << ":" << FullLocation.getSpellingColumnNumber()
                    << " in file " << filename << "\n";
                }
            }
        }
    };

    class : public DiagnosticConsumer {
    public:
        virtual bool IncludeInDiagnosticCounts() const {
            return false; // Enough of that "not-so-fatal error" garbage.
        }
    } diagConsumer;


    void getFuncDecls(vector<string> &files, vector<string> &functionDecls) {
        std::stringstream filesStringStream; // Synthesise CLI entry from files vector.
        filesStringStream << "FAIC ";
        for(size_t i = 0; i < files.size(); ++i) {
            if(i != 0)
            filesStringStream << " ";
            filesStringStream << files[i];
        }
        filesStringStream << " --";
        std::string filesString = filesStringStream.str();

        int filesArgc;
        char ** varFilesArgv;
        stringToArgcArgv(filesString, &filesArgc, &varFilesArgv); // CommonOptionsParser really likes argc/v, so we'll fake it.
        const char ** filesArgv = const_cast<const char**>(varFilesArgv); // Dirty, but does the job.

        CommonOptionsParser OptionsParser(filesArgc, filesArgv, FAICCategory); // Setup parser from string to autogenerate CompilationsDatabse
        ClangTool Tool(OptionsParser.getCompilations(), OptionsParser.getSourcePathList());

        FuncParser::MyPrinter Printer;
        MatchFinder Finder;

        StatementMatcher functionMatcher = callExpr(
            callee(functionDecl(unless(hasOverloadedOperatorName("=")))),
            unless(isExpansionInSystemHeader())
        ).bind("functions");

        Finder.addMatcher(functionMatcher, &Printer);
        Tool.setDiagnosticConsumer(&diagConsumer);
        Tool.run(newFrontendActionFactory(&Finder).get());
    }
}
