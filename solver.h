#pragma once

//#include <vector>
#include <queue>

#include "Node.h"
#include "Solver.h"

class MySolver : public Solver {
public:
  MySolver(int size, std::vector<int> puzzle);
  void init();
  std::string mySolution();

private:
  void _expand(Node &node);
  int _heuristic(const std::vector<int> state) const;

private:
  // Change this to whatever data stucture you need.  However make
  // sure to keep the type mutable to do interfere with const-correctness.
  //mutable std::vector<Node> _frontier;
  mutable std::queue<Node> _frontier;
};
