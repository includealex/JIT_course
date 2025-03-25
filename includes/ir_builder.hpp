#ifndef INCLUDES_IR_BUILDER_HPP_
#define INCLUDES_IR_BUILDER_HPP_

#include "basic_block.hpp"
#include "dfs.hpp"
#include "domin_tree.hpp"
#include "function.hpp"
#include "graph.hpp"
#include "instruction_ext.hpp"
#include "regs.hpp"

namespace custom {

class IRBuilder final {
 public:
  ~IRBuilder();

  Function* createFunction(std::string name, Type rettype, std::vector<Instruction*> params);

  Graph* createGraph();

  BasicBlock* createBasicBlock(Graph* graph);

  Instruction* createMOV(Type type, BasicBlock* basic_block, Instruction* movinstr);
  Instruction* createMOVI(Type type, BasicBlock* basic_block, ImmType imm);
  Instruction* createRET(Type type, BasicBlock* basic_block, Instruction* retinstr);
  Instruction* createRET(Type type, BasicBlock* basic_block);
  Instruction* createRETI(Type type, BasicBlock* basic_block, ImmType imm);
  Instruction* createCAST(Type type, BasicBlock* basic_block, Instruction* castinstr);
  Instruction* createJUMP(BasicBlock* basic_block, BasicBlock* trueBasicBlock);
  Instruction* createJA(BasicBlock* basic_block,
                        BasicBlock* trueBasicBlock,
                        BasicBlock* falseBasicBlock);
  Instruction* createADD(Type type,
                         BasicBlock* basic_block,
                         Instruction* first,
                         Instruction* second);
  Instruction* createADDI(Type type, BasicBlock* basic_block, Instruction* first, ImmType imm);
  Instruction* createADDI(Type type,
                          BasicBlock* basic_block,
                          ImmType first_imm,
                          ImmType second_imm);
  Instruction* createSUB(Type type,
                         BasicBlock* basic_block,
                         Instruction* first,
                         Instruction* second);
  Instruction* createSUBI(Type type, BasicBlock* basic_block, Instruction* first, ImmType imm);
  Instruction* createSUBI(Type type,
                          BasicBlock* basic_block,
                          ImmType first_imm,
                          ImmType second_imm);
  Instruction* createMUL(Type type,
                         BasicBlock* basic_block,
                         Instruction* first,
                         Instruction* second);
  Instruction* createMULI(Type type, BasicBlock* basic_block, Instruction* first, ImmType imm);
  Instruction* createMULI(Type type,
                          BasicBlock* basic_block,
                          ImmType first_imm,
                          ImmType second_imm);
  Instruction* createASHR(Type type,
                          BasicBlock* basic_block,
                          Instruction* first,
                          Instruction* second);
  Instruction* createASHRI(Type type, BasicBlock* basic_block, Instruction* first, ImmType imm);
  Instruction* createASHRI(Type type,
                           BasicBlock* basic_block,
                           ImmType first_imm,
                           ImmType second_imm);
  Instruction* createXOR(Type type,
                         BasicBlock* basic_block,
                         Instruction* first,
                         Instruction* second);
  Instruction* createXORI(Type type, BasicBlock* basic_block, Instruction* first, ImmType imm);
  Instruction* createXORI(Type type,
                          BasicBlock* basic_block,
                          ImmType first_imm,
                          ImmType second_imm);
  Instruction* createCMP(Type type,
                         BasicBlock* basic_block,
                         Instruction* first,
                         Instruction* second);
  Instruction* createNEQ(Type type,
                         BasicBlock* basic_block,
                         Instruction* first,
                         Instruction* second);
  Instruction* createNEQ(Type type, BasicBlock* basic_block, Instruction* first);
  Instruction* createPHI(Type type, BasicBlock* basic_block);
  Instruction* createPARAM(Type type);
  Instruction* createCALL(Opcode opcode,
                          Type type,
                          BasicBlock* basicBlock,
                          std::string function_name);

 private:
  bool check_foo_exists(std::string function_name);
  void increase_n_users(Instruction* instr);
  void increase_n_users(Instruction* first, Instruction* second);
  std::vector<Instruction*> _params;
  std::unordered_map<std::string, Function*> _func_table;
};

}  // namespace custom

#endif  // INCLUDES_IR_BUILDER_HPP_
