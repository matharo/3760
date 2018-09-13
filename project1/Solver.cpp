#include <chrono>
#include <iostream>
#include <memory>
#include <utility>

#include "Node.h"
#include "Solver.h"

Solver::Solver(int size, std::vector<int> puzzle)
    : _puzzle(size, std::move(puzzle)), _size(size) {}

std::string Solver::solution() {
  using namespace std::chrono;

  auto initStart = high_resolution_clock::now();
  init();
  double initTime =
      duration_cast<duration<double>>(high_resolution_clock::now() - initStart)
          .count();

  auto startTime = high_resolution_clock::now();
  auto moveSequence = mySolution();
  double solutionTime =
      duration_cast<duration<double>>(high_resolution_clock::now() - startTime)
          .count();

  std::cout << "Initialization time = " << initTime << " seconds" << std::endl;
  std::cout << "Solution length = " << moveSequence.size() << std::endl;
  std::cout << "Number of nodes expanded = " << _numExpansions << std::endl;
  std::cout << "Search time per node = " << (solutionTime / _numExpansions)
            << std::endl;
  std::cout << "Search time = " << solutionTime << std::endl;

  return moveSequence;
}
