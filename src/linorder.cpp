#include "linorder.hpp"

namespace custom {
std::vector<size_t> Linorder::get_linorder(Graph* graph) {
  fill_linorder(graph);
  return _linorder;
}

void Linorder::fill_linorder(Graph* graph) {
  LoopTree ltree;
  DominTree dtree;

  dtree.build_tree(graph);
  auto preorder_ids_dtree = dtree.preorder_traversal();

  ltree.build_tree(graph);

  auto preloop_vec = ltree.loop_vector;
  std::vector<custom::LTNode*> loop_vec;
  for (auto& el : preloop_vec) {
    if (el->idx == -1)
      continue;
    loop_vec.push_back(el);
  }

  std::vector<size_t> headers_ids;
  for (auto& loop : loop_vec) {
    headers_ids.push_back(loop->get_header_id());
  }

  std::vector<size_t> result_linorder_array;
  std::vector<size_t> visited_elems;

  auto traverse_loop = [&](LTNode* loop, auto&& traverse_loop_ref) -> void {
    if (!loop)
      return;

    size_t header_id = loop->get_header_id();
    if (std::find(visited_elems.begin(), visited_elems.end(), header_id) == visited_elems.end()) {
      result_linorder_array.push_back(header_id);
      visited_elems.push_back(header_id);
    }

    auto loop_ids = loop->get_blocks_id();
    auto cur_latches = loop->get_latches_id();

    std::vector<size_t> non_latch_ids;
    for (auto& loop_id : loop_ids) {
      if (std::find(cur_latches.begin(), cur_latches.end(), loop_id) == cur_latches.end()) {
        non_latch_ids.push_back(loop_id);
      }
    }

    for (auto& inner_loop : loop->succs) {
      traverse_loop_ref(inner_loop, traverse_loop_ref);
      break;
    }

    for (auto& body_id : non_latch_ids) {
      if (std::find(visited_elems.begin(), visited_elems.end(), body_id) == visited_elems.end()) {
        result_linorder_array.push_back(body_id);
        visited_elems.push_back(body_id);
      }
    }

    for (auto& latch_id : cur_latches) {
      if (std::find(visited_elems.begin(), visited_elems.end(), latch_id) == visited_elems.end()) {
        result_linorder_array.push_back(latch_id);
        visited_elems.push_back(latch_id);
      }
    }
  };

  for (auto& el_id : preorder_ids_dtree) {
    if (std::find(visited_elems.begin(), visited_elems.end(), el_id) != visited_elems.end()) {
      continue;
    }

    if (std::find(headers_ids.begin(), headers_ids.end(), el_id) != headers_ids.end()) {
      LTNode* cur_loop = nullptr;
      for (auto& loop : loop_vec) {
        if (loop->get_header_id() == el_id) {
          cur_loop = loop;
          break;
        }
      }

      if (!cur_loop) {
        continue;
      }

      auto preds = graph->get_block(el_id)->get_preds();
      bool all_preds_visited = true;

      for (auto& pr : preds) {
        size_t pr_id = pr->get_id();

        auto cur_latches = cur_loop->get_latches_id();
        if (std::find(cur_latches.begin(), cur_latches.end(), pr_id) != cur_latches.end()) {
          continue;
        }

        if (std::find(visited_elems.begin(), visited_elems.end(), pr_id) == visited_elems.end()) {
          all_preds_visited = false;

          auto pred_subtree = dtree.get_subtree(pr_id);
          for (auto& subtree_id : pred_subtree) {
            if (std::find(visited_elems.begin(), visited_elems.end(), subtree_id) ==
                visited_elems.end()) {
              visited_elems.push_back(subtree_id);
              result_linorder_array.push_back(subtree_id);
            }
          }
        }
      }

      if (all_preds_visited) {
        traverse_loop(cur_loop, traverse_loop);
      }
    } else {
      if (std::find(visited_elems.begin(), visited_elems.end(), el_id) == visited_elems.end()) {
        result_linorder_array.push_back(el_id);
        visited_elems.push_back(el_id);
      }
    }
  }

  _linorder = result_linorder_array;
}

}  // namespace custom
