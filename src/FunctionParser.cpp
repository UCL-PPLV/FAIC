//
//  FunctionParser.cpp
//  Function Analysis In Codebases
//
//  Created by Tiago Ferreira on 08/07/2017.
//  Copyright 2017 Tiago Ferreira
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
//  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/AST/DeclarationName.h"
#include "llvm/Support/CommandLine.h"
#include "clang/Tooling/Tooling.h"
#include "clang/AST/DeclBase.h"
#include "FAIC.hpp"
#include <sstream>

using namespace clang::ast_matchers;
using namespace clang::tooling;
using namespace clang;
using namespace llvm;
using namespace std;

vector<Function> functions;

void DeclFilter::run(const MatchFinder::MatchResult &Result) {

    ASTContext *Context = Result.Context;
    SourceManager *SManager = Result.SourceManager;

    if (const FunctionDecl *E = Result.Nodes.getNodeAs<clang::FunctionDecl>("functionDecls")) {
        FullSourceLoc FullLocation = Context->getFullLoc(E->getLocStart());
        if (FullLocation.isValid()) {
            StringRef filename = SManager->getFilename(FullLocation);
            std::string functionName = E->getNameAsString();
            if (isCPPFile(filename)) {
                if (!E->isOverloadedOperator() && functionName.find("~")) { // Ignore operator overloaders and destructors
                    bool functionIsPresent = false;
                    for(std::size_t i = 0; i < functions.size(); ++i) {
                        if (functions[i].identifier == functionName) {
                            functionIsPresent = true;
                        }
                    }
                    if (!functionIsPresent) {
                        Function newFunc;
                        newFunc.identifier = functionName;
                        newFunc.declFile = filename;
                        functions.push_back(newFunc);
                    }
                }
            }
        }
    }
}

void CallFilter::run(const MatchFinder::MatchResult &Result) {

    ASTContext *Context = Result.Context;
    SourceManager *SManager = Result.SourceManager;

    auto callee = Result.Nodes.getNodeAs<clang::CallExpr>("functionCalls");
	auto caller = Result.Nodes.getNodeAs<clang::FunctionDecl>("functionCallers");

	FullSourceLoc FullLocation = Context->getFullLoc(callee->getLocStart());
	if (FullLocation.isValid()) {
		StringRef filename = SManager->getFilename(FullLocation);
		std::string functionName = callee->getDirectCallee()->getNameAsString();
		if (isCPPFile(filename) && !callee->getDirectCallee()->isOverloadedOperator() && functionName.find("~")) {
			for (std::size_t i = 0; i < functions.size(); ++i) {
				if (functions[i].identifier == functionName) {
					for (size_t x = 0; x < functions.size(); ++x) {
						if (functions[x].identifier == caller->getNameAsString()) {
							functions[i].callers.push_back(functions[x]); // FIXME: Seems to be doubling calls, added removeDuplicates() as quick-fix.
						}
					}
				}
			}
		}
	}
}

class : public DiagnosticConsumer {
public:
    virtual bool IncludeInDiagnosticCounts() const {
        return false; // Enough of that "not-so-fatal" error garbage.
    }
} diagConsumer;

void getFunctions(MatcherType matcher) {
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

    MatchFinder Finder;

    if (matcher == declarations) {
        DeclFilter Filter;
        DeclarationMatcher functionMatcher = functionDecl(isDefinition(), unless(isExpansionInSystemHeader())).bind("functionDecls");
        Finder.addMatcher(functionMatcher, &Filter);
    } else {
        CallFilter Filter;
        StatementMatcher functionMatcher = callExpr(
            callee(functionDecl()),
			hasAncestor(functionDecl().bind("functionCallers")),
            isExpansionInMainFile(),
			unless(isExpansionInSystemHeader())
        ).bind("functionCalls");
        Finder.addMatcher(functionMatcher, &Filter);
    }

    Tool.setDiagnosticConsumer(&diagConsumer);
    Tool.run(newFrontendActionFactory(&Finder).get());
}

void removeDuplicates() {
	for (size_t i = 0; i < functions.size(); i++) {
		functions[i].callers.resize(functions[i].callers.size()/2);
	}
}

void printFunctions() {
	for (size_t i = 0; i < functions.size(); ++i) {
		llvm::outs() << "Showing information for function Index " << i << "\n"
		<< "	Name: " << functions[i].identifier << "\n"
		<< "	Declaration File: " << functions[i].declFile << "\n";
		for (size_t x = 0; x < functions[i].callers.size(); ++x) {
			llvm::outs() << "	Called by function " << functions[i].callers[x].identifier << "\n";
		}
	}
}
