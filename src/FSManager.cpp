//
//  FSManager.cpp
//  Function Analysis In Codebases
//
//  Created by Tiago Ferreira on 12/06/2017.
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

#define warnOnce if (depth == 0) cerr

#include <boost/algorithm/string/predicate.hpp>
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/progress.hpp"
#include <iostream>
#include <vector>

using namespace boost;
using namespace boost::filesystem;

using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using std::string;

namespace FileSystem {

	void getFilesFromPath(path rootPath, vector<string> &filesVector, const int depth = 0) {
		path p(current_path());
		p = system_complete(rootPath);

		try {
			if (!exists(p)) {
				warnOnce << "Invalid path: " << p
				<< "\nFile not found, make sure the file or directory exists." << endl;
		    } else {
				if (!is_directory(p)) {

					if (p.extension().string() == ".cpp") {
						filesVector.push_back(p.string());
					} else {
						warnOnce << "Invalid path: " << p
						<< "\nMust be a C++ file (.cpp extensions only)." << endl;
					}
				} else {
					if (boost::filesystem::is_empty(p)) {
						warnOnce << "Invalid path: " << p
						<< "\nThe directory is empty, no files to anaylse." << endl;
					} else {
						for (auto&& file : directory_iterator(p)) {
							if (!starts_with(file.path().filename().string(), ".")) {
								getFilesFromPath(system_complete(file.path()).string(), filesVector, depth+1);
							}
						}
					}
				}
			}
		} catch (const filesystem_error& ex) {
			cerr << ex.what() << endl;
		}
	}
}
