//
//  GraphManager.cpp
//  Function Analysis In Codebases
//
//  Created by Tiago Ferreira on 04/09/2017.
//  Copyright 2017 Tiago Ferreira
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
//  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/graphviz.hpp>
#include "FunctionParser.hpp"
#include "GraphManager.hpp"
#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

using namespace boost;

Graph callGraph;

void createGraph() {
    for (size_t i = 0; i < functions.size(); ++i) {
        for (size_t x = 0; x < functions[i].callers.size(); ++x) {
            add_edge(functions[i].callers[x].UID, functions[i].UID, callGraph);
        }
    }
}

void populateGraph() {

}

void printGraph() {
    boost::write_graphviz(std::cout, callGraph);
}
