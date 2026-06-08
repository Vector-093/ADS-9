// Copyright 2022 NNTU-CS
#include <algorithm>
#include <vector>
#include "tree.h"

Node::Node(char v) : label(v), treeSize(0) {}

Node::~Node() {
  for (Node* child : branches) delete child;
}

PMTree::PMTree(const std::vector<char>& symbols) {
  std::vector<char> sorted = symbols;
  std::sort(sorted.begin(), sorted.end());
  root_ = new Node('\0');
  root_->branches = buildNodes(sorted);
  root_->treeSize = 0;
  for (Node* child : root_->branches) {
    root_->treeSize += child->treeSize;
  }
}

PMTree::~PMTree() {
  delete root_;
}

Node* PMTree::getRoot() const {
  return root_;
}

std::vector<Node*> PMTree::buildNodes(
std::vector<char> remaining) {
  std::vector<Node*> nodes;
  if (remaining.empty()) return nodes;

  for (size_t i = 0; i < remaining.size(); ++i) {
    char c = remaining[i];
    Node* node = new Node(c);

    std::vector<char> nextRemaining;
    nextRemaining.reserve(remaining.size() - 1);
    for (size_t j = 0; j < remaining.size(); ++j) {
      if (j != i) nextRemaining.push_back(remaining[j]);
    }

    node->branches = buildNodes(nextRemaining);

    if (node->branches.empty()) {
      node->treeSize = 1;
    } else {
      node->treeSize = 0;
      for (Node* child : node->branches) {
        node->treeSize += child->treeSize;
      }
    }
    nodes.push_back(node);
  }
  return nodes;
}

void PMTree::collectAll(Node* node, std::vector<char>& current,
                        std::vector<std::vector<char>>& output) const {
  if (node->branches.empty()) {
    output.push_back(current);
    return;
  }
  for (Node* child : node->branches) {
    current.push_back(child->label);
    collectAll(child, current, output);
    current.pop_back();
  }
}

std::vector<std::vector<char>> PMTree::getAllPerms() const {
  std::vector<std::vector<char>> result;
  std::vector<char> path;
  collectAll(root_, path, result);
  return result;
}

std::vector<char> getPerm1(PMTree& tree, int num) {
  if (num <= 0) return {};
  auto all = tree.getAllPerms();
  if (static_cast<size_t>(num) > all.size()) return {};
  return all[num - 1];
}

std::vector<char> getPerm2(PMTree& tree, int num) {
  if (num <= 0) return {};
  Node* root = tree.getRoot();
  if (static_cast<size_t>(num) > root->treeSize) return {};

  std::vector<char> seq;
  Node* cur = root;
  int left = num;

  while (!cur->branches.empty()) {
    for (Node* child : cur->branches) {
      if (child->treeSize >= static_cast<size_t>(left)) {
        seq.push_back(child->label);
        cur = child;
        break;
      } else {
        left -= static_cast<int>(child->treeSize);
      }
    }
  }
  return seq;
}

