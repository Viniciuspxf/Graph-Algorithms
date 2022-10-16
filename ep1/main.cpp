#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <iostream>
#include <stack>
#define BOOST_ALLOW_DEPRECATED_HEADERS // silence warnings

typedef boost::adjacency_list<boost::vecS,
                              boost::vecS,
                              boost::directedS> Arb;
typedef boost::graph_traits<Arb>::vertex_descriptor Vertex;


class HeadStart {
  public:
    HeadStart(int number_of_vertices) : number_of_vertices(number_of_vertices) {
      is_in_stack = new bool[number_of_vertices];
      discovered =  new int[number_of_vertices];
      minimum_node = new int[number_of_vertices];
      sccCounter = 0;
      
      for (int i = 0; i < number_of_vertices; i++) {
        is_in_stack[i] = false;
        discovered[i] = minimum_node[i] = -1;
      }
    }
    ~HeadStart() {
      delete [] status;
      delete [] minimum_node;
      delete [] discovered;
      delete [] is_in_stack;
    }

    std::stack<int> vertices_stack;
    int * status;
    int * minimum_node;
    int * discovered;
    bool *is_in_stack;
    int number_of_vertices;
    int sccCounter;
};


void depthSearch(Arb& arb, int& counter, int vertex, HeadStart& data) {
  int current_vertex;
  boost::graph_traits<Arb>::out_edge_iterator vertex_it, vertex_end;
  
  data.discovered[vertex] = data.minimum_node[vertex] = ++counter;
  data.vertices_stack.push(vertex);
  data.is_in_stack[vertex] = true;

  

  for (std::tie(vertex_it, vertex_end) = boost::out_edges(vertex, arb);
    vertex_it != vertex_end; ++vertex_it) {

    current_vertex = boost::target(*vertex_it, arb);

    if (data.discovered[current_vertex] == -1)
      depthSearch(arb, counter, current_vertex, data);

    else if (data.is_in_stack[current_vertex])
      data.minimum_node[vertex] = data.minimum_node[current_vertex] < data.minimum_node[vertex] ?  
                                  data.minimum_node[current_vertex] : data.minimum_node[vertex];

  }

  if (data.discovered[vertex] == data.minimum_node[vertex]) {
    while (data.vertices_stack.top() != vertex) {
      data.is_in_stack[data.vertices_stack.top()] = false;
      data.vertices_stack.pop();
    }
    data.is_in_stack[data.vertices_stack.top()] = false;
    data.vertices_stack.pop();
    data.sccCounter++;
  }
}

bool hasSolution(Arb& arb, HeadStart& data) {
  int counter = 0;

  for (int current_vertex = 0; current_vertex < data.number_of_vertices; current_vertex++) {

    if (data.discovered[current_vertex] == -1)
      depthSearch(arb, counter, current_vertex, data);
  }

  for (int current_vertex = 0; current_vertex < data.number_of_vertices / 2; current_vertex++)
    if (data.minimum_node[current_vertex +  data.number_of_vertices] == data.minimum_node[current_vertex])
      return false;

  return true;
}

int convertToValidInteger(int integer, int number_of_literals) {
  return integer > 0 ? integer - 1 : -integer - 1 + number_of_literals;
}

void read_arb(std::istream& in, int n, int m) {
  int a, b;
  int number_of_vertices = 2*n;
  int number_of_clauses = m;
  HeadStart data(number_of_clauses);
 
  Arb arb(number_of_vertices);

  while (number_of_clauses--) {
    in >> a >> b;
    boost::add_edge(convertToValidInteger(-a, n), convertToValidInteger(b, n), arb);
    boost::add_edge(convertToValidInteger(-b, n), convertToValidInteger(a, n), arb);
  }

  std::cout << hasSolution(arb, data);
}


int main (int argc, char** argv) {
  unsigned long int d, n, m;
  std::cin >> d >> n >> m;
  
  read_arb(std::cin, n, m);

  return EXIT_SUCCESS;
}
