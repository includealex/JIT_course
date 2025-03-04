#ifndef INCLUDES_OPTIMIZER_HPP_
#define INCLUDES_OPTIMIZER_HPP_

#include <unordered_map>
#include <vector>

#include "basic_block.hpp"
#include "graph.hpp"
#include "instruction.hpp"
#include "ir_builder.hpp"
#include "rpo.hpp"

namespace custom {

class Optimizer {
 public:
  void constant_fold(Graph* graph);
};

}  // namespace custom

#endif  // INCLUDES_OPTIMIZER_HPP_
