#include "liveness.hpp"

namespace custom {

void Liveness::run_analysis(Graph* graph) {
    set_bb_liveranges(graph);

    return;
}


void Liveness::set_bb_liveranges(Graph* graph) {
    Linorder linorder;
    auto linorder_real = linorder.get_linorder(graph);
    std::size_t cur_live = 0;
    std::size_t cur_lin = 0;

    for (auto& bb_id : linorder_real) {
        auto cur_block = graph->get_block(bb_id);
        cur_block->live_start = cur_live;
        
        auto cur_instr = cur_block->get_first_inst();
        if (cur_instr == nullptr) {
            cur_block->live_end = cur_block->live_start;
            continue;
        }
        
        if (cur_instr->getOpcode() != Opcode::PHI) {
            cur_live += 2;
        }
        cur_instr->lin = cur_lin;
        cur_instr->live_num = cur_live;
        cur_lin++;

        while(cur_instr->get_next() != nullptr) {
            cur_instr = cur_instr->get_next();
            if (cur_instr->getOpcode() != Opcode::PHI) {
                cur_live += 2;
            }
            cur_instr->lin = cur_lin;
            cur_instr->live_num = cur_live;
            cur_lin++;
        }

        cur_live += 2;
        cur_block->live_end = cur_live;
    }
}

} // namespace custom