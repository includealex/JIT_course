#ifndef INCLUDES_REGALLOC_HPP_
#define INCLUDES_REGALLOC_HPP_

#include "liveness.hpp"


namespace custom {

class Regalloc final {
 public:
  void set_reg_num(std::size_t reg_num) { _reg_num = reg_num;}
  void run_analysis(Graph* graph);


 private:
  std::size_t _reg_num = 3;
  std::vector<std::pair<const std::size_t, custom::LiveRange>> _active_regs;
  
  void ExpireOldIntervals(std::pair<const std::size_t, custom::LiveRange> interval_i);
  void SpillAllInterval(std::pair<const std::size_t, custom::LiveRange> interval_i);
};

} // namespace custom

#endif //INCLUDES_REGALLOC_HPP_
