#include "liveness.hpp"

namespace custom {

LiveInterval Liveness::run_analysis(Graph* graph) {
  set_bb_liveranges(graph);
  calc_live_ranges(graph);

  return _intervals;
}

void Liveness::calc_live_ranges(Graph* graph) {
  LoopTree lt;
  lt.build_tree(graph);

  auto preloop_vec = lt.loop_vector;
  std::vector<custom::LTNode*> loop_vec;
  for (auto& el : preloop_vec) {
    if (el->idx == -1)
      continue;
    loop_vec.push_back(el);
  }

  std::vector<size_t> headers_ids;
  for (auto& loop : loop_vec) {
    headers_ids.push_back(loop->get_header_id());
  }

  Linorder linorder;
  auto reversed_linorder = linorder.get_reversed_linorder(graph);

  for (auto& bb_id : reversed_linorder) {
    auto cur_block = graph->get_block(bb_id);

    LiveInterval live;

    // live = union of successor.liveIn for each successor of b
    for (auto& tmp_succ : {cur_block->get_succs_true(), cur_block->get_succs_false()}) {
      if (tmp_succ == nullptr) {
        continue;
      }

      live = {live, tmp_succ->get_liveIn()};
    }

    // for each phi function phi of successors of b do
    //    live.add(phi.inputOf(b))
    std::set<std::size_t> cur_dest_regs;
    auto cur_instr = cur_block->get_first_inst();
    while (cur_instr != nullptr) {
      for (auto& el : cur_instr->getDestRegs()) {
        cur_dest_regs.insert(el);
      }

      cur_instr = cur_instr->get_next();
    }

    for (auto& tmp_succ : {cur_block->get_succs_true(), cur_block->get_succs_false()}) {
      if (tmp_succ == nullptr) {
        continue;
      }
      auto cur_instr = tmp_succ->get_first_inst();
      while (cur_instr != nullptr) {
        if (cur_instr->getOpcode() == Opcode::PHI) {
          for (auto& el : cur_instr->getSrcRegs()) {
            if (cur_dest_regs.count(el)) {
              live.add_empty(el, cur_instr->live_num);
            }
          }
        }
        cur_instr = cur_instr->get_next();
      }
    }

    // for each opd in live do
    //    intervals[opd].addRange(b.from, b.to)
    auto bb_start_end = cur_block->get_liverange();
    for (auto const& [key, _] : live.get_liveIn()) {
      _intervals.add(key, bb_start_end);
    }

    // for each operation op of b in reverse order do
    //    for each output operand opd of op do
    //         intervals[opd].setFrom(op.id)
    //         live.remove(opd)
    //    for each input operand opd of op do
    //         intervals[opd].addRange(b.from, op.id)
    //         live.add(opd)
    cur_instr = cur_block->get_last_inst();
    while (cur_instr != nullptr) {
      for (auto& el : cur_instr->getDestRegs()) {
        _intervals.setFrom(el, cur_instr->live_num);
        live.remove(el);
      }
      for (auto& el : cur_instr->getSrcRegs()) {
        _intervals.add(el, LiveRange(cur_block->get_liverange_start(), cur_instr->live_num));
        live.add_empty(el, cur_instr->live_num);
      }
      cur_instr = cur_instr->get_prev();
    }

    // for each phi function phi of b do
    //    live.remove(phi.output)
    cur_instr = cur_block->get_first_inst();
    while (cur_instr != nullptr) {
      if (cur_instr->getOpcode() != Opcode::PHI) {
        cur_instr = cur_instr->get_next();
        continue;
      }

      for (auto& el : cur_instr->getDestRegs()) {
        live.remove(el);
      }
      cur_instr = cur_instr->get_next();
    }

    // if b is loop header then
    //    loopEnd = last block of the loop starting at b
    //    for each opd in live do
    //        intervals[opd].addRange(b.from, loopEnd.to)
    for (auto& loop : loop_vec) {
      if (cur_block->get_id() == loop->get_header_id()) {
        // Single-latched loop expected here
        BasicBlock* loopEnd;
        for (auto& el : loop->get_latches_id()) {
          if (el != cur_block->get_id()) {
            loopEnd = graph->get_block(el);
          }
        }
        for (auto const& [key, _] : live.get_liveIn()) {
          _intervals.add(key,
                         LiveRange(cur_block->get_liverange_start(), loopEnd->get_liverange_end()));
        }
      }
    }

    // b.liveIn = live
    cur_block->set_liveIn(live);
  }

  fix_inervals_with_late_starts();
}

void Liveness::fix_inervals_with_late_starts() {
  auto tmp_copy = _intervals.get_liveIn();

  for (auto& [key, interval] : tmp_copy) {
    if (interval.get_start() < _helper_intervals[key]) {
      interval.set_start(_helper_intervals[key]);
    }
  }

  _intervals.set_liveIn(std::move(tmp_copy));
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
    _helper_intervals[cur_lin] = cur_live;
    cur_lin++;

    while (cur_instr->get_next() != nullptr) {
      cur_instr = cur_instr->get_next();
      if (cur_instr->getOpcode() != Opcode::PHI) {
        cur_live += 2;
      }
      cur_instr->lin = cur_lin;
      cur_instr->live_num = cur_live;
      _helper_intervals[cur_lin] = cur_live;
      cur_lin++;
    }

    cur_live += 2;
    cur_block->set_liverange_end(cur_live);
  }
}

}  // namespace custom