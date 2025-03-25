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

Opcode Instruction::getOpcode() const {
  return _opcode;
}

Type Instruction::getType() const {
  return _type;
}

ImmType Instruction::get_imm() const {
  return _imm;
}

ImmType Instruction::get_second_imm() const {
  return _second_imm;
}

void Instruction::set_imm(ImmType value) {
  _imm = value;
}

void Instruction::set_second_imm(ImmType value) {
  _second_imm = value;
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

void Instruction::add_src_inst(Instruction* instr) {
  if (std::find(_src_insts.begin(), _src_insts.end(), instr) != _src_insts.end()) {
    return;
  }

  _src_insts.push_back(instr);
}

std::vector<Instruction*> Instruction::get_src_insts() {
  return _src_insts;
}

void Instruction::remove_src_instr(Instruction* instr) {
  if (instr == nullptr) {
    return;
  }

  _src_insts.erase(std::find(_src_insts.begin(), _src_insts.end(), instr));
}

void Instruction::add_user() {
  _n_users++;
}

void Instruction::sub_user() {
  _n_users--;
}

std::size_t Instruction::get_users() {
  return _n_users;
}

void Instruction::setOpcode(Opcode opc) {
  _opcode = opc;
}

std::string Instruction::get_called_name() const {
  return _called_name;
}

void Instruction::set_called_name(std::string name) {
  _called_name = name;
}

}  // namespace custom
