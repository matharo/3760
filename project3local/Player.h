#pragma once

#include "Ghost.h"
#include "Maze.h"
#include "Observations.h"

#include <chrono>

class Player {
public:
  Player(const Maze &maze, int numGhosts);

  void clearCalculatedGhostProbabilities();
  void setCalculatedGhostProbability(int ghost, Position position, int behavior, double p);
  double getCalculatedGhostProbability(int ghost, Position position, int behavior) const;
  const std::map<std::tuple<Position, int>, double> calculatedGhostProbabilities(int ghost) const;
  const std::map<std::tuple<std::tuple<int,int>, int>, double> calculatedGhostProbabilities_python(int ghost) const;

  int numGhosts() const;
  Position getPosition() const;
  void movePlayer(Direction d);

  void startTimer();
  double timeUsed() const;

  void draw(const Observations &observations, Position playerPos,
            std::vector<Position> ghostPositions,
            std::vector<int> noisyDistances) const;

protected:
  const Maze &_maze;

private:
  Position _position;
  int _numGhosts;
  
  mutable std::map<std::tuple<Position, int>, double> _calculatedGhostProbability[3];
  
  std::chrono::time_point<std::chrono::high_resolution_clock> _startTime;
};
