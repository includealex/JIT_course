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
  std::map<std::size_t, std::size_t> _helper_intervals;
  void calc_live_ranges(Graph* graph);
  void set_bb_liveranges(Graph* graph);
  void fix_inervals_with_late_starts();
};

}  // namespace custom

#endif  // INCLUDES_LIVENESS_HPP_