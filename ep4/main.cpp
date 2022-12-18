#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <iostream>
#include <stack>
#define BOOST_ALLOW_DEPRECATED_HEADERS // silence warnings

typedef boost::adjacency_list<boost::vecS,
                              boost::vecS,
                              boost::directedS> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;


int main() {
  return 0;
}