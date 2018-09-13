#include <algorithm>
#include <iostream>
#include <random>

#include "MySolver.h"

MySolver::MySolver(int size, std::vector<int> puzzle) : Solver(size, puzzle) {}

void MySolver::init() {
  // Put any initialization code here
}

std::string MySolver::mySolution() {
  // You can rewrite this to use whatever algorithm you need to.
  // Right now it randomly selects frontier nodes (not a good algorithm)

  Node initial(_puzzle.getInitial(), "");

  // Initialize random number generator, only needed because this
  // solver uses randomized behavior.
  std::random_device rd;
  std::mt19937 gen(rd());

  //_frontier.push_back(initial);
  _frontier.push(initial);
  int count = 0;
  while (!_frontier.empty()) {
    count++;
    // Select a random element of the frontier and remove it from the
    // frontier.  Note you will need to change this since you will
    // probably not be using a vector to store your frontier.
    //std::uniform_int_distribution<> dis(0, _frontier.size() - 1);
    //int randomIndex = dis(gen);
    //Node current = _frontier[randomIndex];
    //_frontier[randomIndex] = _frontier[_frontier.size() - 1];
    //_frontier.pop_back();

    Node current = _frontier.front();
    _frontier.pop();

    // Check if we have reached the goal
    if (_puzzle.isGoal(current.getState()))
      return current.getPath();

    if (count % 100000 == 0)
      std::cout << _numExpansions << " nodes examined.  " << _frontier.size()
                << " nodes on the frontier" << std::endl;

    // _expand adds unexpanded neighbors to the frontier
    _expand(current);
  }

  return "";
}

int MySolver::_heuristic(const std::vector<int> state) const {
  // Implement this for use in your informed search your informed search
  return 0;
}

void MySolver::_expand(Node &node) {
  std::vector<int> state = node.getState();
  if (_expanded.find(state) != _expanded.end())
    return;
  _numExpansions++;
  _expanded.insert(state);

  for (auto action : _puzzle.actions(state)) {
    auto newState = _puzzle.result(state, action);

    if (_expanded.find(newState) == _expanded.end()) {
      Node newNode(newState, node.getPath() + action);

      // The following line will probably need to be changed
      // depending on the data structure you use for _frontier.
      // Do not make any other changes to this function.
      //_frontier.push_back(newNode);
      _frontier.push(newNode);
    }
  }
}
