#include "regalloc.hpp"

namespace custom {

void Regalloc::run_analysis(Graph* graph) {
    Liveness lv;
    auto intervals = lv.run_analysis(graph).get_liveIn();

    for (auto& interval_i : intervals) {
        ExpireOldIntervals(interval_i);
        if (_active_regs.size() == _reg_num) {
            SpillAllInterval(interval_i);
        }
        else {
            _active_regs.push_back(interval_i); // register removed from pool of free registers
        }
    }

    return;
}

void Regalloc::ExpireOldIntervals(std::pair<const std::size_t, custom::LiveRange> interval_i) {
    for (auto& interval_j : _active_regs) {
        if (interval_j.second.get_end() > interval_i.second.get_end()) {
            return;
        }
        // TODO:
        // remove interval_j from _active_regs
        // add register[j] to pool of free registers
    }

    return;
}

void Regalloc::SpillAllInterval(std::pair<const std::size_t, custom::LiveRange> interval_i) {
    // spill is last interval in _active_regs
    auto spill = _active_regs.back();
    if (spill.second.get_end() > interval_i.second.get_end()) {
        // TODO:
        // register[interval_i] is register[spill]
        // location[spill] is new stack location
        // remove spill from active
        // add interval_i to active, sorted by increasing end point
    }
    else {
        // TODO:
        // location[i] is new stack location
    }

    return;
}

} // namespace custom