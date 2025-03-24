#ifndef INCLUDES_OPTIMIZER_HPP_
#define INCLUDES_OPTIMIZER_HPP_

#include <unordered_map>
#include <vector>

#include "basic_block.hpp"
#include "graph.hpp"
#include "instruction_ext.hpp"
#include "ir_builder.hpp"
#include "rpo.hpp"

namespace custom {

class Optimizer {
 public:
  void constant_fold(Graph* graph, IRBuilder* builder);
  void peephole(Graph* graph, IRBuilder* builder);

 private:
  void replace_movi_with_constants(BasicBlock* block);
  void remove_unused_movi(BasicBlock* block);
  void replace_arith(BasicBlock* block);
};

}  // namespace custom

#endif  // INCLUDES_OPTIMIZER_HPP_
