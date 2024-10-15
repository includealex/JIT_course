#include <gtest/gtest.h>
#include "../includes/ir_builder.hpp"

custom::Graph* simplest_graph() {
    custom::Graph* graph = custom::IRBuilder::createGraph();
    custom::BasicBlock* entry = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* loop = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* done = custom::IRBuilder::createBasicBlock(graph);

    entry->add_succs_true(loop);
    loop->push_preds_back(entry);
    loop->add_succs_false(loop);
    loop->add_succs_true(done);
    done->push_preds_back(loop);

    return graph;
}

TEST(DFS_TEST, BasicAssertions) {
    custom::Graph* graph = simplest_graph();

    custom::BasicBlock* entry = graph->get_block(0);
    custom::BasicBlock* loop = graph->get_block(1);
    custom::BasicBlock* done = graph->get_block(2); 

    auto dfs_e = custom::DFS();
    dfs_e.run_dfs(entry);
    std::vector<std::size_t> cmp_e = {0, 1, 2};
    ASSERT_EQ(dfs_e.get_dfs_ids_arr(), cmp_e);

    auto dfs_l = custom::DFS();
    dfs_l.run_dfs(loop);
    std::vector<std::size_t> cmp_l = {1, 2};
    ASSERT_EQ(dfs_l.get_dfs_ids_arr(), cmp_l);

    auto dfs_l_e = custom::DFS();
    dfs_l_e.run_dfs_excluded_block(loop, done);
    std::vector<std::size_t> cmp_l_e = {1};
    ASSERT_EQ(dfs_l_e.get_dfs_ids_arr(), cmp_l_e);

    custom::DominTree dt;
    dt.build_tree(graph);
}

