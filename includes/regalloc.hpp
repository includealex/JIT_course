#ifndef INCLUDES_REGALLOC_HPP_
#define INCLUDES_REGALLOC_HPP_

#include <map>
#include <string>
#include <variant>
#include <vector>

#include "liveness.hpp"

namespace custom {

class FloatRegalloc final {
 public:
  void set_reg_num(std::size_t reg_num) {
    _reg_num = reg_num;
  }

  std::map<std::size_t, std::variant<std::size_t, std::string>> run_analysis(Graph* graph);

 private:
  std::size_t _reg_num = 3;
  std::map<std::size_t, custom::LiveRange> _active_regs;
  std::vector<std::size_t> _free_registers;
  std::map<std::size_t, std::size_t> _allocated_registers;
  std::map<std::size_t, std::string> _stack_location;

  void ExpireOldIntervals(const std::pair<const std::size_t, custom::LiveRange>& interval_i);
  void SpillAllInterval(const std::pair<const std::size_t, custom::LiveRange>& interval_i);
  void AssignRegister(const std::pair<const std::size_t, custom::LiveRange>& interval_i);
  std::string AllocateStackLocation();
};

class IntRegalloc final {
 public:
  void set_reg_num(std::size_t reg_num) {
    _reg_num = reg_num;
  }

  std::map<std::size_t, std::variant<std::size_t, std::string>> run_analysis(Graph* graph);

 private:
  std::size_t _reg_num = 6;
  std::map<std::size_t, custom::LiveRange> _active_regs;
  std::vector<std::size_t> _free_registers;
  std::map<std::size_t, std::size_t> _allocated_registers;
  std::map<std::size_t, std::string> _stack_location;

  void ExpireOldIntervals(const std::pair<const std::size_t, custom::LiveRange>& interval_i);
  void SpillAllInterval(const std::pair<const std::size_t, custom::LiveRange>& interval_i);
  void AssignRegister(const std::pair<const std::size_t, custom::LiveRange>& interval_i);
  std::string AllocateStackLocation();
};

}  // namespace custom

#endif  // INCLUDES_REGALLOC_HPP_
