#include "instruction_ext.hpp"

namespace custom {

MovInstruction::MovInstruction(Opcode opcode, Type type, BasicBlock* basicBlock, ImmType value)
    : Instruction(opcode, type, basicBlock) {
  _imm = value;
  assert((opcode == Opcode::MOVI) or (opcode == Opcode::MOV));
}

RetInstruction::RetInstruction(Opcode opcode, Type type, BasicBlock* basicBlock)
    : Instruction(opcode, type, basicBlock) {
  assert(opcode == Opcode::RET);
}

RetInstruction::RetInstruction(Opcode opcode, Type type, BasicBlock* basicBlock, Instruction* instr)
    : Instruction(opcode, type, basicBlock) {
  assert(opcode == Opcode::RET);
  _src_insts.insert(instr);
}

RetInstruction::RetInstruction(Opcode opcode, Type type, BasicBlock* basicBlock, ImmType value)
    : Instruction(opcode, type, basicBlock) {
  assert(opcode == Opcode::RETI);
  _imm = value;
}

CastInstruction::CastInstruction(Opcode opcode,
                                 Type type,
                                 BasicBlock* basicBlock,
                                 Instruction* instr)
    : Instruction(opcode, type, basicBlock) {
  assert(opcode == Opcode::CAST);
  _src_insts.insert(instr);
}

JumpInstruction::JumpInstruction(Opcode opcode,
                                 Type type,
                                 BasicBlock* basicBlock,
                                 BasicBlock* trueBasicBlock,
                                 BasicBlock* falseBasicBlock)
    : Instruction(opcode, type, basicBlock),
      _trueBasicBlock(trueBasicBlock),
      _falseBasicBlock(falseBasicBlock) {
  assert(opcode == Opcode::JA);
}

JumpInstruction::JumpInstruction(Opcode opcode,
                                 Type type,
                                 BasicBlock* basicBlock,
                                 BasicBlock* trueBasicBlock)
    : Instruction(opcode, type, basicBlock), _trueBasicBlock(trueBasicBlock) {
  assert(opcode == Opcode::JMP);
}

BasicBlock* JumpInstruction::get_true_BB() const {
  return _trueBasicBlock;
}
BasicBlock* JumpInstruction::get_false_BB() const {
  return _falseBasicBlock;
}

void JumpInstruction::set_true_BB(BasicBlock* new_BB) {
  _trueBasicBlock = new_BB;
}
void JumpInstruction::set_false_BB(BasicBlock* new_BB) {
  _falseBasicBlock = new_BB;
}

InstructionFrom2Instr::InstructionFrom2Instr(
    Opcode opcode, Type type, BasicBlock* basicBlock, Instruction* first, Instruction* second) {
  assert((opcode == Opcode::MUL) or (opcode == Opcode::MULI) or (opcode == Opcode::SUB) or
         (opcode == Opcode::SUBI) or (opcode == Opcode::ADD) or (opcode == Opcode::ADDI) or
         (opcode == Opcode::ASHR) or (opcode == Opcode::ASHRI) or (opcode == Opcode::XOR) or
         (opcode == Opcode::XORI) or (opcode == Opcode::CMP) or (opcode == Opcode::NEQ));
  _src_insts.insert(first);
  _src_insts.insert(second);
}

PhiInstruction::PhiInstruction(Opcode opcode, Type type, BasicBlock* basicBlock)
    : Instruction(opcode, type, basicBlock) {
  assert(opcode == Opcode::PHI);
}

void PhiInstruction::AddPhiUsage(Instruction* first, Instruction* second) {
  _src_insts.insert(first);
  _src_insts.insert(second);
}

NeqInstruction::NeqInstruction(Opcode opcode, Type type, BasicBlock* basicBlock, Instruction* instr)
    : Instruction(opcode, type, basicBlock) {
  _src_insts.insert(instr);
}

InstructionFromInstrAndImm::InstructionFromInstrAndImm(
    Opcode opcode, Type type, BasicBlock* basicBlock, Instruction* instr, ImmType imm)
    : Instruction(opcode, type, basicBlock) {
  assert((opcode == Opcode::MUL) or (opcode == Opcode::MULI) or (opcode == Opcode::SUB) or
         (opcode == Opcode::SUBI) or (opcode == Opcode::ADD) or (opcode == Opcode::ADDI) or
         (opcode == Opcode::ASHR) or (opcode == Opcode::ASHRI) or (opcode == Opcode::XOR) or
         (opcode == Opcode::CMP) or (opcode == Opcode::NEQ));
  _src_insts.insert(instr);
}

}  // namespace custom