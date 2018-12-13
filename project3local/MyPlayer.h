#pragma once

#include "Player.h"

class MyPlayer : public Player {
public:
  MyPlayer(const Maze &maze, int numGhosts);

  Direction getMove(Observations &observations);
};
