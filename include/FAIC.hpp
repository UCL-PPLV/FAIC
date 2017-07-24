//
//  FAIC.hpp
//  Function Analysis In Codebases
//
//  Created by Tiago Ferreira on 24/07/2017.
//  Copyright 2017 Tiago Ferreira
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
//  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#ifndef FAIC_INCLUDE
#define FAIC_INCLUDE

#include <string>
#include <vector>
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"

// MARK: FSManager

extern std::vector<std::string> files;
void getFilesFromPath(std::string rootPath, int &depth);

// MARK: FunctionParser

static llvm::cl::OptionCategory FAICCategory("FAIC Options");
static llvm::cl::extrahelp FAICHelp(clang::tooling::CommonOptionsParser::HelpMessage);
static llvm::cl::extrahelp MoreHelp("\nMore help text..."); // TODO: Actually add help text.

class DeclFilter : public clang::ast_matchers::MatchFinder::MatchCallback {
    public: virtual void run(const clang::ast_matchers::MatchFinder::MatchResult &Result);
};

class CallFilter : public clang::ast_matchers::MatchFinder::MatchCallback {
    public: virtual void run(const clang::ast_matchers::MatchFinder::MatchResult &Result);
};

struct Function {
       std::string identifier;
       std::string hexID;
       std::string declFile;
       int numberOfCalls;
       std::vector<std::string> callers;
       std::vector<std::string> callersFile;
};

extern std::vector<Function> functions;
enum MatcherType { declarations, calls };
void getFunctions(MatcherType matcher);

// MARK: StringToArgVC

bool _isQuote(char c);
bool _isEscape(char c);
bool _isEscape(char c);
bool _isWhitespace(char c);
std::vector<std::string> parse(const std::string& args);
void stringToArgcArgv(const std::string& str, int* argc, char*** argv);

#endif
