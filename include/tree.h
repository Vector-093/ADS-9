// Copyright 2022 NNTU-CS
#ifndef INCLUDE_TREE_H_
#define INCLUDE_TREE_H_

#include <vector>

struct PermNode {
  char symbol;
  std::vector<PermNode*> kids;
  size_t subtreeCount;

  explicit PermNode(char v);
  ~PermNode();
};

class PermutationTree {
 public:
  explicit PermutationTree(const std::vector<char>& symbols);

  PermutationTree(const PermutationTree&) = delete;
  PermutationTree& operator=(const PermutationTree&) = delete;

  ~PermutationTree();

  std::vector<std::vector<char>> allPermutations() const;

  PermNode* rootNode() const;

 private:
  PermNode* root;

  std::vector<PermNode*> createChildren(std::vector<char> avail);
  void gatherPerms(PermNode* node, std::vector<char>& curr,
                   std::vector<std::vector<char>>& out) const;
};

std::vector<char> getPermSlow(PermutationTree& tree, int num);

std::vector<char> getPermFast(PermutationTree& tree, int num);

#endif  // INCLUDE_TREE_H_
