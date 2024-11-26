#include "liveness.hpp"

namespace custom {

void Liveness::set_live_ints(Graph* graph) {
  Linorder linorder;
  auto cur_order = linorder.get_linorder(graph);

  std::vector<BasicBlock*> bb_ptr_linorder;
  for (auto& el_id : cur_order) {
    bb_ptr_linorder.push_back(graph->get_block(el_id));
  }

  return;
}

}  // namespace custom
