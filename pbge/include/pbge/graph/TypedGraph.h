
#ifndef PBGE_GRAPH_TYPEDGRAPH_H
#define PBGE_GRAPH_TYPEDGRAPH_H
#include <utility>
#include <cstddef>
#include <vector>
#include <list>
#include <algorithm>
#include <iterator>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>

namespace pbge {
    
    namespace graph_internal {
        typedef boost::adjacency_list<boost::vecS, boost::vecS> Graph;
        typedef std::pair<int, int> Edge;
    }

    template<class T>
    class Graph{
    public:
        typedef T vertex_type;
        Graph(size_t n) : numberOfVertices(n), graph(n) {
            vertices.reserve(n);
        }

        void addVertex(vertex_type v) {
            vertices.push_back(v);
        }
        
        void addEdge(vertex_type from, vertex_type to) {
            size_t v1 = std::distance(vertices.begin(), std::find(vertices.begin(), vertices.end(), from));
            size_t v2 = std::distance(vertices.begin(), std::find(vertices.begin(), vertices.end(), to));
            boost::add_edge(v1, v2, graph);
        }

        std::vector<vertex_type> topologicalSort() {
            std::list<graph_internal::Graph::vertex_descriptor> ordering;
            std::list<graph_internal::Graph::vertex_descriptor>::iterator it;
            std::vector<vertex_type> ordered;
            boost::topological_sort(graph, std::front_inserter(ordering));

            for(it = ordering.begin(); it != ordering.end(); it++) {
                ordered.push_back(vertices.at(*it));
            }
            return ordered;
        }
    private:
        std::vector<vertex_type> vertices;
        size_t numberOfVertices;
        graph_internal::Graph graph;
    };
}

#endif