//
//  FSManager.cpp
//  Function Analysis In Codebases
//
//  Created by Tiago Ferreira on 12/07/2017.
//  Copyright 2017 Tiago Ferreira
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
//  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#define warnOnce if (depth == 0) llvm::outs()

#include <boost/algorithm/string/predicate.hpp>
#include "clang/Frontend/CompilerInstance.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/FrontendAction.h"
#include "llvm/Support/CommandLine.h"
#include "clang/Tooling/Tooling.h"
#include "clang/AST/ASTConsumer.h"
#include "boost/filesystem.hpp"
#include "FSManager.hpp"
#include <string.h>
#include <vector>

using namespace boost::filesystem;
using namespace clang;
using namespace std;

bool isCPPFile(path filePath) {
    return strcmp(filePath.extension().c_str(), ".cpp") == 0;
}

void getFilesFromPathRecursive(vector<string>& files, string rootPath, int& depth) {
    path p(current_path());
    p = system_complete(rootPath);

    try {
        if (exists(p)) {
            if (!is_directory(p)) {
                if (isCPPFile(p)) {
                    files.push_back(p.string());
                } else {
                    warnOnce << "Invalid path: " << p.string()
                    << "\nMust be a C++ file (.cpp extensions only). \n";
                }
            } else if (boost::filesystem::is_empty(p)) {
                warnOnce << "Invalid path: " << p.string()
                << "\nThe directory is empty, no files to anaylse. \n";
            } else {
                for (auto&& file : directory_iterator(p)) {
                    if (!boost::starts_with(file.path().filename().string(), ".")) {
                        getFilesFromPathRecursive(files, system_complete(file.path()).string(), ++depth);
                    }
                }
            }
        }
    } catch (const filesystem_error& ex) {
        llvm::outs() << ex.what() << "\n";
    }
}

vector<string> filesFromPath(string rootPath) {
    int depth = 0;
    vector<string> files;
    getFilesFromPathRecursive(files, rootPath, depth);
    return files;
}
