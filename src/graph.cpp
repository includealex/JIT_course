#include "graph.hpp"

namespace custom {

Graph::~Graph() {
  for (BasicBlock* block : _blocks) {
    delete block;
  }
  _blocks.clear();
}

void Graph::addBasicBlock(BasicBlock* block) {
  block->set_graph(this);
  if (basic_blocks_num() == 0) {
    _root = block;
    block->set_id(0);
  } else {
    block->set_id(basic_blocks_num());
  }
  _blocks.push_back(block);
  _blocks_size++;
}

// FIXME: fix it.
// void Graph::remove_block(BasicBlock* block) {
//   if (!block) {
//     std::cout << "WARNING! Tried to remove non-existant block" << std::endl;
//     return;
//   }

//   auto old_root_id = get_root()->get_id();

//   for (auto& pred : block->get_preds()) {
//     if (pred->get_succs_true() == block) {
//       auto succs = block->get_succs_true();
//       if (succs) {
//         pred->add_succs_true(succs);
//       }
//     }
//     if (pred->get_succs_false() == block) {
//       auto succs = block->get_succs_false();
//       if (succs) {
//         pred->add_succs_false(block->get_succs_true());
//       }
//     }
//   }

//   BasicBlock* succ = block->get_succs_true();
//   if (succ) {
//     auto preds = block->get_preds();
//     std::vector<BasicBlock*> updated_preds = {};
//     for (auto& pred : preds) {
//       if (pred != block) {
//         updated_preds.push_back(pred);
//       }
//     }

//     preds = std::move(updated_preds);
//   }

//   std::vector<BasicBlock*> updated_blocks;
//   for (auto& blk : _blocks) {
//     if (blk != block) {
//       updated_blocks.push_back(blk);
//     }
//   }

//   _blocks.clear();
//   _blocks = std::move(updated_blocks);

//   _blocks_size -= 1;

//   auto b_id = block->get_id();
//   delete block;

//   if (b_id == old_root_id) {
//     if (succ) {
//       _root = succ;
//       _root->set_id(0);
//     }
//     else {
//       std::cout << "WARNING! Removed single-rooted basic block" << std::endl;
//     }
//   }
// }

BasicBlock* Graph::get_block(size_t index) const {
  if (index < _blocks.size()) {
    return _blocks[index];
  }
  return nullptr;
}

BasicBlock* Graph::get_root() const {
  return get_block(0);
}

std::size_t Graph::basic_blocks_num() const {
  return _blocks_size;
}

}  // namespace custom
