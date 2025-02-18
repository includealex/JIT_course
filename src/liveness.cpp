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
    auto cur_block = graph->get_block(bb_id);

    LiveInterval live;
    for (auto& tmp_succ : {cur_block->get_succs_true(), cur_block->get_succs_false()}) {
        if (tmp_succ == nullptr) {
            continue;
        }

        // live = union of successor.liveIn for each successor of b
        live = {live, tmp_succ->get_liveIn()};

        // for each phi function phi of successors of b do
        //    live.add(phi.inputOf(b)) 
        auto cur_instr = tmp_succ->get_first_inst();
        while (cur_instr != nullptr) {
            if (cur_instr->getOpcode() == Opcode::PHI) {
                for (auto& el : cur_instr->getSrcRegs()) {
                    live.add_empty(el);
                }
            }
            cur_instr = cur_instr->get_next();
        }

        // for each opd in live do
        //    intervals[opd].addRange(b.from, b.to)
        LiveRange bb_start_end(cur_block->get_liverange());
        std::map<std::size_t, LiveRange> new_liveIn;
        for (auto const& [key, _] : live.get_liveIn()) {
            new_liveIn[key] = bb_start_end;
        }
        _intervals.set_liveIn(new_liveIn);
        new_liveIn.clear();

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
                new_liveIn[el] = LiveRange(cur_instr->live_num,cur_instr->live_num+2);
                live.remove(el);
            }
            for (auto& el : cur_instr->getSrcRegs()) {
                new_liveIn[el] = LiveRange(cur_block->get_liverange_start(),cur_instr->live_num);
                live.add_empty(el);
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

        // TODO:
        // if b is loop header then
        //    loopEnd = last block of the loop starting at b
        //    for each opd in live do
        //        intervals[opd].addRange(b.from, loopEnd.to)

        cur_block->set_liveIn(live);
        _intervals.print();
    }
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