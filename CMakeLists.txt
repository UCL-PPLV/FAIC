##
##  CMakeLists.txt
##  Function Analysis In Codebases
##
##  Created by Tiago Ferreira on 08/11/2017.
##  Copyright 2017 Tiago Ferreira
##
##  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
##  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
##  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
##  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
##  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
##  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
##

PROJECT("FAIC")
SET(FAIC_VERSION_MAJOR 0)
SET(FAIC_VERSION_MINOR 1)


## MARK: CMake Settings

CMAKE_MINIMUM_REQUIRED(VERSION 3.0.2)
INCLUDE(FindPackageHandleStandardArgs)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS, 1)
ENABLE_TESTING()

## FAIC Sourcecode

SET(SRC
    "src/FAIC.cpp"
    "src/FSManager.cpp"
    "src/FunctionParser.cpp"
    "src/GraphManager.cpp"
)

ADD_DEFINITIONS("-std=c++14")
INCLUDE_DIRECTORIES(include)

## MARK: LLVM Search Paths

EXECUTE_PROCESS(COMMAND llvm-config --prefix OUTPUT_VARIABLE LLVM_PATH)
string(REGEX REPLACE "\n$" "" LLVM_PATH "${LLVM_PATH}")
MESSAGE(STATUS "Using LLVM from: ${LLVM_PATH}")
LINK_DIRECTORIES("${LLVM_PATH}/lib")
INCLUDE_DIRECTORIES("${LLVM_PATH}/include")

ADD_EXECUTABLE(${PROJECT_NAME} ${SRC})

## MARK: Boost Libraries

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

FIND_PACKAGE(Boost REQUIRED system filesystem)
IF(Boost_FOUND)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${Boost_LIBRARIES})
ENDIF()

## MARK: LLVM Clang Libraries

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
    "clangAST"
    "clangASTMatchers"
    "clangAnalysis"
    "clangBasic"
    "clangDriver"
    "clangEdit"
    "clangFrontend"
    "clangFrontendTool"
    "clangLex"
    "clangParse"
    "clangSema"
    "clangEdit"
    "clangRewrite"
    "clangRewriteFrontend"
    "clangStaticAnalyzerFrontend"
    "clangStaticAnalyzerCheckers"
    "clangStaticAnalyzerCore"
    "clangSerialization"
    "clangToolingCore"
    "clangTooling"
    "clangFormat"
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
    "LLVMLTO"
    "LLVMPasses"
    "LLVMObjCARCOpts"
    "LLVMSymbolize"
    "LLVMDebugInfoPDB"
    "LLVMDebugInfoDWARF"
    "LLVMMIRParser"
    "LLVMCoverage"
    "LLVMTableGen"
    "LLVMDlltoolDriver"
    "LLVMOrcJIT"
    "LLVMObjectYAML"
    "LLVMLibDriver"
    "LLVMOption"
    "LLVMGlobalISel"
    "LLVMSelectionDAG"
    "LLVMAsmPrinter"
    "LLVMDebugInfoCodeView"
    "LLVMDebugInfoMSF"
    "LLVMMCDisassembler"
    "LLVMMCJIT"
    "LLVMLineEditor"
    "LLVMInterpreter"
    "LLVMExecutionEngine"
    "LLVMRuntimeDyld"
    "LLVMCodeGen"
    "LLVMTarget"
    "LLVMCoroutines"
    "LLVMipo"
    "LLVMInstrumentation"
    "LLVMVectorize"
    "LLVMScalarOpts"
    "LLVMLinker"
    "LLVMIRReader"
    "LLVMAsmParser"
    "LLVMInstCombine"
    "LLVMTransformUtils"
    "LLVMBitWriter"
    "LLVMAnalysis"
    "LLVMProfileData"
    "LLVMObject"
    "LLVMMCParser"
    "LLVMMC"
    "LLVMBitReader"
    "LLVMCore"
    "LLVMBinaryFormat"
    "LLVMSupport"
    "LLVMDemangle"
)

include(FindCurses)
TARGET_LINK_LIBRARIES(${PROJECT_NAME}
    "pthread"
    "z"
    "dl"
    ${CURSES_LIBRARIES}
)

## MARK: EasyLogging++

INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/lib/Easylogging++")
ADD_SUBDIRECTORY(lib/EasyLogging++)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} EasyLogging++)

## MARK: StringToArgVC

INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/lib/StringToArgVC")
ADD_SUBDIRECTORY(lib/StringToArgVC)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} StringToArgVC)

## MARK: googletest

INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/lib/googletest")
ADD_SUBDIRECTORY(lib/googletest)

## Testing

SET(FAIC_TESTS_NAME "FAICUnitTests")
SET(TESTS
    "test/Clang.cpp"
)
ADD_EXECUTABLE(${FAIC_TESTS_NAME} ${TESTS})
TARGET_LINK_LIBRARIES(${FAIC_TESTS_NAME} gtest_main)

ADD_TEST(DryRun FAIC ${CMAKE_SOURCE_DIR}/test/test.cpp)
ADD_TEST(ClangTests ${FAIC_TESTS_NAME})

