#ifndef GRAPH_HPP_
#define GRAPH_HPP_

#include <vector>

namespace custom {

class BasicBlock;

class Graph final {
 public:
    void addBasicBlock(BasicBlock* block);

    BasicBlock* get_block(std::size_t idx);

 private:
    std::vector<BasicBlock*> _blocks;
};

} // namespace custom

#endif // GRAPH_HPP_

