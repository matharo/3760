#pragma once

#include <map>
#include <vector>

#include "Player.h"

class MyPlayer : public Player {
public:
  MyPlayer(double timeLimit);
  void maxPlayerMove(PenteState s);
  void minPlayerMove(PenteState s);

  std::tuple<int, PenteState> maxPlayerValue(PenteState s, int depth);
  std::tuple<int, PenteState> minPlayerValue(PenteState s, int depth);
  std::vector<Position> candidateMoves(PenteState s);
  int heuristic(PenteState s);
  int moveValue(PenteState s, Position m);

private:
  std::map<PenteState, Position> _bestMove;
};
