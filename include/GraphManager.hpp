//
//  GraphManager.hpp
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

#ifndef GRAPHMANAGER_INCLUDE
#define GRAPHMANAGER_INCLUDE

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>

using namespace boost;

typedef adjacency_list<listS, vecS, directedS> Graph;
typedef graph_traits<Graph>::vertex_descriptor VertexName;

extern Graph callGraph;
typedef typename graph_traits<Graph>::vertex_descriptor FunctionVertex;


void createGraph();
void populateGraph();
void printGraph();

#endif
