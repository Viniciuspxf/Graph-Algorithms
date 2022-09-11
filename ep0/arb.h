/* the definition of HeadStart may be changed in this file; the rest
 * MUST NOT be changed
 */

#ifndef ARB_H
#define ARB_H

#define BOOST_ALLOW_DEPRECATED_HEADERS // silence warnings
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

typedef boost::adjacency_list<boost::vecS,
                              boost::vecS,
                              boost::directedS> Arb;
typedef boost::graph_traits<Arb>::vertex_descriptor Vertex;

/* Students must adapt this class for their needs; the innards of
   these objects are only accessed by the student's code */
class HeadStart {
public:
  HeadStart(int number_of_vertices) : number_of_vertices(number_of_vertices) {
    in_time =  new int[number_of_vertices];
    out_time = new int[number_of_vertices];

    for (int i = 0; i < number_of_vertices; i++)
      in_time[i] = out_time[i] = 0;
  }

  void set_in_time(int vertex, int time) {
    in_time[vertex] = time;
  }

  void set_out_time(int vertex, int time) {
    out_time[vertex] = time;
  }

  int get_in_time(int vertex) const {
    return in_time[vertex];
  }

  int get_out_time(int vertex) const {
    return out_time[vertex];
  }

private:
  int * in_time;
  int * out_time;
  int number_of_vertices;
};

#endif // #ifndef ARB_H
