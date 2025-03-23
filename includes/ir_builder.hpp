#ifndef INCLUDES_IR_BUILDER_HPP_
#define INCLUDES_IR_BUILDER_HPP_

#include "basic_block.hpp"
#include "dfs.hpp"
#include "domin_tree.hpp"
#include "graph.hpp"
#include "instruction_ext.hpp"
#include "regs.hpp"

namespace custom {

class IRBuilder final {
 public:
  // TODO:
  // add create Function() method

  static Graph* createGraph() {
    return new Graph();
  }

  static BasicBlock* createBasicBlock(Graph* graph) {
    BasicBlock* basic_block = new BasicBlock(graph);
    graph->addBasicBlock(basic_block);
    return basic_block;
  }

  static Instruction* createMOV(Type type, BasicBlock* basic_block, ImmType imm) {
    Instruction* instr = new MovInstruction(Opcode::MOV, type, basic_block, imm);
    basic_block->pushback_instr(instr);
    return instr;
  }

  static Instruction* createMOVI(Type type, BasicBlock* basic_block, ImmType imm) {
    Instruction* instr = new MovInstruction(Opcode::MOVI, type, basic_block, imm);
    basic_block->pushback_instr(instr);
    return instr;
  }

  static Instruction* createRET(Type type, BasicBlock* basic_block, Instruction* retinstr) {
    Instruction* instr = new RetInstruction(Opcode::RET, type, basic_block, retinstr);
    basic_block->pushback_instr(instr);
    return instr;
  }

  static Instruction* createRET(Type type, BasicBlock* basic_block) {
    Instruction* instr = new RetInstruction(Opcode::RET, type, basic_block);
    basic_block->pushback_instr(instr);
    return instr;
  }

  static Instruction* createRETI(Type type, BasicBlock* basic_block, ImmType imm) {
    Instruction* instr = new RetInstruction(Opcode::RETI, type, basic_block, imm);
    basic_block->pushback_instr(instr);
    return instr;
  }

  static Instruction* createCAST(Type type, BasicBlock* basic_block, Instruction* castinstr) {
    Instruction* instr = new CastInstruction(Opcode::CAST, type, basic_block, castinstr);
    basic_block->pushback_instr(instr);
    return instr;
  }

  static Instruction* createJUMP(BasicBlock* basic_block, BasicBlock* trueBasicBlock) {
    Instruction* instr =
        new JumpInstruction(Opcode::JMP, Type::myvoid, basic_block, trueBasicBlock);
    basic_block->pushback_instr(instr);
    return instr;
  }

  static Instruction* createJA(BasicBlock* basic_block,
                               BasicBlock* trueBasicBlock,
                               BasicBlock* falseBasicBlock) {
    Instruction* instr =
        new JumpInstruction(Opcode::JA, Type::myvoid, basic_block, trueBasicBlock, falseBasicBlock);
    basic_block->pushback_instr(instr);
    return instr;
  }

  static Instruction* createADD(Type type,
                                BasicBlock* basic_block,
                                Instruction* first,
                                Instruction* second) {
    Instruction* instr = new InstructionFrom2Instr(Opcode::ADD, type, basic_block, first, second);
    basic_block->pushback_instr(instr);
    return instr;
  }

  static Instruction* createADDI(Type type,
                                 BasicBlock* basic_block,
                                 Instruction* first,
                                 ImmType imm) {
    Instruction* instr =
        new InstructionFromInstrAndImm(Opcode::ADDI, type, basic_block, first, imm);
    basic_block->pushback_instr(instr);
    return instr;
  }

  static Instruction* createSUB(Type type,
                                BasicBlock* basic_block,
                                Instruction* first,
                                Instruction* second) {
    Instruction* instr = new InstructionFrom2Instr(Opcode::SUB, type, basic_block, first, second);
    basic_block->pushback_instr(instr);
    return instr;
  }

  static Instruction* createSUBI(Type type,
                                 BasicBlock* basic_block,
                                 Instruction* first,
                                 ImmType imm) {
    Instruction* instr =
        new InstructionFromInstrAndImm(Opcode::SUBI, type, basic_block, first, imm);
    basic_block->pushback_instr(instr);
    return instr;
  }

  static Instruction* createMUL(Type type,
                                BasicBlock* basic_block,
                                Instruction* first,
                                Instruction* second) {
    Instruction* instr = new InstructionFrom2Instr(Opcode::MUL, type, basic_block, first, second);
    basic_block->pushback_instr(instr);
    return instr;
  }

  static Instruction* createMULI(Type type,
                                 BasicBlock* basic_block,
                                 Instruction* first,
                                 ImmType imm) {
    Instruction* instr =
        new InstructionFromInstrAndImm(Opcode::MULI, type, basic_block, first, imm);
    basic_block->pushback_instr(instr);
    return instr;
  }

  static Instruction* createASHR(Type type,
                                 BasicBlock* basic_block,
                                 Instruction* first,
                                 Instruction* second) {
    Instruction* instr = new InstructionFrom2Instr(Opcode::ASHR, type, basic_block, first, second);
    basic_block->pushback_instr(instr);
    return instr;
  }

  static Instruction* createASHRI(Type type,
                                  BasicBlock* basic_block,
                                  Instruction* first,
                                  ImmType imm) {
    Instruction* instr =
        new InstructionFromInstrAndImm(Opcode::ASHRI, type, basic_block, first, imm);
    basic_block->pushback_instr(instr);
    return instr;
  }

  static Instruction* createXOR(Type type,
                                BasicBlock* basic_block,
                                Instruction* first,
                                Instruction* second) {
    Instruction* instr = new InstructionFrom2Instr(Opcode::XOR, type, basic_block, first, second);
    basic_block->pushback_instr(instr);
    return instr;
  }

  static Instruction* createXORI(Type type,
                                 BasicBlock* basic_block,
                                 Instruction* first,
                                 ImmType imm) {
    Instruction* instr =
        new InstructionFromInstrAndImm(Opcode::XORI, type, basic_block, first, imm);
    basic_block->pushback_instr(instr);
    return instr;
  }

  static Instruction* createCMP(Type type,
                                BasicBlock* basic_block,
                                Instruction* first,
                                Instruction* second) {
    Instruction* instr = new InstructionFrom2Instr(Opcode::CMP, type, basic_block, first, second);
    basic_block->pushback_instr(instr);
    return instr;
  }

  static Instruction* createNEQ(Type type,
                                BasicBlock* basic_block,
                                Instruction* first,
                                Instruction* second) {
    Instruction* instr = new InstructionFrom2Instr(Opcode::NEQ, type, basic_block, first, second);
    basic_block->pushback_instr(instr);
    return instr;
  }

  static Instruction* createNEQ(Type type, BasicBlock* basic_block, Instruction* first) {
    Instruction* instr = new NeqInstruction(Opcode::NEQ, type, basic_block, first);
    basic_block->pushback_instr(instr);
    return instr;
  }

  static Instruction* createPHI(Type type, BasicBlock* basic_block) {
    Instruction* instr = new PhiInstruction(Opcode::PHI, type, basic_block);
    basic_block->pushback_instr(instr);
    return instr;
  }
};

}  // namespace custom

#endif  // INCLUDES_IR_BUILDER_HPP_
