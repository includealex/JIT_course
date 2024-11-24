#ifndef INCLUDES_DFS_RPO_HPP_
#define INCLUDES_DFS_RPO_HPP_

#include <algorithm>
#include <cstddef>
#include <stack>
#include <unordered_set>
#include <vector>

#include "basic_block.hpp"

namespace custom {

class RPO final {
 public:
  void run_rpo(BasicBlock* block, BasicBlock* excluded_block = nullptr);
  std::vector<std::size_t> get_rpo_ids_arr();
  void run_reversed_rpo(BasicBlock* start, BasicBlock* end);

 private:
  std::vector<std::size_t> _rpo_ids;

  void get_rpo_ids(BasicBlock* block, std::size_t exclude_id = -1);
  void clear_all_markers(BasicBlock* block);
  void clear_rpo_markers(BasicBlock* block);
  void reversed_dfs(BasicBlock* block,
                    std::unordered_set<BasicBlock*>& visited,
                    std::vector<BasicBlock*>& post_order);
};

}  // namespace custom

#endif  // INCLUDES_DFS_RPO_HPP_
