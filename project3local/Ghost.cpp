#include "Ghost.h"


Ghost::Ghost(const Maze &maze, int behavior, Position pos)
    : _maze(maze), _behavior(behavior), _position(pos), _initialPosition(pos), _goal(0) {
}

void Ghost::move(const Position &playerPosition) {
  switch (_behavior) {
  case 0: // Stationairy
    break;
  case 1: // Scared (move away)
    _position =
        _maze.result(_position, _maze.directionAway(_position, playerPosition));
    break;
  case 2: // Wants to be caught (move towards)
    _position = _maze.result(_position,
                             _maze.directionToward(_position, playerPosition));
    break;
  case 3: // Left to right
    if (_position[0] < 8)
      _position = _maze.result(
          _position, _maze.directionToward(_position, _maze.getStart()));
    else if (_position[0] < 12)
      _position =
          _maze.result(_position, _maze.directionToward(_position, {17, 9}));
    else
      _position =
          _maze.result(_position, _maze.directionToward(_position, {1, 9}));
    break;
  case 4: // Clockwise
    if (_position[1] < 6) {
      if (_position[0] < 6) {
        _position =
            _maze.result(_position, _maze.directionToward(_position, {9, 3}));
      } else if (_position[0] < 13) {
        _position =
            _maze.result(_position, _maze.directionToward(_position, {17, 1}));
      } else {
        _position =
            _maze.result(_position, _maze.directionToward(_position, {14, 9}));
      }
    } else if (_position[1] < 13) {
      if (_position[0] < 6) {
        _position =
            _maze.result(_position, _maze.directionToward(_position, {1, 1}));
      } else if (_position[0] < 13) {
        _position =
            _maze.result(_position, _maze.directionToward(_position, {14, 9}));
      } else {
        _position =
            _maze.result(_position, _maze.directionToward(_position, {17, 17}));
      }
    } else {
      if (_position[0] < 6) {
        _position =
            _maze.result(_position, _maze.directionToward(_position, {4, 9}));
      } else if (_position[0] < 13) {
        _position =
            _maze.result(_position, _maze.directionToward(_position, {1, 17}));
      } else {
        _position =
            _maze.result(_position, _maze.directionToward(_position, {9, 15}));
      }
    }
    break;
  }
}

Position Ghost::getInitial() const { return _initialPosition; }

Position Ghost::getPosition() const { return _position; }
