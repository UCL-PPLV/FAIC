//
//  FAIC.cpp
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

#include "FSManager.cpp"
#include "FunctionParser.cpp"

// TODO: Ignore missing include paths.

using namespace std;
using namespace FileSystem;
using namespace FuncParser;

int main(int argc, char const *argv[]) {
    vector<string> files;
    getFilesFromPath(argv[1], files);

    vector<string> functionDecls;
    getFuncDecls(files, functionDecls);

    return 0;
}
