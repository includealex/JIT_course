#include <gtest/gtest.h>

#include "ir_builder.hpp"

namespace custom {

TEST(FactorialGraphTest, BasicAssertions) {
  custom::IRBuilder builder;

  auto* a0 = builder.createPARAM(Type::myu32);
  custom::Function* function = builder.createFunction("fact_foo", Type::myu64, {a0});

  auto* graph = function->get_graph();
  custom::BasicBlock* entry = builder.createBasicBlock(graph);
  custom::BasicBlock* loop = builder.createBasicBlock(graph);
  custom::BasicBlock* done = builder.createBasicBlock(graph);

  entry->add_succs_true(loop);
  loop->add_succs_false(loop);
  loop->add_succs_true(done);

  auto* v0 = builder.createMOVI(Type::myu64, entry, 0);
  auto* v1 = builder.createMOVI(Type::myu64, entry, 1);
  auto* v2 = builder.createCAST(Type::myu64, entry, a0);
  auto* v3 = builder.createCMP(Type::myu64, loop, v0, v1);
  auto* v4 = builder.createJA(loop, loop, done);
  auto* v5 = builder.createMUL(Type::myu64, loop, v0, v1);
  auto* v6 = builder.createADDI(Type::myu64, loop, v1, 1);
  auto* v7 = builder.createJUMP(loop, loop);
  auto* v8 = builder.createRET(Type::myu64, done, v0);

  ASSERT_EQ(v0->getOpcode(), Opcode::MOVI);
  ASSERT_EQ(v1->getOpcode(), Opcode::MOVI);
  ASSERT_EQ(v2->getOpcode(), Opcode::CAST);
  ASSERT_EQ(v3->getOpcode(), Opcode::CMP);
  ASSERT_EQ(v4->getOpcode(), Opcode::JA);
  ASSERT_EQ(v5->getOpcode(), Opcode::MUL);
  ASSERT_EQ(v6->getOpcode(), Opcode::ADDI);
  ASSERT_EQ(v7->getOpcode(), Opcode::JMP);
  ASSERT_EQ(v8->getOpcode(), Opcode::RET);

  ASSERT_EQ(entry->get_succs_true(), loop);
  ASSERT_EQ(loop->get_succs_false(), loop);
  ASSERT_EQ(loop->get_succs_true(), done);

  ASSERT_EQ(entry->get_id(), 0);
  ASSERT_EQ(loop->get_id(), 1);
  ASSERT_EQ(done->get_id(), 2);

  delete function;
}

}  // namespace custom