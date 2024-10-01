#include "basic_block.hpp"
#include "instruction.hpp"
#include "graph.hpp"

namespace custom {

void BasicBlock::addInstruction(Instruction* inst) {
    inst->_basic_block = this;

    if (_last_inst) {
        _last_inst->_next = inst;
        inst->_prev = _last_inst;
        _last_inst = inst;
    } else {
        _first_inst = inst;
        _last_inst = inst;
        inst->_prev = nullptr;
    }

    inst->_next = nullptr;

    if (isPhiInstruction(inst)) {
        if (!_first_Phi) {
            _first_Phi = inst;
        }
    }

    _instructions.push_back(inst);
}

size_t BasicBlock::getInstructionCount() const {
    return _instructions.size();
}

Instruction* BasicBlock::getInstruction(size_t index) const {
    if (index < _instructions.size()) {
        return _instructions[index];
    }
    return nullptr;
}

bool BasicBlock::isPhiInstruction(Instruction* inst) {
    return inst->getOpcode() == Opcode::PHI;
}

} // namespace custom

