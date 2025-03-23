#include <gtest/gtest.h>

#include "ir_builder.hpp"

namespace custom {

custom::Graph* buildFactorialGraph() {
  custom::IRBuilder builder;

  custom::Graph* graph = builder.createGraph();
  custom::BasicBlock* entry = builder.createBasicBlock(graph);
  custom::BasicBlock* loop = builder.createBasicBlock(graph);
  custom::BasicBlock* done = builder.createBasicBlock(graph);

  entry->add_succs_true(loop);
  loop->add_succs_false(loop);
  loop->add_succs_true(done);

  auto* v0 = builder.createMOVI(Type::myu64, entry, 0);
  auto* v1 = builder.createMOVI(Type::myu64, entry, 1);
  // TODO: add casting from PARAMS
  // builder.createCAST(Type::myu64, entry);
  auto* v2 = builder.createCMP(Type::myu64, loop, v0, v1);
  auto* v3 = builder.createJA(loop, loop, done);
  auto* v4 = builder.createMUL(Type::myu64, loop, v0, v1);
  auto* v5 = builder.createADDI(Type::myu64, loop, v1, 1);
  auto* v6 = builder.createJUMP(loop, loop);
  auto* v7 = builder.createRET(Type::myu64, done, v0);

  return graph;
}

TEST(FactorialGraphTest, BasicAssertions) {
  custom::Graph* graph = buildFactorialGraph();

  custom::BasicBlock* entry = graph->get_block(0);
  custom::BasicBlock* loop = graph->get_block(1);
  custom::BasicBlock* done = graph->get_block(2);

  // TODO: add assertions

  ASSERT_EQ(entry->get_succs_true(), loop);
  ASSERT_EQ(loop->get_succs_false(), loop);
  ASSERT_EQ(loop->get_succs_true(), done);

  ASSERT_EQ(entry->get_id(), 0);
  ASSERT_EQ(loop->get_id(), 1);
  ASSERT_EQ(done->get_id(), 2);

  delete graph;
}

}  // namespace custom