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

#define BOOST_FILESYSTEM_VERSION 3

#ifndef BOOST_FILESYSTEM_NO_DEPRECATED
#  define BOOST_FILESYSTEM_NO_DEPRECATED
#endif
#ifndef BOOST_SYSTEM_NO_DEPRECATED
#  define BOOST_SYSTEM_NO_DEPRECATED
#endif

#define warnOnce if (depth == 0) LOG(FATAL)

#include <boost/algorithm/string/predicate.hpp>
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/progress.hpp"
#include "EasyLogging++.hpp"
#include "FSManager.hpp"
#include <iostream>
#include <vector>

using namespace boost;
using namespace boost::filesystem;
using namespace std;

vector<string> files;

bool isCPPFile(string filePath) {
    path p(current_path());
    p = system_complete(filePath);
    return p.extension().string() == ".cpp";
}

void getFilesFromPath(string rootPath, int &depth) {
    path p(current_path());
    p = system_complete(rootPath);

    try {
        if (!exists(p)) {
            warnOnce << "Invalid path: " << p;
            warnOnce << "\nFile not found, make sure the file or directory exists." << endl;
        } else if (!is_directory(p)) {
            if (isCPPFile(p.filename().string())) {
                files.push_back(p.string());
            } else {
                warnOnce << "Invalid path: " << p
                << "\nMust be a C++ file (.cpp extensions only)." << endl;
            }
        } else if (boost::filesystem::is_empty(p)) {
            warnOnce << "Invalid path: " << p
            << "\nThe directory is empty, no files to anaylse." << endl;
        } else {
            for (auto&& file : directory_iterator(p)) {
                if (!starts_with(file.path().filename().string(), ".")) {
                    getFilesFromPath(system_complete(file.path()).string(), ++depth);
                }
            }
        }
    } catch (const filesystem_error& ex) {
        cerr << ex.what() << endl;
    }
}
