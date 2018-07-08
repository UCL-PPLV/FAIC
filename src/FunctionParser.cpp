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
#include "FunctionParser.hpp"
#include "StringToArgVC.hpp"
#include "EasyLogging++.hpp"
#include "FSManager.hpp"
#include <iostream>
#include <sstream>

using namespace clang::ast_matchers;
using namespace clang::tooling;
using namespace clang;
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
            if (E->hasBody() && !E->isOverloadedOperator() && functionName.find("~") && functionName != "") { // Ignore operator overloaders and destructors
                bool functionIsPresent = false;
                for(std::size_t i = 0; i < functions.size(); ++i) {
                    if (functions[i].name == functionName) {
                        functionIsPresent = true;
                    }
                }
                if (!functionIsPresent) {
                    Function newFunc;
                    newFunc.UID = functions.size();
                    newFunc.name = functionName;
                    newFunc.declFile = filename;
                    functions.push_back(newFunc);
                }
            }
        }
    }
}

void CallFilter::run(const MatchFinder::MatchResult &Result) {
    ASTContext *Context = Result.Context;

    auto callee = Result.Nodes.getNodeAs<clang::CallExpr>("functionCalls");
    auto caller = Result.Nodes.getNodeAs<clang::FunctionDecl>("functionCallers");

    FullSourceLoc FullLocation = Context->getFullLoc(callee->getLocStart());
    if (FullLocation.isValid()) {
        std::string functionName = callee->getDirectCallee()->getNameAsString();
        if (!callee->getDirectCallee()->isOverloadedOperator() && functionName.find("~") && functionName != "") {
            for (std::size_t i = 0; i < functions.size(); ++i) {
                if (functions[i].name == functionName) {
                    for (size_t x = 0; x < functions.size(); ++x) {
                        if (functions[x].name == caller->getNameAsString()) {
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

CommonOptionsParser filesToOptionsParser(vector<string> files) {
    std::stringstream filesStringStream; // Synthesise CLI entry from files vector.
    filesStringStream << "FAIC ";
    for(size_t i = 0; i < files.size(); ++i) {
        filesStringStream << " ";
        filesStringStream << files[i];
    }
    filesStringStream << " --";
    
    int filesArgc;
    char ** varFilesArgv;
    
    stringToArgcArgv(filesStringStream.str(), &filesArgc, &varFilesArgv);
    const char ** filesArgv = const_cast<const char**>(varFilesArgv);
    CommonOptionsParser OptionsParser(filesArgc, filesArgv, FAICCategory);
    return OptionsParser;
}

void getFunctions(vector<string> files, MatcherType matcher) {
    CommonOptionsParser OptionsParser = filesToOptionsParser(files);
    ClangTool Tool(OptionsParser.getCompilations(), OptionsParser.getSourcePathList());

    MatchFinder Finder;

    if (matcher == declarations) {
        DeclFilter Filter;
        DeclarationMatcher functionMatcher = functionDecl(isDefinition(),
                unless(isExpansionInSystemHeader())).bind("functionDecls");
        Finder.addMatcher(functionMatcher, &Filter);
    } else {
        CallFilter Filter;
        StatementMatcher functionMatcher = callExpr(
                callee(functionDecl()), hasAncestor(functionDecl().bind("functionCallers")),
                isExpansionInMainFile(), unless(isExpansionInSystemHeader())).bind("functionCalls");
        Finder.addMatcher(functionMatcher, &Filter);
    }

    Tool.setDiagnosticConsumer(&diagConsumer);
    Tool.run(newFrontendActionFactory(&Finder).get());
}

void cleanup() {
    for (size_t i = 0; i < functions.size(); i++) {
        functions[i].callers.resize(functions[i].callers.size()/2);
    }
}

void printFunctions() {
    for (size_t i = 0; i < functions.size(); ++i) {
        LOG(DEBUG) << "SHOWING INFO FROM VECTOR INDEX " << i << "\n"
            << "    UID: " << functions[i].UID << "\n"
            << "    Name: " << functions[i].name << "\n"
            << "    Declaration File: " << functions[i].declFile;
        for (size_t x = 0; x < functions[i].callers.size(); ++x) {
            llvm::outs() << "        Called by function " << functions[i].callers[x].name
                << " (" << functions[i].callers[x].UID <<") \n";
        }
    }
}
