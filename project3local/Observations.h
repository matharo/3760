#pragma once

#include "Maze.h"

#include <string>
#include <tuple>
#include <vector>

// Possibilities are:
//   "Player move", x, y
//   "Ghost caught", ghost_id, IGNORE
//   "Ghost not caught", ghost_id, IGNORE
//   "Ghost move", ghost_id, IGNORE
//   "Noisy distance", ghost_id, noisyDistance
class Observation {
public:
  Observation(std::string description, int data1, int data2);

  std::string description() const;
  int data1() const;
  int data2() const;

private:
  const std::tuple<std::string, int, int> _observation;
};

class Observations {
public:
  Observations(int numGhosts);

  int numObservations() const;
  Observation getObservation(int i) const;
  void addObservation(Observation obs);

  int numAliveGhosts() const;
  bool ghostAlive(int g) const;
  void ghostDied(int g);

private:
  std::vector<Observation> _observations;
  int _numAliveGhosts;
  std::array<bool, 3> _ghostAlive;
};
