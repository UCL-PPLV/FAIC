//
//  FunctionParser.hpp
//  Function Analysis In Codebases
//
//  Created by Tiago Ferreira on 11/10/2017.
//  Copyright 2017 Tiago Ferreira
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
//  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#ifndef FUNCTIONPARSER_INCLUDE
#define FUNCTIONPARSER_INCLUDE

#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "FSManager.hpp"
#include <string>
#include <vector>

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
	size_t UID;
	std::string name;
	std::string declFile;
	std::vector<Function> callers;

	bool operator == (const Function& other) const {
		return std::tie(this->UID, this->name, this->declFile, this->callers) ==
		std::tie(other.UID, other.name, other.declFile, other.callers);
	}
};

extern std::vector<Function> functions;
enum MatcherType { declarations, calls };

clang::tooling::CommonOptionsParser filesToOptionsParser(std::vector<std::string> files);
void getFunctions(std::vector<std::string> files, MatcherType matcher);
void cleanup();
void printFunctions();

#endif
