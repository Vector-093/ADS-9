// Copyright 2022 NNTU-CS
#ifndef INCLUDE_TREE_H_
#define INCLUDE_TREE_H_

#include <vector>

struct Node {
  char label;
  std::vector<Node*> branches;
  size_t treeSize;

  explicit Node(char v);
  ~Node();
};

class PMTree {
 public:
  explicit PMTree(const std::vector<char>& symbols);

  PMTree(const PMTree&) = delete;
  PMTree& operator=(const PMTree&) = delete;

  ~PMTree();

  std::vector<std::vector<char>> getAllPerms() const;

  Node* getRoot() const;

 private:
  Node* root_;

  std::vector<Node*> buildNodes(std::vector<char> remaining);
  void collectAll(Node* node, std::vector<char>& current,
                  std::vector<std::vector<char>>& output) const;
};

std::vector<char> getPerm1(PMTree& tree, int num);

std::vector<char> getPerm2(PMTree& tree, int num);

#endif  // INCLUDE_TREE_H_
