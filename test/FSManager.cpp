//
//  FSManager.cpp
//  Function Analysis In Codebases
//
//  Created by Tiago Ferreira on 09/07/2018.
//  Copyright 2018 Tiago Ferreira
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
//  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#include "boost/filesystem.hpp"
#include "FSManager.hpp"
#include "gtest/gtest.h"

using namespace boost::filesystem;

TEST(FSManager, isCPPFile) {
    EXPECT_TRUE(isCPPFile(system_complete("test.cpp")));
    EXPECT_TRUE(isCPPFile(system_complete("./this/etcEtc.cpp")));
    EXPECT_FALSE(isCPPFile(system_complete("notACPPFile.txt")));
    EXPECT_FALSE(isCPPFile(system_complete("noExtension")));
    EXPECT_FALSE(isCPPFile(system_complete("/thisIsJustAFolder/")));
}
