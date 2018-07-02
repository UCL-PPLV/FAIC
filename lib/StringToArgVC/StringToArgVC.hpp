//
//  StringToArgVC.hpp
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

#ifndef STRINGTOARGVC_INCLUDE
#define STRINGTOARGVC_INCLUDE

#include <string>
#include <vector>

bool _isQuote(char c);
bool _isEscape(char c);
bool _isEscape(char c);
bool _isWhitespace(char c);
std::vector<std::string> parse(const std::string& args);
void stringToArgcArgv(const std::string& str, int* argc, char*** argv);

#endif
