#pragma once

#include "Maze.h"

#include <vector>

class Ghost {
public:
  Ghost(const Maze &maze, int behavior, Position pos);
  void move(const Position &pos);
  Position getPosition() const;
  Position getInitial() const;

private:
  const Maze &_maze;
  int _behavior;
  Position _initialPosition, _position;
  
  // For clockwise ghost
  int _goal;
};
