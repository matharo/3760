#include "Maze.h"

#include <cmath>
#include <cstdlib>
#include <set>


#include <iostream>

Maze::Maze()
    : _map({"WWWWWWWWWWWWWWWWWWW", "W        W        W", "W WW WWW W WWW WW W",
            "W                 W", "W WW W WWWWW W WW W", "W    W   W   W    W",
            "WWWW WWW W WWW WWWW", "WWWW W   S   W WWWW", "WWWW W WWWWW W WWWW",
            "A      WJJJW      B", "WWWW W WWWWW W WWWW", "WWWW W       W WWWW",
            "WWWW WWW W WWW WWWW", "W    W   W   W    W", "W WW W WWWWW W WW W",
            "W                 W", "W WW WWW W WWW WW W", "W        W        W",
            "WWWWWWWWWWWWWWWWWWW"}) {
  std::srand(time(NULL));
              
  // Find the starting and other special locations
  for (int x = 0; x < _map[0].size(); x++)
    for (int y = 0; y < _map.size(); y++)
      if (_map[y][x] == 'S')
        _start = {x, y};
      else if (_map[y][x] == 'A')
        _positionA = {x, y};
      else if (_map[y][x] == 'B')
        _positionB = {x, y};

  // Find the valid positions
  for (int x = 0; x < _map[0].size(); x++)
    for (int y = 0; y < _map.size(); y++)
      if (_map[y][x] == ' ' || _map[y][x] == 'A' || _map[y][x] == 'B' || _map[y][x] == 'S')
        _validPositions.push_back({x, y});

  // Find the error distribution
  for (int i = 0; i < 5; i++)
    _errorDistribution.push_back(std::pow(1.2, i));
  for (int i = 3; i >= 0; i--)
    _errorDistribution.push_back(std::pow(1.2, i));
  double s = 0;
  for (auto p : _errorDistribution)
    s += p;
  for (int i = 0; i < _errorDistribution.size(); i++)
    _errorDistribution[i] /= s;

  // Calculate the noisy distance probability
  int maxN{0};
  for (int d = 1; d < 2 * _size; d++)
    for (int i = 0; i < _errorDistribution.size(); i++) {
      int n = std::min(
          std::max(1, static_cast<int>(d + i - _errorDistribution.size() / 2)),
          2 * _size + 2);
      maxN = std::max(n, maxN);
      _noisyDistanceProb[std::make_tuple(n, d)] += _errorDistribution[i];
    }

  for (int n = 0; n <= maxN; n++) {
    double s = 0.;
    for (auto& item : _noisyDistanceProb)
      if (std::get<0>(item.first) == n)
        s += item.second;
    for (auto& item : _noisyDistanceProb)
      if (std::get<0>(item.first) == n)
        item.second /= s;
  }
  _noisyDistanceProb[std::make_tuple(1, 1)] = 1.;

  // Calculate the maze distances, directions, etc.
  for (auto& p1 : validPositions())
    _distance[std::make_tuple(p1, p1)] = 0;

  bool done = false;
  int currentDistance = 0;
  while (!done) {
    done = true;
    for (auto& iter : _distance)
      if (iter.second == currentDistance) {
        Position p1 = std::get<0>(iter.first);
        Position p2 = std::get<1>(iter.first);
        for (auto direction : validMoves(p2)) {
          Position newP = result(p2, direction);
          if (_distance.find(std::make_tuple(p1, newP)) == _distance.end())
            _distance[std::make_tuple(p1, newP)] = currentDistance + 1;
          done = false;
        }
      }
    currentDistance++;
  }

  for (Position p1 : validPositions()) {
    for (Position p2 : validPositions())
      if (p1 != p2) {
        for (auto direction : validMoves(p1))
          if (_distance[std::make_tuple(result(p1, direction), p2)] <
              _distance[std::make_tuple(p1, p2)])
            _directionToward[std::make_tuple(p1, p2)] = direction;
          else if (_distance[std::make_tuple(result(p1, direction), p2)] >
                   _distance[std::make_tuple(p1, p2)])
            _directionAway[std::make_tuple(p1, p2)] = direction;
      }
  }
}

bool Maze::inMaze(const Position &pos) const {
  if (pos[0] < 0)
    return false;
  if (pos[0] >= _map[0].size())
    return false;
  if (pos[1] < 0)
    return false;
  if (pos[1] >= _map.size())
    return false;

  switch (_map[pos[1]][pos[0]]) {
  case 'W':
  case 'J':
    return false;
    break;
  default:
    return true;
  }
}

const std::vector<Position> Maze::validPositions() const {
  return _validPositions;
}

Position Maze::getStart() const { return _start; }

std::vector<char> Maze::validMoves(const Position &pos) const {
  switch (_map[pos[1]][pos[0]]) {
  case 'A':
  case 'B':
    return {'E', 'W'};
  default:
    std::vector<char> directions;
    if (inMaze({pos[0], pos[1] - 1}))
      directions.push_back('N');
    if (inMaze({pos[0] + 1, pos[1]}))
      directions.push_back('E');
    if (inMaze({pos[0], pos[1] + 1}))
      directions.push_back('S');
    if (inMaze({pos[0] - 1, pos[1]}))
      directions.push_back('W');
    return directions;
  }
}

Position Maze::result(const Position &pos, char direction) const {
  if (pos == _positionA && direction == 'W')
    return _positionB;
  if (pos == _positionB && direction == 'E')
    return _positionA;

  switch (direction) {
  case 'N':
    return {pos[0], pos[1] - 1};
  case 'E':
    return {pos[0] + 1, pos[1]};
  case 'S':
    return {pos[0], pos[1] + 1};
  case 'W':
    return {pos[0] - 1, pos[1]};
  }
  return pos; // Don't move
}

int Maze::manhattanDistance(const Position &location1,
                            const Position &location2) const {
  return std::abs(location1[0] - location2[0]) +
         std::abs(location1[1] - location2[1]);
}

int Maze::mazeDistance(const Position &location1,
                       const Position &location2) const {
  return _distance[std::make_tuple(location1, location2)];
}

Direction Maze::directionToward(const Position &current,
                                const Position &goal) const {
  return _directionToward[std::make_tuple(current, goal)];
}

Direction Maze::directionAway(const Position &current,
                              const Position &goal) const {
  return _directionAway[std::make_tuple(current, goal)];
}

int Maze::noisyDistance(const Position &location1,
                        const Position &location2) const {
  int m = manhattanDistance(location1, location2);

  double v = static_cast<double>(random()) / RAND_MAX;
  int e = 0;
  for (int i = 0; i < _errorDistribution.size(); i++) {
    if (v < _errorDistribution[i]) {
      e = i - _errorDistribution.size() / 2;
      break;
    } else
      v -= _errorDistribution[i];
  }
  return std::max(1, m + e);
}

double Maze::noisyDistanceProb(int noisyDistance, int distance) const {
  return _noisyDistanceProb[std::make_tuple(noisyDistance, distance)];
}
