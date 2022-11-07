#include "asgt.h"
#include <stack>
#include <utility>
#include <iostream>

class HeadStart {
  public:
    HeadStart(int number_of_vertices) {
      this->number_of_vertices = number_of_vertices;
      bcc_counter = 1;

      discovered =  new int[number_of_vertices];
      minimum_node = new int[number_of_vertices];
      parent = new int[number_of_vertices];     
      children = new int[number_of_vertices]; 
    
      for (int i = 0; i < number_of_vertices; i++) {
        parent[i] = discovered[i] = minimum_node[i] = -1;
        children[i] = 0;
      }
    }

    ~HeadStart() {
      delete [] minimum_node;
      delete [] discovered;
      delete [] parent;
      delete [] children;
    }

    std::stack<boost::detail::edge_desc_impl<boost::undirected_tag, std::size_t>> edges_stack;
    int number_of_vertices;
    int * minimum_node;
    int * discovered;
    int *parent;
    int *children;
    int bcc_counter;

};


void depthSearch(Graph& graph, int& counter, int vertex, HeadStart& data) {
  int current_vertex;
  int visitCounter = 0;
  
  data.discovered[vertex] = data.minimum_node[vertex] = ++counter;


  for (const auto& edge : boost::make_iterator_range(boost::out_edges(vertex, graph))) {

    current_vertex = edge.m_target;

    if (data.discovered[current_vertex] == -1) {
      visitCounter++;
      data.edges_stack.push(edge);
      data.parent[current_vertex] = vertex;
      depthSearch(graph, counter, current_vertex, data);

      data.minimum_node[vertex] = data.minimum_node[current_vertex] < data.minimum_node[vertex] ?  
                                  data.minimum_node[current_vertex] : data.minimum_node[vertex];


      if ((data.discovered[vertex] == 1 && visitCounter > 1) || (data.discovered[vertex] > 1 && data.minimum_node[current_vertex] >= data.discovered[vertex])) {
        graph[vertex].cutvertex = true;

        while (data.edges_stack.top().m_source != vertex || data.edges_stack.top().m_target != current_vertex) {
            graph[data.edges_stack.top()].bcc = data.bcc_counter;
            data.edges_stack.pop();
        }

        graph[data.edges_stack.top()].bcc = data.bcc_counter;
        data.edges_stack.pop();
        data.bcc_counter++;
      }


    }
    else if (current_vertex != data.parent[vertex]) {
      data.minimum_node[vertex] = data.discovered[current_vertex] < data.minimum_node[vertex] ?  
                                  data.discovered[current_vertex] : data.minimum_node[vertex];
      if (data.discovered[current_vertex] < data.discovered[vertex]) {
        data.edges_stack.push(edge);        
      }
    }

  }

  data.children[vertex] = visitCounter;
}

void compute_bcc (Graph &g, bool fill_cutvxs, bool fill_bridges)
{

  HeadStart headStart(boost::num_vertices(g));
  int counter = 0;

  for (const auto& vertex : boost::make_iterator_range(boost::vertices(g))) {
    g[vertex].cutvertex = false;
  }
  
  for (const auto& edge : boost::make_iterator_range(boost::edges(g))) {
    g[edge].bcc = 0;
    g[edge].bridge = false;
  }

  for (const auto& vertex : boost::make_iterator_range(boost::vertices(g))) {
    if (headStart.discovered[vertex] == -1)
      depthSearch(g, counter, vertex, headStart);

    bool stackIsNotEmpty = false;
    while (!headStart.edges_stack.empty()) {
        stackIsNotEmpty = true;

        g[headStart.edges_stack.top()].bcc = headStart.bcc_counter;
        headStart.edges_stack.pop();
    }
    if (stackIsNotEmpty) {
        headStart.bcc_counter++;
    }
  }

  for (const auto& edge : boost::make_iterator_range(boost::edges(g))) {
    g[edge].bridge = headStart.discovered[edge.m_source] > headStart.discovered[edge.m_target] ?  false :
      headStart.minimum_node[edge.m_target] > headStart.discovered[edge.m_source];
  }

  for (const auto& vertex : boost::make_iterator_range(boost::vertices(g))) {
    g[vertex].cutvertex = headStart.parent[vertex] == -1 ? headStart.children[vertex] >= 2 : 
      g[vertex].cutvertex;
  }
}
