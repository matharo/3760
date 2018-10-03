#pragma once

#include <mutex>

#include "PenteState.h"

class Player {
public:
  Player(double timeLimit);

  void startTimer();
  bool turnOver() const;
  void selectMove(Position m);
  Position getSelectedMove() const;

  void clearConsideredMoves();
  void considerMove(Position m);
  std::vector<Position> getConsideredMoves() const;
  void selectHorizon(PenteState h, int value);
  PenteState getHorizon() const;
  int getValue() const;

  void maxPlayerMove(PenteState s);
  void minPlayerMove(PenteState s);

private:
  mutable std::mutex _accessLock;
  double _timeLimit;
  std::chrono::time_point<std::chrono::high_resolution_clock> _startTime;
  Position _selectedMove{ { -1, -1 } };
  int _moveValue;
  std::vector<Position> _consideredMoves;
  PenteState _horizon;
};
