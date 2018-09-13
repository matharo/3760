#pragma once

#include <vector>
#include <string>

class Node {
public:
  Node(std::vector<int> state, std::string path);

  const std::vector<int> &getState() const;
  std::string getPath() const;

private:
  std::vector<int> _state;
  std::string _path;
};
