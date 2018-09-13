#include <algorithm>
#include <array>
#include <random>

#include "SlidingTilePuzzle.h"

SlidingTilePuzzle::SlidingTilePuzzle(int size, std::vector<int> initial)
    : _size(size), _initial(std::move(initial)) {

  for (int i = 0; i < size * size; i++) {
    _goal.push_back(i);
  }
}

std::vector<int> SlidingTilePuzzle::getInitial() const { return _initial; }

bool SlidingTilePuzzle::isGoal(const std::vector<int> &state) const {
  for (int i = 0; i < _size * _size; i++)
    if (state[i] != i)
      return false;

  return true;
}

std::vector<char>
SlidingTilePuzzle::actions(const std::vector<int> &state) const {
  auto coords = _emptyTileCoordinates(state);

  std::vector<char> actions;
  if (coords[0] > 0) {
    actions.push_back('D');
  }
  if (coords[0] < _size - 1) {
    actions.push_back('U');
  }
  if (coords[1] > 0) {
    actions.push_back('R');
  }
  if (coords[1] < _size - 1) {
    actions.push_back('L');
  }

  return actions;
}

std::vector<int> SlidingTilePuzzle::result(const std::vector<int> &state,
                                           char action) const {
  int emptyTileIndex = std::find(state.begin(), state.end(), 0) - state.begin();
  std::vector<int> newState{state};

  switch (action) {
  case 'D':
    newState[emptyTileIndex] = state[emptyTileIndex - _size];
    newState[emptyTileIndex - _size] = 0;
    return newState;
  case 'U':
    newState[emptyTileIndex] = state[emptyTileIndex + _size];
    newState[emptyTileIndex + _size] = 0;
    return newState;
  case 'R':
    newState[emptyTileIndex] = state[emptyTileIndex - 1];
    newState[emptyTileIndex - 1] = 0;
    return newState;
  default:
    newState[emptyTileIndex] = state[emptyTileIndex + 1];
    newState[emptyTileIndex + 1] = 0;
    return newState;
  }
}

std::vector<int> SlidingTilePuzzle::randomState() const {
  std::random_device rd;
  std::mt19937 gen(rd());
  auto state = getInitial();
  for (int i = 0; i < 1000; i++) {
    auto possibleActions = actions(state);
    std::uniform_int_distribution<> dis(0, possibleActions.size() - 1);
    state = result(state, possibleActions[dis(gen)]);
  }
  return state;
}

std::array<int, 2>
SlidingTilePuzzle::_emptyTileCoordinates(const std::vector<int> &state) const {
  int index = std::find(state.begin(), state.end(), 0) - state.begin();
  return std::array<int, 2>{index / _size, index % _size};
}
