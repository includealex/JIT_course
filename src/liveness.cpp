#include "liveness.hpp"

namespace custom {

LiveInterval Liveness::run_analysis(Graph* graph) {
  set_bb_liveranges(graph);
  calc_live_ranges(graph);

  return _intervals;
}

void Liveness::calc_live_ranges(Graph* graph) {
  Linorder linorder;
  auto reversed_linorder = linorder.get_reversed_linorder(graph);

  for (auto& bb_id : reversed_linorder) {
    // TODO: apply some cringe here
    // here _intervals should be modified and applied changes
  }
}

void Liveness::set_bb_liveranges(Graph* graph) {
  Linorder linorder;
  auto linorder_real = linorder.get_linorder(graph);
  std::size_t cur_live = 0;
  std::size_t cur_lin = 0;

  for (auto& bb_id : linorder_real) {
    auto cur_block = graph->get_block(bb_id);
    cur_block->set_liverange_start(cur_live);

    auto cur_instr = cur_block->get_first_inst();
    if (cur_instr == nullptr) {
      cur_block->set_liverange_end(cur_block->get_liverange_start());
      continue;
    }

    if (cur_instr->getOpcode() != Opcode::PHI) {
      cur_live += 2;
    }
    cur_instr->lin = cur_lin;
    cur_instr->live_num = cur_live;
    cur_lin++;

    while (cur_instr->get_next() != nullptr) {
      cur_instr = cur_instr->get_next();
      if (cur_instr->getOpcode() != Opcode::PHI) {
        cur_live += 2;
      }
      cur_instr->lin = cur_lin;
      cur_instr->live_num = cur_live;
      cur_lin++;
    }

    cur_live += 2;
    cur_block->set_liverange_end(cur_live);
  }
}

}  // namespace custom