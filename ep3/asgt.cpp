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
  /* placeholder for NRVO */
  Digraph digraph(num_vertices(market));

  /* flip some signs in the arc costs below to exercise the many
   * execution pathways */
  Arc a0;
  /* create arcs 01 and 10 */
  // Arc a0, a1;
  // std::tie(a0, std::ignore) = add_edge(0, 1, digraph);
  // digraph[a0].cost = 11.0;
  // std::tie(a1, std::ignore) = add_edge(1, 0, digraph);
  // digraph[a1].cost = -17.0;

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
  digraph[0].distance = 0;
  vector<boost::detail::edge_desc_impl<boost::directed_tag, std::size_t>> arcs;

  for (size_t i = 0; i < num_vertices(digraph) - 1; i++) {
    for (const auto& arc : boost::make_iterator_range(edges(digraph))) {
      auto source = arc.m_source;
      auto target = arc.m_target;
      auto cost =  digraph[arc].cost;

      if (digraph[source].distance != INFINITY && digraph[target].distance  > digraph[source].distance + cost) {
        digraph[target].distance = digraph[source].distance + cost;
        digraph[target].predecessor = source;
      }
   }
  }

  for (const auto& arc : boost::make_iterator_range(edges(digraph))) {
    auto source = arc.m_source;
    auto target = arc.m_target;
    auto cost =  digraph[arc].cost;

    if (digraph[source].distance != INFINITY && digraph[target].distance  > digraph[source].distance + cost) {
      arcs.push_back(arc);
      digraph[source].is_in_vector = true;

      while (!digraph[digraph[source].predecessor].is_in_vector) {
        target = source;
        source = digraph[source].predecessor;
        arcs.push_back(boost::edge(source, target, digraph).first);
        digraph[source].is_in_vector = true;
      }

      std::reverse(arcs.begin(), arcs.end());

      Walk walk(digraph, arcs[0].m_source);

      for (auto current_arc : arcs) {
        walk.extend(current_arc);
      }

      return {true, NegativeCycle(walk), boost::none};
    }
  }
  return {false, boost::none, boost::none};



  // Walk walk(digraph, 0);
  // walk.extend(a0);
  // walk.extend(a1);

  /* Replace `NegativeCycle(walk)` with `boost::none` in the next
   * command to trigger "negative cycle reported but not computed".
   * Comment the whole `return` and uncomment the remaining lines to
   * exercise construction of a feasible potential. */

  // encourage RVO
  // return {true, NegativeCycle(walk), boost::none};

  /* Replace `FeasiblePotential(digraph, y)` with `boost::none` in the
   * next command to trigger "feasible potential reported but not
   * computed". */

  // encourage RVO
  vector<double> y(num_vertices(digraph), 0.0);
  return {false, boost::none, FeasiblePotential(digraph, y)};
}

Loophole build_loophole(const NegativeCycle& negcycle,
                        const Digraph& aux_digraph,
                        const Digraph& market)
{
  /* bogus code */
  Walk w(market, negcycle.get()[0].m_source);  
  for (const auto arc: negcycle.get()) {
    w.extend(boost::edge(arc.m_source, arc.m_target, market).first);
  }
  // encourage RVO
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

  // encourage RVO
  return FeasibleMultiplier(market, z);
}
