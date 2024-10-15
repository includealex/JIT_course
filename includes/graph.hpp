#ifndef INCLUDES_GRAPH_HPP_
#define INCLUDES_GRAPH_HPP_

#include <cstddef>
#include <vector>

namespace custom {

class BasicBlock;

class Graph final {
 public:
    void addBasicBlock(BasicBlock* block);

    BasicBlock* get_block(std::size_t idx);

    std::vector<BasicBlock*> _blocks;
 private:
};

} // namespace custom

#endif // INCLUDES_GRAPH_HPP_

