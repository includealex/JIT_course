#include "instruction.hpp"

namespace custom {

Instruction::Instruction(Opcode Opcode,
                         Type type,
                         BasicBlock* basicBlock,
                         Instruction* next_instr,
                         Instruction* prev_instr)
    : _prev(prev_instr),
      _next(next_instr),
      _opcode(Opcode),
      _basic_block(basicBlock),
      _type(type) {}

Instruction::Instruction(Opcode opcode,
                         Type type,
                         BasicBlock* basicBlock,
                         const std::vector<std::size_t>& destRegs,
                         const std::vector<std::size_t>& srcRegs)
    : _prev(nullptr),
      _next(nullptr),
      _opcode(opcode),
      _basic_block(basicBlock),
      _type(type),
      _destRegs(destRegs),
      _srcRegs(srcRegs) {}

Instruction::Instruction(Opcode opcode,
                         Type type,
                         BasicBlock* basicBlock,
                         const std::vector<std::size_t>& destRegs,
                         const std::size_t value)
    : _prev(nullptr),
      _next(nullptr),
      _opcode(opcode),
      _basic_block(basicBlock),
      _type(type),
      _destRegs(destRegs),
      _imm(value) {}

Opcode Instruction::getOpcode() const {
  return _opcode;
}

Type Instruction::getType() const {
  return _type;
}

const std::vector<std::size_t>& Instruction::getDestRegs() const {
  return _destRegs;
}

const std::vector<std::size_t>& Instruction::getSrcRegs() const {
  return _srcRegs;
}

void Instruction::set_basic_block(BasicBlock* other) {
  _basic_block = other;
}

void Instruction::set_next(Instruction* other) {
  _next = other;
}

void Instruction::set_prev(Instruction* other) {
  _prev = other;
}

Instruction* Instruction::get_next() const {
  return _next;
}

Instruction* Instruction::get_prev() const {
  return _prev;
}

void Instruction::set_id(std::size_t id) {
  _instr_id = id;
}

std::size_t Instruction::get_id() const {
  return _instr_id;
}

std::size_t Instruction::getImmediateValue() const {
  return _imm;
}

std::size_t Instruction::get_lin() const {
  return _lin;
}

std::size_t Instruction::get_livenum() const {
  return _live_num;
}

void Instruction::set_lin(std::size_t num) {
  _lin = num;
}

void Instruction::set_livenum(std::size_t num) {
  _live_num = num;
}

}  // namespace custom