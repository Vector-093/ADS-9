// Copyright 2022 NNTU-CS
#include <algorithm>
#include <vector>
#include "tree.h"

PermNode::PermNode(char v) : symbol(v), subtreeCount(0) {}

PermNode::~PermNode() {
  for (PermNode* kid : kids) delete kid;
}

PermutationTree::PermutationTree(const std::vector<char>& symbols) {
  std::vector<char> sortedSyms = symbols;
  std::sort(sortedSyms.begin(), sortedSyms.end());
  root = new PermNode('\0');
  root->kids = createChildren(sortedSyms);
  root->subtreeCount = 0;
  for (PermNode* kid : root->kids) {
    root->subtreeCount += kid->subtreeCount;
  }
}

PermutationTree::~PermutationTree() {
  delete root;
}

PermNode* PermutationTree::rootNode() const {
  return root;
}

std::vector<PermNode*> PermutationTree::createChildren(std::vector<char> avail) {
  std::vector<PermNode*> result;
  if (avail.empty()) return result;

  for (size_t idx = 0; idx < avail.size(); ++idx) {
    char sym = avail[idx];
    PermNode* node = new PermNode(sym);

    std::vector<char> remaining;
    remaining.reserve(avail.size() - 1);
    for (size_t j = 0; j < avail.size(); ++j) {
      if (j != idx) remaining.push_back(avail[j]);
    }

    node->kids = createChildren(remaining);

    if (node->kids.empty()) {
      node->subtreeCount = 1;
    } else {
      node->subtreeCount = 0;
      for (PermNode* kid : node->kids) {
        node->subtreeCount += kid->subtreeCount;
      }
    }
    result.push_back(node);
  }
  return result;
}

void PermutationTree::gatherPerms(PermNode* node, std::vector<char>& curr,
                                  std::vector<std::vector<char>>& out) const {
  if (node->kids.empty()) {
    out.push_back(curr);
    return;
  }
  for (PermNode* kid : node->kids) {
    curr.push_back(kid->symbol);
    gatherPerms(kid, curr, out);
    curr.pop_back();
  }
}

std::vector<std::vector<char>> PermutationTree::allPermutations() const {
  std::vector<std::vector<char>> output;
  std::vector<char> currentSeq;
  gatherPerms(root, currentSeq, output);
  return output;
}

std::vector<char> getPermSlow(PermutationTree& tree, int n) {
  if (n <= 0) return {};
  auto all = tree.allPermutations();
  if (static_cast<size_t>(n) > all.size()) return {};
  return all[n - 1];
}

std::vector<char> getPermFast(PermutationTree& tree, int n) {
  if (n <= 0) return {};
  PermNode* root = tree.rootNode();
  if (static_cast<size_t>(n) > root->subtreeCount) return {};

  std::vector<char> seq;
  PermNode* cur = root;
  int left = n;

  while (!cur->kids.empty()) {
    for (PermNode* kid : cur->kids) {
      if (kid->subtreeCount >= static_cast<size_t>(left)) {
        seq.push_back(kid->symbol);
        cur = kid;
        break;
      } else {
        left -= static_cast<int>(kid->subtreeCount);
      }
    }
  }
  return seq;
}

