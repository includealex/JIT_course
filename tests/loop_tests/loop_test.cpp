#include <gtest/gtest.h>

#include "../includes/ir_builder.hpp"
#include "../includes/loop_tree.hpp"

TEST(LoopAnalyzerTest, FirstExampleDFSCheck) {
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
    custom::BasicBlock* K = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* L = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* O = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* Q = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* R = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* T = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* V = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* W = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* X = custom::IRBuilder::createBasicBlock(graph);

    A->add_succs_true(B);
    B->add_succs_true(C);
    C->add_succs_true(D);
    C->add_succs_false(R);
    D->add_succs_true(E);
    E->add_succs_true(F);
    E->add_succs_false(L);
    F->add_succs_true(H);
    F->add_succs_false(G);
    G->add_succs_true(Q);
    H->add_succs_true(T);
    H->add_succs_false(O);
    I->add_succs_true(A);
    K->add_succs_true(A);
    L->add_succs_true(I);
    O->add_succs_true(E);
    Q->add_succs_true(W);
    R->add_succs_true(B);
    T->add_succs_true(V);
    V->add_succs_true(X);
    V->add_succs_false(K);
    W->add_succs_true(A);

    custom::DFS df;
    df.run_dfs(graph->get_root());

    std::vector<std::pair<std::size_t, std::vector<std::size_t>>> expected_loops = {
        {A->get_id(), {I->get_id(), W->get_id(), K->get_id()}},
        {B->get_id(), {R->get_id()}},
        {E->get_id(), {O->get_id()}}
    };

    auto loops = df.get_loops();

    ASSERT_EQ(loops.size(), expected_loops.size());

    for (const auto& [expected_header, expected_latches] : expected_loops) {
        bool found = false;

        for (auto& loop : loops) {
            if (loop.get_header()->get_id() == expected_header) {
                found = true;

                std::vector<std::size_t> actual_latches;
                for (const auto& latch : loop.get_latches()) {
                    actual_latches.push_back(latch->get_id());
                }

                std::sort(actual_latches.begin(), actual_latches.end());
                auto sorted_expected_latches = expected_latches;
                std::sort(sorted_expected_latches.begin(), sorted_expected_latches.end());

                EXPECT_EQ(actual_latches, sorted_expected_latches);
                break;
            }
        }
        EXPECT_TRUE(found);
    }

    delete graph;
}

TEST(LoopAnalyzerTest, FirstExample) {
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
    custom::BasicBlock* K = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* L = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* O = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* Q = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* R = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* T = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* V = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* W = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* X = custom::IRBuilder::createBasicBlock(graph);

    A->add_succs_true(B);
    B->add_succs_true(C);
    C->add_succs_true(D);
    C->add_succs_false(R);
    D->add_succs_true(E);
    E->add_succs_true(F);
    E->add_succs_false(L);
    F->add_succs_true(H);
    F->add_succs_false(G);
    G->add_succs_true(Q);
    H->add_succs_true(T);
    H->add_succs_false(O);
    I->add_succs_true(A);
    K->add_succs_true(A);
    L->add_succs_true(I);
    O->add_succs_true(E);
    Q->add_succs_true(W);
    R->add_succs_true(B);
    T->add_succs_true(V);
    V->add_succs_true(X);
    V->add_succs_false(K);
    W->add_succs_true(A);

    custom::LoopTree lt;
    lt.build_tree(graph);

    auto root = lt.get_root();

    ASSERT_NE(root, nullptr) << "Root node should be initialized";

    ASSERT_FALSE(root->get_blocks_id().empty()) << "Root should contain non-loop blocks";
    ASSERT_EQ(root->succs.size(), 1);

    auto& first_child = root->succs[0];
    ASSERT_EQ(first_child->get_header_id(), A->get_id());
    std::vector<size_t> expected_blocks = {L->get_id(), V->get_id(), D->get_id(), Q->get_id(), T->get_id(), G->get_id()};
    std::vector<size_t> expected_latches = {I->get_id(), W->get_id(), K->get_id()};
    auto real_blocks = first_child->get_blocks_id();
    auto real_latches = first_child->get_latches_id();

    auto all_latches_found = std::all_of(expected_latches.begin(), expected_latches.end(), [&real_latches](int a_elt) {
        return std::find(real_latches.begin(), real_latches.end(), a_elt) != real_latches.end();
    });
    ASSERT_TRUE(all_latches_found);

    auto all_blocks_found = std::all_of(expected_blocks.begin(), expected_blocks.end(), [&real_blocks](int a_elt) {
        return std::find(real_blocks.begin(), real_blocks.end(), a_elt) != real_blocks.end();
    });
    ASSERT_TRUE(all_blocks_found);

    ASSERT_EQ(first_child->succs.size(), C->get_id());
    for (const auto& successor : first_child->succs) {
        if (successor->get_header_id() == (E->get_id())) {
            auto real = successor->get_blocks_id();
            std::vector<size_t> expected = {F->get_id(), H->get_id()};
            auto each_block = std::all_of(expected.begin(), expected.end(), [&real](int a_elt) {
                return std::find(real.begin(), real.end(), a_elt) != real.end();
            });
            ASSERT_TRUE(each_block);

            ASSERT_EQ(successor->get_latches_id(), std::vector<size_t>{O->get_id()});
        } else if (successor->get_header_id() == (B->get_id())) {
            ASSERT_EQ(successor->get_blocks_id(), std::vector<size_t>{C->get_id()});
            ASSERT_EQ(successor->get_latches_id(), std::vector<size_t>{R->get_id()});
        }
    }

    delete graph;
}

TEST(LoopAnalyzerTest, SecondExample) {
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
    E->add_succs_true(D);
    F->add_succs_false(E);
    F->add_succs_true(G);
    G->add_succs_true(D);

    custom::LoopTree lt;
    lt.build_tree(graph);

    auto root = lt.get_root();

    ASSERT_NE(root, nullptr);
    auto real = root->get_blocks_id();

    ASSERT_FALSE(real.empty());
    ASSERT_EQ(root->succs.size(), 0);

    std::vector<std::size_t> expected = {A->get_id(), B->get_id(), C->get_id(), D->get_id(), E->get_id(), F->get_id(), G->get_id()};
    auto each_block = std::all_of(expected.begin(), expected.end(), [&real](int a_elt) {
        return std::find(real.begin(), real.end(), a_elt) != real.end();
    });
    ASSERT_TRUE(each_block);

    delete graph;
}

TEST(LoopAnalyzerTest, ThirdExample) {
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
    H->add_succs_true(B);
    I->add_succs_true(K);
    J->add_succs_true(C);

    custom::LoopTree lt;
    lt.build_tree(graph);

    auto root = lt.get_root();

    std::vector<size_t> root_blocks_expected = {A->get_id(), K->get_id(), I->get_id()}; 
    auto root_blocks_real= root->get_blocks_id();

    auto each_block = std::all_of(root_blocks_expected.begin(), root_blocks_expected.end(), [&root_blocks_real](int a_elt) {
        return std::find(root_blocks_real.begin(), root_blocks_real.end(), a_elt) != root_blocks_real.end();
    });
    ASSERT_TRUE(each_block);

    auto& first_child = root->succs[0];
    for (const auto& successor : first_child->succs) {
        if (successor->get_header_id() == C->get_id()) {
            ASSERT_EQ(successor->get_latches_id(), std::vector<size_t>{D->get_id()});
            ASSERT_EQ(successor->get_blocks_id().size(), 0);
        } else if (successor->idx == static_cast<int>(E->get_id())) {
            ASSERT_EQ(successor->get_latches_id(), std::vector<size_t>{F->get_id()});
            ASSERT_EQ(successor->get_blocks_id().size(), 0);
        }
    }

    delete graph;
}

TEST(LoopAnalyzerTest, FourthExample) {
    custom::Graph* graph = custom::IRBuilder::createGraph();
    custom::BasicBlock* A = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* B = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* C = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* D = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* E = custom::IRBuilder::createBasicBlock(graph);

    A->add_succs_true(B);
    B->add_succs_true(D);
    B->add_succs_false(C);
    D->add_succs_true(E);
    E->add_succs_true(B);

    custom::LoopTree lt;
    lt.build_tree(graph);

    auto root = lt.get_root();
    ASSERT_EQ(root->succs.size(), 1);

    std::vector<size_t> root_blocks_expected = {A->get_id(), C->get_id()}; 
    auto root_blocks_real= root->get_blocks_id();

    auto each_block = std::all_of(root_blocks_expected.begin(), root_blocks_expected.end(), [&root_blocks_real](int a_elt) {
        return std::find(root_blocks_real.begin(), root_blocks_real.end(), a_elt) != root_blocks_real.end();
    });
    ASSERT_TRUE(each_block);

    auto& first_child = root->succs[0];
    ASSERT_EQ(first_child->get_header_id(), (B->get_id()));
    ASSERT_EQ(first_child->get_blocks_id(), (std::vector<size_t>{D->get_id()}));
    ASSERT_EQ(first_child->get_latches_id(), (std::vector<size_t>{E->get_id()}));

    delete graph;
}

TEST(LoopAnalyzerTest, FifthExample) {
    custom::Graph* graph = custom::IRBuilder::createGraph();
    custom::BasicBlock* A = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* B = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* C = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* D = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* E = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* F = custom::IRBuilder::createBasicBlock(graph);

    A->add_succs_true(B);
    B->add_succs_true(C);
    C->add_succs_true(D);
    C->add_succs_false(F);
    D->add_succs_true(F);
    D->add_succs_false(E);
    E->add_succs_true(B);

    custom::LoopTree lt;
    lt.build_tree(graph);

    auto root = lt.get_root();
    ASSERT_EQ(root->succs.size(), 1);

    std::vector<size_t> root_blocks_expected = {A->get_id(), F->get_id()}; 
    auto root_blocks_real= root->get_blocks_id();

    auto root_each_block = std::all_of(root_blocks_expected.begin(), root_blocks_expected.end(), [&root_blocks_real](int a_elt) {
        return std::find(root_blocks_real.begin(), root_blocks_real.end(), a_elt) != root_blocks_real.end();
    });
    ASSERT_TRUE(root_each_block);

    auto& first_child = root->succs[0];
    ASSERT_EQ(first_child->get_header_id(), (B->get_id()));
    ASSERT_EQ(first_child->get_latches_id(), (std::vector<size_t>{E->get_id()}));

    std::vector<size_t> blocks_expected = {C->get_id(), D->get_id()}; 
    auto blocks_real= first_child->get_blocks_id();

    auto each_block = std::all_of(blocks_expected.begin(), blocks_expected.end(), [&blocks_real](int a_elt) {
        return std::find(blocks_real.begin(), blocks_real.end(), a_elt) != blocks_real.end();
    });
    ASSERT_TRUE(each_block);

    delete graph;
}

TEST(LoopAnalyzerTest, SixthExample) {
    custom::Graph* graph = custom::IRBuilder::createGraph();
    custom::BasicBlock* A = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* B = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* C = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* D = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* E = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* F = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* G = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* H = custom::IRBuilder::createBasicBlock(graph);

    A->add_succs_true(B);
    B->add_succs_false(C);
    B->add_succs_true(D);
    C->add_succs_true(F);
    C->add_succs_false(E);
    D->add_succs_true(F);
    F->add_succs_true(G);
    G->add_succs_true(H);
    G->add_succs_false(B);
    H->add_succs_true(A);

    custom::LoopTree lt;
    lt.build_tree(graph);

    auto root = lt.get_root();
    ASSERT_EQ(root->succs.size(), 1);
    ASSERT_EQ(root->get_blocks_id(), std::vector<std::size_t>{E->get_id()});

    auto& first_child = root->succs[0];
    ASSERT_EQ(first_child->get_header_id(), (A->get_id()));
    ASSERT_EQ(first_child->get_latches_id(), (std::vector<size_t>{H->get_id()}));
    ASSERT_EQ(first_child->get_blocks_id().size(), 0);

    auto& second_child = first_child->succs[0];
    ASSERT_EQ(second_child->get_header_id(), (B->get_id()));
    ASSERT_EQ(second_child->get_latches_id(), (std::vector<size_t>{G->get_id()}));

    std::vector<size_t> blocks_expected = {C->get_id(), D->get_id(), F->get_id()}; 
    auto blocks_real= second_child->get_blocks_id();

    auto each_block = std::all_of(blocks_expected.begin(), blocks_expected.end(), [&blocks_real](int a_elt) {
        return std::find(blocks_real.begin(), blocks_real.end(), a_elt) != blocks_real.end();
    });
    ASSERT_TRUE(each_block);

    delete graph;
}
