//
//  Clang.cpp
//  Function Analysis In Codebases
//
//  Created by Tiago Ferreira on 08/07/2018.
//  Copyright 2018 Tiago Ferreira
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
#include "clang/Tooling/Tooling.h"
#include "gtest/gtest.h"

using namespace llvm;
using namespace clang;
using namespace clang::tooling;
using namespace clang::ast_matchers;

class DumpCallback : public MatchFinder::MatchCallback {
    virtual void run(const MatchFinder::MatchResult &Result) { }
};

TEST(Dependencies, ClangAST) {
    DumpCallback Callback;
    MatchFinder Finder;
    Finder.addMatcher(recordDecl().bind("x"), &Callback);
    std::unique_ptr<FrontendActionFactory> Factory(newFrontendActionFactory(&Finder));
    EXPECT_TRUE(clang::tooling::runToolOnCode(Factory->create(), "class X {};"));
}
