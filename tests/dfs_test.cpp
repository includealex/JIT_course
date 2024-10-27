#include <gtest/gtest.h>
#include "../includes/ir_builder.hpp"

TEST(DFS_Test, FirstExample) {
    custom::Graph* graph = custom::IRBuilder::createGraph();
    custom::BasicBlock* A = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* B = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* C = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* D = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* E = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* F = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* G = custom::IRBuilder::createBasicBlock(graph);

    A->add_succs_true(B);
    B->add_succs_false(C);
    B->add_succs_true(F);
    C->add_succs_true(D);
    F->add_succs_false(E);
    F->add_succs_true(G);
    G->add_succs_true(D);

    custom::DominTree dt;
    dt.build_tree(graph);

    ASSERT_EQ(dt.get_domin_succs(0), (std::vector<size_t>{1}));
    ASSERT_EQ(dt.get_domin_succs(1), (std::vector<size_t>{2, 3, 5}));
    ASSERT_EQ(dt.get_domin_succs(5), (std::vector<size_t>{4, 6}));
}

TEST(DFS_Test, SecondExample) {
    custom::Graph* graph = custom::IRBuilder::createGraph();
    custom::BasicBlock* A = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* B = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* C = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* D = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* E = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* F = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* G = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* H = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* I = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* J = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* K = custom::IRBuilder::createBasicBlock(graph);
    
    A->add_succs_true(B);
    B->add_succs_true(J);
    B->add_succs_false(C);
    C->add_succs_true(D);
    D->add_succs_true(E);
    D->add_succs_false(C);
    E->add_succs_true(F);
    F->add_succs_true(G);
    F->add_succs_false(E);
    G->add_succs_true(I);
    G->add_succs_false(H);
    H->add_succs_true(A);
    I->add_succs_true(K);
    J->add_succs_true(C);

    custom::DominTree dt;
    dt.build_tree(graph);

    ASSERT_EQ(dt.get_domin_succs(0), (std::vector<size_t>{1}));
    ASSERT_EQ(dt.get_domin_succs(1), (std::vector<size_t>{2, 9}));
    ASSERT_EQ(dt.get_domin_succs(2), (std::vector<size_t>{3}));
    ASSERT_EQ(dt.get_domin_succs(3), (std::vector<size_t>{4}));
    ASSERT_EQ(dt.get_domin_succs(4), (std::vector<size_t>{5}));
    ASSERT_EQ(dt.get_domin_succs(5), (std::vector<size_t>{6}));
    ASSERT_EQ(dt.get_domin_succs(6), (std::vector<size_t>{7, 8}));
    ASSERT_EQ(dt.get_domin_succs(8), (std::vector<size_t>{10}));
}

TEST(DFS_Test, ThirdExample) {
    custom::Graph* graph = custom::IRBuilder::createGraph();
    custom::BasicBlock* A = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* B = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* C = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* D = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* E = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* F = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* G = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* H = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* I = custom::IRBuilder::createBasicBlock(graph);

    A->add_succs_true(B);
    B->add_succs_true(C);
    B->add_succs_false(E);
    C->add_succs_true(D);
    D->add_succs_true(G);
    E->add_succs_false(F);
    E->add_succs_true(D);
    F->add_succs_true(H);
    F->add_succs_false(B);
    G->add_succs_true(I);
    G->add_succs_false(C);
    H->add_succs_true(G);
    H->add_succs_false(I);

    custom::DominTree dt;
    dt.build_tree(graph);

    ASSERT_EQ(dt.get_domin_succs(0), (std::vector<size_t>{1}));
    ASSERT_EQ(dt.get_domin_succs(1), (std::vector<size_t>{2, 3, 4, 6, 8}));
    ASSERT_EQ(dt.get_domin_succs(4), (std::vector<size_t>{5}));
    ASSERT_EQ(dt.get_domin_succs(5), (std::vector<size_t>{7}));
}

