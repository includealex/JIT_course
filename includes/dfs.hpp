#ifndef INCLUDES_DFS_HPP_ 
#define INCLUDES_DFS_HPP_

#include <cstddef>
#include <vector>

#include "basic_block.hpp"

namespace custom {

class DFS final {
public:
    void run_dfs (BasicBlock* block);
    void run_dfs_excluded_block(BasicBlock* block, BasicBlock* excluded_block);

    std::vector<std::size_t> get_dfs_ids_arr();


private:
    std::vector<std::size_t> _dfs_ids;
    void get_dfs_ids (BasicBlock* block, std::size_t exclude_id = -1);
    void clear_markers(BasicBlock* block);
};

} // namespace custom


#endif  // INCLUDES_DFS_HPP_

