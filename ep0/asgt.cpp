/* This is the main file that the students must work on to submit; the
 * other one is arb.h
 */

#include "asgt.h"

void depthSearch(Arb& arb, int& counter, int vertex, HeadStart& data) {
  boost::graph_traits<Arb>::out_edge_iterator vertex_it, vertex_end;
  data.set_in_time(vertex, counter);

  for (std::tie(vertex_it, vertex_end) = boost::out_edges(vertex, arb);
    vertex_it != vertex_end; ++vertex_it) {
    counter++;
    depthSearch(arb, counter, boost::target(*vertex_it, arb), data);
  }
  counter++;

  data.set_out_time(vertex, counter);
}

Arb read_arb(std::istream& in)
{
  int number_of_vertices;
  int i, j;
  in >> number_of_vertices;
  Arb arb(number_of_vertices);

  while (number_of_vertices-- - 1) {
    in >> i >> j;
    boost::add_edge(--i, --j, arb);
  }
  return arb;
}

HeadStart preprocess (Arb &arb, const Vertex& root)
{
  int counter = 0;
  HeadStart data(boost::num_vertices(arb));

  depthSearch(arb, counter, 0, data);
  return data;
}

bool is_ancestor (const Vertex& u, const Vertex& v, const HeadStart& data)
{
  if (data.get_in_time((int) u) <= data.get_out_time((int)v)
     && data.get_out_time((int) u) >= data.get_out_time((int)v))
    return true;
  return false;
}
