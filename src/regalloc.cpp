#include "regalloc.hpp"
#include <algorithm>
#include <iostream>

namespace custom {

std::map<std::size_t, std::variant<std::size_t, std::string>> 
Regalloc::run_analysis(Graph* graph) {
    Liveness lv;
    auto intervals = lv.run_analysis(graph).get_liveIn();

    std::map<std::size_t, std::variant<std::size_t, std::string>> allocation_result;

    for (const auto& interval_i : intervals) {
        ExpireOldIntervals(interval_i);
        if (_active_regs.size() >= _reg_num) {
            SpillAllInterval(interval_i);
        } else {
            AssignRegister(interval_i);
        }

        if (_allocated_registers.find(interval_i.first) != _allocated_registers.end()) {
            allocation_result[interval_i.first] = _allocated_registers[interval_i.first];
        } else {
            allocation_result[interval_i.first] = _stack_location[interval_i.first];
        }
    }
    
    return allocation_result;
}

void Regalloc::ExpireOldIntervals(const std::pair<const std::size_t, custom::LiveRange>& interval_i) {
    for (auto it = _active_regs.begin(); it != _active_regs.end();) {
        if (it->second.get_end() < interval_i.second.get_start()) {
            std::size_t reg = _allocated_registers[it->first];
            _free_registers.push_back(reg);
            _allocated_registers.erase(it->first);
            it = _active_regs.erase(it);
        } else {
            ++it;
        }
    }
}

void Regalloc::SpillAllInterval(const std::pair<const std::size_t, custom::LiveRange>& interval_i) {
    auto spill_it = std::prev(_active_regs.end());

    if (spill_it->second.get_end() > interval_i.second.get_end()) {
        _stack_location[spill_it->first] = AllocateStackLocation();
        _allocated_registers[interval_i.first] = _allocated_registers[spill_it->first];
        _allocated_registers.erase(spill_it->first);
        _active_regs.erase(spill_it);
        _active_regs[interval_i.first] = interval_i.second;
    } else {
        _stack_location[interval_i.first] = AllocateStackLocation();
    }
}

void Regalloc::AssignRegister(const std::pair<const std::size_t, custom::LiveRange>& interval_i) {
    if (!_free_registers.empty()) {
        std::size_t reg = _free_registers.back();
        _free_registers.pop_back();
        _allocated_registers[interval_i.first] = reg;
    } else {
        _allocated_registers[interval_i.first] = _allocated_registers.size();
    }
    _active_regs[interval_i.first] = interval_i.second;
}

std::string Regalloc::AllocateStackLocation() {
    static std::size_t stack_offset = 0;
    return "stack_slot_" + std::to_string(stack_offset++);
}

} // namespace custom
