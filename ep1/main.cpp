#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <iostream>
#include <stack>
#define BOOST_ALLOW_DEPRECATED_HEADERS // silence warnings

typedef boost::adjacency_list<boost::vecS,
                              boost::vecS,
                              boost::directedS> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;

class HeadStart {
  public:
    HeadStart(int number_of_vertices, int number_of_arcs) : number_of_vertices(number_of_vertices), number_of_arcs(number_of_arcs) {
      is_in_stack = new bool[number_of_vertices];
      discovered =  new int[number_of_vertices];
      minimum_node = new int[number_of_vertices];
      order = new int[number_of_vertices];
      sccCounter = 1;
      
      for (int i = 0; i < number_of_vertices; i++) {
        is_in_stack[i] = false;
        discovered[i] = minimum_node[i] = -1;
      }
    }
    ~HeadStart() {
      delete [] minimum_node;
      delete [] discovered;
      delete [] is_in_stack;
      delete [] order;
    }

    std::stack<int> vertices_stack;
    int * minimum_node;
    int * discovered;
    bool *is_in_stack;
    int *order;
    int number_of_vertices;
    int number_of_arcs;
    int sccCounter;
};

int convertVertexToInteger(int vertex, int number_of_literals) {
  return vertex >= number_of_literals ? -vertex + number_of_literals - 1 : vertex + 1;
}

int convertToValidInteger(int integer, int number_of_literals) {
  return integer > 0 ? integer - 1 : -integer - 1 + number_of_literals;
}

void depthSearch(Graph& graph, int& counter, int vertex, HeadStart& data) {
  int current_vertex;
  boost::graph_traits<Graph>::out_edge_iterator vertex_it, vertex_end;
  
  data.discovered[vertex] = data.minimum_node[vertex] = ++counter;
  data.vertices_stack.push(vertex);
  data.is_in_stack[vertex] = true;

  for (std::tie(vertex_it, vertex_end) = boost::out_edges(vertex, graph);
    vertex_it != vertex_end; ++vertex_it) {

    current_vertex = boost::target(*vertex_it, graph);

    if (data.discovered[current_vertex] == -1)
      depthSearch(graph, counter, current_vertex, data);

    if (data.is_in_stack[current_vertex])
      data.minimum_node[vertex] = data.minimum_node[current_vertex] < data.minimum_node[vertex] ?  
                                  data.minimum_node[current_vertex] : data.minimum_node[vertex];

  }

  if (data.discovered[vertex] == data.minimum_node[vertex]) {
    while (data.vertices_stack.top() != vertex) {
      data.is_in_stack[data.vertices_stack.top()] = false;
      data.order[data.vertices_stack.top()] = data.sccCounter;
      data.vertices_stack.pop();
    }
    data.is_in_stack[data.vertices_stack.top()] = false;
    data.order[data.vertices_stack.top()] = data.sccCounter;
    data.vertices_stack.pop();
    data.sccCounter++;
  }
}

void reach_visit(Graph& graph, int origin, bool *visited, int * predecessor) {
  visited[origin] = true;
  int current_vertex;
  boost::graph_traits<Graph>::out_edge_iterator vertex_it, vertex_end;

  for (std::tie(vertex_it, vertex_end) = boost::out_edges(origin, graph);
    vertex_it != vertex_end; ++vertex_it) {

    current_vertex = boost::target(*vertex_it, graph);

    if (!visited[current_vertex]){
      predecessor[current_vertex] = origin;
      reach_visit(graph, current_vertex, visited, predecessor);
    }
  }
  
}

void print_graph(Graph graph, int number_of_vertices, int number_of_arcs) {
  int current_vertex;
  std::cout << number_of_vertices << " " <<  number_of_arcs << "\n";
  for (int vertex = 0; vertex < number_of_vertices; vertex++) {

    boost::graph_traits<Graph>::out_edge_iterator vertex_it, vertex_end;

    for (std::tie(vertex_it, vertex_end) = boost::out_edges(vertex, graph);
      vertex_it != vertex_end; ++vertex_it) {

      current_vertex = boost::target(*vertex_it, graph);
      std::cout << convertVertexToInteger(vertex, number_of_vertices / 2) << " " << convertVertexToInteger(current_vertex, number_of_vertices / 2) << "\n";
    }
  }
}

void build_path(int * predecessor, int origin, int destiny, int number_of_vertices) {
  int* path = new int[number_of_vertices];
  int path_size = 0;
  int current_vertex = destiny;

  while (current_vertex != origin) {
    path[path_size++] = current_vertex;
    current_vertex =  predecessor[current_vertex];
  }

  path[path_size++] = current_vertex;

  std::cout << path_size - 1 << " ";

  for (int i = path_size - 1; i > 0; i--) {
    std::cout << convertVertexToInteger(path[i], number_of_vertices/2) << " ";
  }
  std:: cout << convertVertexToInteger(path[0], number_of_vertices/2) << "\n"; 
  

  delete [] path;

}

void reach(Graph& graph, int origin, int destiny, int number_of_vertices) {
  bool *visited = new bool[number_of_vertices];
  int *predecessor = new int[number_of_vertices];

  for (int i = 0; i < number_of_vertices; i++) {
    visited[i] = false;
    predecessor[i] = -1;
  }

  predecessor[origin] = origin;
  reach_visit(graph, origin, visited, predecessor);
  build_path(predecessor, origin, destiny, number_of_vertices);

  delete [] visited;
  delete [] predecessor;
}

void print_true_assignments(HeadStart& data) {
  for (int i = 0; i < data.number_of_vertices / 2; i++)
    std::cout << (data.order[i] < data.order[i + data.number_of_vertices / 2] ) << " ";
}

void outputSolution(Graph& graph, HeadStart& data, int d) {
  int counter = 0;
  bool hasSolution = true;

  for (int current_vertex = 0; current_vertex < data.number_of_vertices; current_vertex++) {

    if (data.discovered[current_vertex] == -1)
      depthSearch(graph, counter, current_vertex, data);
  }
 
  if (d == 0) {
    for (int current_vertex = 0; current_vertex < data.number_of_vertices / 2; current_vertex++)
      if (data.order[current_vertex +  data.number_of_vertices/2] == data.order[current_vertex]) {
        hasSolution = false;
        std::cout << "NO\n" << current_vertex + 1 << "\n";
        reach(graph, current_vertex, current_vertex +  data.number_of_vertices/2, data.number_of_vertices);
        reach(graph, current_vertex +  data.number_of_vertices/2, current_vertex, data.number_of_vertices);
        break;
      }

    if (hasSolution) {
      std::cout << "YES\n";
      print_true_assignments(data);
    }
  }
  else if (d == 1) {
    for (int current_vertex = 0; current_vertex < data.number_of_vertices - 1; current_vertex++) 
      std::cout << data.order[current_vertex] << " ";

    std::cout << data.order[data.number_of_vertices - 1] << "\n";  
  }
  else
    print_graph(graph, data.number_of_vertices, data.number_of_arcs);
}

void read_graph(std::istream& in, int n, int m, int d) {
  int a, b;
  int number_of_vertices = 2*n;
  int number_of_clauses = m;
  HeadStart data(number_of_vertices, 2*number_of_clauses);
 
  Graph graph(number_of_vertices);

  while (number_of_clauses--) {
    in >> a >> b;
    boost::add_edge(convertToValidInteger(-a, n), convertToValidInteger(b, n), graph);
    boost::add_edge(convertToValidInteger(-b, n), convertToValidInteger(a, n), graph);
  }

  outputSolution(graph, data, d);
}

int main(int argc, char** argv) {
  unsigned long int d, n, m;
  std::cin >> d >> n >> m;
  
  read_graph(std::cin, n, m, d);

  return EXIT_SUCCESS;
}
