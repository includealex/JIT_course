#ifndef INCLUDES_REGALLOC_HPP_
#define INCLUDES_REGALLOC_HPP_

#include <map>
#include <vector>

#include "liveness.hpp"

namespace custom {

class Regalloc final {
 public:
  void set_reg_num(std::size_t reg_num) {
    _reg_num = reg_num;
  }
  void run_analysis(Graph* graph);

 private:
  std::size_t _reg_num = 3;
  std::map<std::size_t, custom::LiveRange> _active_regs;
  std::vector<std::size_t> _free_registers;
  std::map<std::size_t, std::size_t> _allocated_registers;
  std::map<std::size_t, std::size_t> _stack_location;

  void ExpireOldIntervals(const std::pair<const std::size_t, custom::LiveRange>& interval_i);
  void SpillAllInterval(const std::pair<const std::size_t, custom::LiveRange>& interval_i);
  void AssignRegister(const std::pair<const std::size_t, custom::LiveRange>& interval_i);
  std::size_t AllocateStackLocation();
};

}  // namespace custom

#endif  // INCLUDES_REGALLOC_HPP_
