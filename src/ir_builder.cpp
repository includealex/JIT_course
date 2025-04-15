#include "ir_builder.hpp"

namespace custom {

Function* IRBuilder::createFunction(std::string name,
                                    Type rettype,
                                    std::vector<Instruction*> params) {
  auto* foo = new Function(name, rettype, params);
  _func_table.insert({name, foo});
  return foo;
}

Graph* IRBuilder::createGraph() {
  return new Graph();
}

BasicBlock* IRBuilder::createBasicBlock(Graph* graph) {
  BasicBlock* basic_block = new BasicBlock(graph);
  graph->addBasicBlock(basic_block);
  return basic_block;
}

Instruction* IRBuilder::createMOV(Type type, BasicBlock* basic_block, Instruction* movinstr) {
  Instruction* instr = new MovInstruction(Opcode::MOV, type, basic_block, movinstr);
  basic_block->pushback_instr(instr);
  return instr;
}

Instruction* IRBuilder::createMOVI(Type type, BasicBlock* basic_block, ImmType imm) {
  Instruction* instr = new MovInstruction(Opcode::MOVI, type, basic_block, imm);
  basic_block->pushback_instr(instr);
  return instr;
}

Instruction* IRBuilder::createRET(Type type, BasicBlock* basic_block, Instruction* retinstr) {
  Instruction* instr = new RetInstruction(Opcode::RET, type, basic_block, retinstr);
  basic_block->pushback_instr(instr);
  basic_block->get_graph()->add_return_block(basic_block);
  increase_n_users(retinstr);
  retinstr->add_user_inst(instr);
  return instr;
}

Instruction* IRBuilder::createRET(Type type, BasicBlock* basic_block) {
  Instruction* instr = new RetInstruction(Opcode::RET, type, basic_block);
  basic_block->get_graph()->add_return_block(basic_block);
  basic_block->pushback_instr(instr);
  return instr;
}

Instruction* IRBuilder::createRETI(Type type, BasicBlock* basic_block, ImmType imm) {
  Instruction* instr = new RetInstruction(Opcode::RETI, type, basic_block, imm);
  basic_block->get_graph()->add_return_block(basic_block);
  basic_block->pushback_instr(instr);
  return instr;
}

Instruction* IRBuilder::createCAST(Type type, BasicBlock* basic_block, Instruction* castinstr) {
  Instruction* instr = new CastInstruction(Opcode::CAST, type, basic_block, castinstr);
  basic_block->pushback_instr(instr);
  increase_n_users(castinstr);
  castinstr->add_user_inst(instr);
  return instr;
}

Instruction* IRBuilder::createJUMP(BasicBlock* basic_block, BasicBlock* trueBasicBlock) {
  Instruction* instr = new JumpInstruction(Opcode::JMP, Type::myvoid, basic_block, trueBasicBlock);
  basic_block->pushback_instr(instr);
  return instr;
}

Instruction* IRBuilder::createJA(BasicBlock* basic_block,
                                 BasicBlock* trueBasicBlock,
                                 BasicBlock* falseBasicBlock) {
  Instruction* instr =
      new JumpInstruction(Opcode::JA, Type::myvoid, basic_block, trueBasicBlock, falseBasicBlock);
  basic_block->pushback_instr(instr);
  return instr;
}

Instruction* IRBuilder::createADD(Type type,
                                  BasicBlock* basic_block,
                                  Instruction* first,
                                  Instruction* second) {
  Instruction* instr = new InstructionFrom2Instr(Opcode::ADD, type, basic_block, first, second);
  basic_block->pushback_instr(instr);
  increase_n_users(first, second);
  first->add_user_inst(instr);
  second->add_user_inst(instr);
  return instr;
}

Instruction* IRBuilder::createADDI(Type type,
                                   BasicBlock* basic_block,
                                   Instruction* first,
                                   ImmType imm) {
  Instruction* instr = new InstructionFromInstrAndImm(Opcode::ADDI, type, basic_block, first, imm);
  basic_block->pushback_instr(instr);
  increase_n_users(first);
  first->add_user_inst(instr);
  return instr;
}

Instruction* createADDI(Type type, BasicBlock* basic_block, ImmType first_imm, ImmType second_imm) {
  Instruction* instr =
      new InstructionFrom2Imm(Opcode::ADDI, type, basic_block, first_imm, second_imm);
  basic_block->pushback_instr(instr);
  return instr;
}

Instruction* IRBuilder::createSUB(Type type,
                                  BasicBlock* basic_block,
                                  Instruction* first,
                                  Instruction* second) {
  Instruction* instr = new InstructionFrom2Instr(Opcode::SUB, type, basic_block, first, second);
  basic_block->pushback_instr(instr);
  increase_n_users(first, second);
  first->add_user_inst(instr);
  second->add_user_inst(instr);
  return instr;
}

Instruction* IRBuilder::createSUBI(Type type,
                                   BasicBlock* basic_block,
                                   Instruction* first,
                                   ImmType imm) {
  Instruction* instr = new InstructionFromInstrAndImm(Opcode::SUBI, type, basic_block, first, imm);
  basic_block->pushback_instr(instr);
  increase_n_users(first);
  first->add_user_inst(instr);
  return instr;
}

Instruction* IRBuilder::createSUBI(Type type,
                                   BasicBlock* basic_block,
                                   ImmType first_imm,
                                   ImmType second_imm) {
  Instruction* instr =
      new InstructionFrom2Imm(Opcode::SUBI, type, basic_block, first_imm, second_imm);
  basic_block->pushback_instr(instr);
  return instr;
}

Instruction* IRBuilder::createMUL(Type type,
                                  BasicBlock* basic_block,
                                  Instruction* first,
                                  Instruction* second) {
  Instruction* instr = new InstructionFrom2Instr(Opcode::MUL, type, basic_block, first, second);
  basic_block->pushback_instr(instr);
  increase_n_users(first, second);
  first->add_user_inst(instr);
  second->add_user_inst(instr);
  return instr;
}

Instruction* IRBuilder::createMULI(Type type,
                                   BasicBlock* basic_block,
                                   Instruction* first,
                                   ImmType imm) {
  Instruction* instr = new InstructionFromInstrAndImm(Opcode::MULI, type, basic_block, first, imm);
  basic_block->pushback_instr(instr);
  increase_n_users(first);
  first->add_user_inst(instr);
  return instr;
}

Instruction* IRBuilder::createBOUNDSCHECK(Type type,
                                          BasicBlock* basic_block,
                                          Instruction* first,
                                          ImmType imm) {
  Instruction* instr =
      new InstructionFromInstrAndImm(Opcode::BOUNDSCHECK, type, basic_block, first, imm);
  basic_block->pushback_instr(instr);
  increase_n_users(first);
  first->add_user_inst(instr);
  return instr;
}

Instruction* IRBuilder::createMULI(Type type,
                                   BasicBlock* basic_block,
                                   ImmType first_imm,
                                   ImmType second_imm) {
  Instruction* instr =
      new InstructionFrom2Imm(Opcode::MULI, type, basic_block, first_imm, second_imm);
  basic_block->pushback_instr(instr);
  return instr;
}

Instruction* IRBuilder::createASHR(Type type,
                                   BasicBlock* basic_block,
                                   Instruction* first,
                                   Instruction* second) {
  Instruction* instr = new InstructionFrom2Instr(Opcode::ASHR, type, basic_block, first, second);
  basic_block->pushback_instr(instr);
  increase_n_users(first, second);
  first->add_user_inst(instr);
  second->add_user_inst(instr);
  return instr;
}

Instruction* IRBuilder::createASHRI(Type type,
                                    BasicBlock* basic_block,
                                    Instruction* first,
                                    ImmType imm) {
  Instruction* instr = new InstructionFromInstrAndImm(Opcode::ASHRI, type, basic_block, first, imm);
  basic_block->pushback_instr(instr);
  increase_n_users(first);
  first->add_user_inst(instr);
  return instr;
}

Instruction* IRBuilder::createASHRI(Type type,
                                    BasicBlock* basic_block,
                                    ImmType first_imm,
                                    ImmType second_imm) {
  Instruction* instr =
      new InstructionFrom2Imm(Opcode::ASHRI, type, basic_block, first_imm, second_imm);
  basic_block->pushback_instr(instr);
  return instr;
}

Instruction* IRBuilder::createXOR(Type type,
                                  BasicBlock* basic_block,
                                  Instruction* first,
                                  Instruction* second) {
  Instruction* instr = new InstructionFrom2Instr(Opcode::XOR, type, basic_block, first, second);
  basic_block->pushback_instr(instr);
  increase_n_users(first, second);
  first->add_user_inst(instr);
  second->add_user_inst(instr);
  return instr;
}

Instruction* IRBuilder::createXORI(Type type,
                                   BasicBlock* basic_block,
                                   Instruction* first,
                                   ImmType imm) {
  Instruction* instr = new InstructionFromInstrAndImm(Opcode::XORI, type, basic_block, first, imm);
  basic_block->pushback_instr(instr);
  increase_n_users(first);
  first->add_user_inst(instr);
  return instr;
}

Instruction* IRBuilder::createXORI(Type type,
                                   BasicBlock* basic_block,
                                   ImmType first_imm,
                                   ImmType second_imm) {
  Instruction* instr =
      new InstructionFrom2Imm(Opcode::XORI, type, basic_block, first_imm, second_imm);
  basic_block->pushback_instr(instr);
  return instr;
}

Instruction* IRBuilder::createCMP(Type type,
                                  BasicBlock* basic_block,
                                  Instruction* first,
                                  Instruction* second) {
  Instruction* instr = new InstructionFrom2Instr(Opcode::CMP, type, basic_block, first, second);
  basic_block->pushback_instr(instr);
  increase_n_users(first, second);
  first->add_user_inst(instr);
  second->add_user_inst(instr);
  return instr;
}

Instruction* IRBuilder::createNEQ(Type type,
                                  BasicBlock* basic_block,
                                  Instruction* first,
                                  Instruction* second) {
  Instruction* instr = new InstructionFrom2Instr(Opcode::NEQ, type, basic_block, first, second);
  basic_block->pushback_instr(instr);
  increase_n_users(first, second);
  first->add_user_inst(instr);
  second->add_user_inst(instr);
  return instr;
}

Instruction* IRBuilder::createNULLCHECK(Type type, BasicBlock* basic_block, Instruction* first) {
  Instruction* instr = new NullCheckInstruction(Opcode::NULLCHECK, type, basic_block, first);
  basic_block->pushback_instr(instr);
  increase_n_users(first);
  first->add_user_inst(instr);
  return instr;
}

Instruction* IRBuilder::createNEQ(Type type, BasicBlock* basic_block, Instruction* first) {
  Instruction* instr = new NeqInstruction(Opcode::NEQ, type, basic_block, first);
  basic_block->pushback_instr(instr);
  increase_n_users(first);
  first->add_user_inst(instr);
  return instr;
}

Instruction* IRBuilder::createPHI(Type type, BasicBlock* basic_block) {
  Instruction* instr = new PhiInstruction(Opcode::PHI, type, basic_block);
  basic_block->pushback_instr(instr);
  return instr;
}

Instruction* IRBuilder::createPARAM(Type type) {
  Instruction* instr = new Param(Opcode::PARAM, type);
  _params.push_back(instr);
  return instr;
}

Instruction* IRBuilder::createCALL(Type type, BasicBlock* basicBlock, std::string function_name) {
  if (!check_foo_exists(function_name)) {
    std::cerr << "CALL can not be created. No function with name: " << function_name << std::endl;
  }

  Instruction* instr = new CallInstruction(Opcode::CALL, type, basicBlock, function_name);
  basicBlock->pushback_instr(instr);
  return instr;
}

Instruction* IRBuilder::createCALL(Type type,
                                   BasicBlock* basicBlock,
                                   std::string function_name,
                                   Instruction* first,
                                   Instruction* second) {
  if (!check_foo_exists(function_name)) {
    std::cerr << "CALL can not be created. No function with name: " << function_name << std::endl;
  }

  Instruction* instr = new CallInstruction(Opcode::CALL, type, basicBlock, function_name);
  basicBlock->pushback_instr(instr);
  increase_n_users(first, second);
  first->add_user_inst(instr);
  second->add_user_inst(instr);
  return instr;
}

IRBuilder::~IRBuilder() {
  for (auto& tmp_param : _params) {
    delete tmp_param;
  }
}

void IRBuilder::increase_n_users(Instruction* instr) {
  if (instr == nullptr) {
    return;
  }

  instr->add_user();
}

void IRBuilder::increase_n_users(Instruction* first, Instruction* second) {
  increase_n_users(first);

  if (first == second) {
    return;
  }

  increase_n_users(second);
}

bool IRBuilder::check_foo_exists(std::string function_name) {
  if (_func_table.find(function_name) == _func_table.end()) {
    return false;
  }
  return true;
}

Function* IRBuilder::getFunction(std::string name) {
  return _func_table[name];
}

}  // namespace custom