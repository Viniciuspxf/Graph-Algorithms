#include "asgt.h"

#include <utility>              // for std::get
#include <tuple>
#include <vector>

#define BOOST_ALLOW_DEPRECATED_HEADERS // silence warnings
#include <boost/graph/adjacency_list.hpp>
#include <boost/optional.hpp>

#include "cycle.h"
#include "digraph.h"
#include "potential.h"

/* The code in this template file is all "bogus". It just illustrates
 * how to return answers back to main.cpp. */

/* The following declarations shorten the bogus code below. Feel free
 * to change/drop them. */
using boost::add_edge;
using boost::num_vertices;
using boost::out_edges;
using std::vector;

Digraph build_digraph(const Digraph& market)
{
  Digraph digraph(num_vertices(market));

  Arc a0;

  for (const auto& edge : boost::make_iterator_range(edges(market))) {
    std::tie(a0, std::ignore) = add_edge(edge.m_source, edge.m_target, digraph);
    digraph[a0].cost = -log(market[edge].cost);
  }

  return digraph;
}

std::tuple<bool,
           boost::optional<NegativeCycle>,
           boost::optional<FeasiblePotential>>
has_negative_cycle(Digraph& digraph)
{
  vector<boost::detail::edge_desc_impl<boost::directed_tag, std::size_t>> arcs;

  for (size_t i = 0; i < num_vertices(digraph) - 1; i++) {
    for (const auto& arc : boost::make_iterator_range(edges(digraph))) {
      auto source = arc.m_source;
      auto target = arc.m_target;
      auto cost =  digraph[arc].cost;

      if (digraph[target].distance  > digraph[source].distance + cost) {
        digraph[target].distance = digraph[source].distance + cost;
        digraph[target].predecessor = source;
      }
   }
  }

  for (const auto& arc : boost::make_iterator_range(edges(digraph))) {
    auto source = arc.m_source;
    auto target = arc.m_target;
    auto cost =  digraph[arc].cost;

    if (digraph[target].distance  > digraph[source].distance + cost) {
      arcs.push_back(arc);
      digraph[target].is_in_vector = true;

      while (!digraph[source].is_in_vector) {
        digraph[source].is_in_vector = true;
        target = source;
        source = digraph[source].predecessor;
        arcs.push_back(boost::edge(source, target, digraph).first);
      }

      std::reverse(arcs.begin(), arcs.end());

      Walk walk(digraph, arcs[0].m_source);

      for (auto current_arc : arcs) {
        walk.extend(current_arc);

        if (current_arc.m_target == arcs[0].m_source)
          break;
      }

      return {true, NegativeCycle(walk), boost::none};
    }
  }

  

  vector<double> y;
  for (const auto& vertex : boost::make_iterator_range(vertices(digraph))) {
    y.push_back(digraph[vertex].distance);
  }

  return {false, boost::none, FeasiblePotential(digraph, y)};
}

Loophole build_loophole(const NegativeCycle& negcycle,
                        const Digraph& aux_digraph,
                        const Digraph& market)
{
  Walk w(market, negcycle.get()[0].m_source);  
  for (const auto arc: negcycle.get()) {
    w.extend(boost::edge(arc.m_source, arc.m_target, market).first);
  }
  return Loophole(w);
}

FeasibleMultiplier build_feasmult(const FeasiblePotential& feaspot,
                                  const Digraph& aux_digraph,
                                  const Digraph& market)
{
  vector<double> z;

  for (double value : feaspot.potential()) {
    z.push_back(exp(-value));
  }
  return FeasibleMultiplier(market, z);
}
