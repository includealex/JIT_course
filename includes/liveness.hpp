#ifndef INCLUDES_LIVENESS_HPP_
#define INCLUDES_LIVENESS_HPP_

#include "linorder.hpp"

namespace custom {

class Liveness final {
 public:
  void set_live_ints(Graph* graph);

 private:
  std::size_t instr_live_max;
  std::size_t instr_lin_max;
};

}  // namespace custom

#endif  // INCLUDES_LIVENESS_HPP_