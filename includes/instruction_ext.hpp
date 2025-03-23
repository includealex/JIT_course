#ifndef INCLUDES_INSTRUCTION_EXT_HPP_
#define INCLUDES_INSTRUCTION_EXT_HPP_

#include <cassert>

#include "instruction.hpp"

namespace custom {

class MovInstruction : public custom::Instruction {
 public:
  MovInstruction(Opcode opcode, Type type, BasicBlock* basicBlock, ImmType value);
};

class RetInstruction : public Instruction {
 public:
  RetInstruction(Opcode opcode, Type type, BasicBlock* basicBlock);
  RetInstruction(Opcode opcode, Type type, BasicBlock* basicBlock, ImmType value);
  RetInstruction(Opcode opcode, Type type, BasicBlock* basicBlock, Instruction* instr);
};

class CastInstruction : public Instruction {
 public:
  CastInstruction(Opcode opcode, Type type, BasicBlock* basicBlock, Instruction* instr);
};

class JumpInstruction : public Instruction {
 public:
  JumpInstruction(Opcode opcode, Type type, BasicBlock* basicBlock, BasicBlock* trueBasicBlock);
  JumpInstruction(Opcode opcode,
                  Type type,
                  BasicBlock* basicBlock,
                  BasicBlock* trueBasicBlock,
                  BasicBlock* falseBasicBlock);

  BasicBlock* get_true_BB() const;
  BasicBlock* get_false_BB() const;

  void set_true_BB(BasicBlock* new_BB);
  void set_false_BB(BasicBlock* new_BB);

 private:
  BasicBlock* _trueBasicBlock;
  BasicBlock* _falseBasicBlock;
};

class InstructionFrom2Instr : public Instruction {
 public:
  InstructionFrom2Instr(
      Opcode opcode, Type type, BasicBlock* basicBlock, Instruction* first, Instruction* second);
};

class NeqInstruction : public Instruction {
 public:
  NeqInstruction(Opcode opcode, Type type, BasicBlock* basicBlock, Instruction* instr);
};

class PhiInstruction : public Instruction {
 public:
  PhiInstruction(Opcode opcode, Type type, BasicBlock* basicBlock);
  void AddPhiUsage(Instruction* first, Instruction* second);
};

class InstructionFromInstrAndImm : public Instruction {
 public:
  InstructionFromInstrAndImm(
      Opcode opcode, Type type, BasicBlock* basicBlock, Instruction* instr, ImmType imm);
};

}  // namespace custom

#endif  // INCLUDES_INSTRUCTION_EXT_HPP_