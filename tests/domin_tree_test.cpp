#include <gtest/gtest.h>
#include "../includes/ir_builder.hpp"

TEST(DominTreeTest, FirstExample) {
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

    ASSERT_EQ(dt.get_domin_succs(A->get_id()), (std::vector<size_t>{B->get_id()}));
    ASSERT_EQ(dt.get_domin_succs(B->get_id()), (std::vector<size_t>{C->get_id(), D->get_id(), F->get_id()}));
    ASSERT_EQ(dt.get_domin_succs(F->get_id()), (std::vector<size_t>{E->get_id(), G->get_id()}));

    delete graph;
}

TEST(DominTreeTest, SecondExample) {
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
    H->add_succs_true(A);
    I->add_succs_true(K);
    J->add_succs_true(C);

    custom::DominTree dt;
    dt.build_tree(graph);

    ASSERT_EQ(dt.get_domin_succs(A->get_id()), (std::vector<size_t>{B->get_id()}));
    ASSERT_EQ(dt.get_domin_succs(B->get_id()), (std::vector<size_t>{C->get_id(), J->get_id()}));
    ASSERT_EQ(dt.get_domin_succs(C->get_id()), (std::vector<size_t>{D->get_id()}));
    ASSERT_EQ(dt.get_domin_succs(D->get_id()), (std::vector<size_t>{E->get_id()}));
    ASSERT_EQ(dt.get_domin_succs(E->get_id()), (std::vector<size_t>{F->get_id()}));
    ASSERT_EQ(dt.get_domin_succs(F->get_id()), (std::vector<size_t>{G->get_id()}));
    ASSERT_EQ(dt.get_domin_succs(G->get_id()), (std::vector<size_t>{H->get_id(), I->get_id()}));
    ASSERT_EQ(dt.get_domin_succs(I->get_id()), (std::vector<size_t>{K->get_id()}));

    delete graph;
}

TEST(DominTreeTest, ThirdExample) {
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
    E->add_succs_true(D);
    F->add_succs_true(H);
    F->add_succs_false(B);
    G->add_succs_true(I);
    G->add_succs_false(C);
    H->add_succs_true(G);
    H->add_succs_false(I);

    custom::DominTree dt;
    dt.build_tree(graph);

    ASSERT_EQ(dt.get_domin_succs(A->get_id()), (std::vector<size_t>{B->get_id()}));
    ASSERT_EQ(dt.get_domin_succs(B->get_id()), (std::vector<size_t>{C->get_id(), D->get_id(), E->get_id(), G->get_id(), I->get_id()}));
    ASSERT_EQ(dt.get_domin_succs(E->get_id()), (std::vector<size_t>{F->get_id()}));
    ASSERT_EQ(dt.get_domin_succs(F->get_id()), (std::vector<size_t>{H->get_id()}));

    delete graph;
}

