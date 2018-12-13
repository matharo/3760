#pragma once

#include <array>
#include <map>
#include <string>
#include <tuple>
#include <vector>

using Position = std::array<int, 2>;
using Direction = char;

class Maze {
public:
  Maze();
  Position getStart() const;
  bool inMaze(const Position &pos) const;
  const std::vector<Position> validPositions() const;
  std::vector<char> validMoves(const Position &pos) const;
  Position result(const Position &pos, char direction) const;
  int manhattanDistance(const Position &location1,
                        const Position &location2) const;
  int mazeDistance(const Position &location1, const Position &location2) const;
  int noisyDistance(const Position &location1, const Position &location2) const;
  Direction directionToward(const Position &current,
                            const Position &goal) const;
  Direction directionAway(const Position &current, const Position &goal) const;
  double noisyDistanceProb(int noisyDistance, int distance) const;

private:
  int _size{19};
  std::vector<std::string> _map;
  std::vector<Position> _validPositions;

  Position _start, _positionA, _positionB;

  mutable std::map<std::tuple<Position, Position>, int> _distance;
  mutable std::map<std::tuple<Position, Position>, char> _directionToward,
      _directionAway;

  std::vector<double> _errorDistribution;
  mutable std::map<std::tuple<int, int>, double>
      _noisyDistanceProb; // Index = noisy dist, actual dist
};
