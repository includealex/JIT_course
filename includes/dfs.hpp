#ifndef INCLUDES_DFS_HPP_ 
#define INCLUDES_DFS_HPP_

#include <cstddef>
#include <stack>
#include <unordered_set>
#include <vector>

#include "basic_block.hpp"
#include "loop.hpp"

namespace custom {

class DFS final {
public:
    void run_dfs(BasicBlock* block, BasicBlock* excluded_block = nullptr);
    std::vector<std::size_t> get_dfs_ids_arr();

    void add_loop(BasicBlock* header, BasicBlock* latch);
    const std::vector<Loop>& get_loops() const;

    void populate_graph();

    std::vector<BasicBlock*> get_basic_blocks_between(BasicBlock* start, BasicBlock* end);

private:
    std::vector<std::size_t> _dfs_ids;
    void get_dfs_ids (BasicBlock* block, std::size_t exclude_id = -1);
    void clear_all_markers(BasicBlock* block);
    void clear_dfs_markers(BasicBlock* block);
    void clear_loop_gray_black_markers(BasicBlock* block);

    void dfs_helper(BasicBlock* block, BasicBlock* end, std::vector<BasicBlock*>& path, 
                    std::unordered_set<BasicBlock*>& visited, bool& found);

    std::vector<Loop> _loops;
};

} // namespace custom


#endif  // INCLUDES_DFS_HPP_

