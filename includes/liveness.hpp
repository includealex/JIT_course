#ifndef INCLUDES_LIVENESS_HPP_
#define INCLUDES_LIVENESS_HPP_

#include <algorithm>
#include <vector>

#include "linorder.hpp"

namespace custom {

class Liveness {
 public:
  LiveInterval run_analysis(Graph* graph);

 private:
  LiveInterval _intervals;
  void calc_live_ranges(Graph* graph);
  void set_bb_liveranges(Graph* graph);
};

}  // namespace custom

#endif  // INCLUDES_LIVENESS_HPP_