#pragma once

#include <list>
#include <string>
#include <unordered_set>

#include "SlidingTilePuzzle.h"

class Solver {
public:
  Solver(int size, std::vector<int> puzzle);
  std::string solution();

  virtual void init() = 0;
  virtual std::string mySolution() = 0;

protected:
  int _size;
  SlidingTilePuzzle _puzzle;
  int numExpansions = 0;
  std::unordered_set<std::vector<int>> _expanded;
  int _numExpansions = 0;
};
