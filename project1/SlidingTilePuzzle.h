#pragma once

#include <array>
#include <vector>

class SlidingTilePuzzle {
public:
public:
  explicit SlidingTilePuzzle(int size, std::vector<int> initial);

  std::vector<int> getInitial() const;
  bool isGoal(const std::vector<int> &state) const;
  std::vector<char> actions(const std::vector<int> &state) const;
  std::vector<int> result(const std::vector<int> &state, char action) const;
  std::vector<int> randomState() const;

private:
  std::array<int, 2> _emptyTileCoordinates(const std::vector<int> &state) const;

private:
  int _size;
  std::vector<int> _initial, _goal;
};

namespace std {
template <> struct hash<std::vector<int>> {
  size_t operator()(const vector<int> &v) const {
    std::hash<int> hasher;
    size_t seed = 0;
    for (int i : v) {
      seed ^= hasher(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
    return seed;
  }
};
} // namespace std
