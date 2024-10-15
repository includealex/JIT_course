#ifndef INCLUDES_DFS_HPP_ 
#define INCLUDES_DFS_HPP_

#include <cstddef>
#include <vector>

#include "basic_block.hpp"

namespace custom {

class DFS final {
public:
    void run_dfs (BasicBlock* block);

    std::vector<std::size_t> get_dfs_ids();

private:
    std::vector<std::size_t> _dfs_ids;
    void get_dfs_ids (BasicBlock* block);
    void clear_markers(BasicBlock* block);
};

} // namespace custom


#endif  // INCLUDES_DFS_HPP_

