#include "Observations.h"

Observation::Observation(std::string description, int data1, int data2)
    : _observation(std::make_tuple(description, data1, data2)) {}

std::string Observation::description() const {
  return std::get<0>(_observation);
}

int Observation::data1() const { return std::get<1>(_observation); }

int Observation::data2() const { return std::get<2>(_observation); }

Observations::Observations(int numGhosts) : _numAliveGhosts(numGhosts) {
  for (int i = 0; i < numGhosts; i++)
    _ghostAlive[i] = true;
  for (int i = numGhosts; i < 3; i++)
    _ghostAlive[i] = false;
}

int Observations::numObservations() const { return _observations.size(); }

Observation Observations::getObservation(int i) const {
  return _observations[i];
}

void Observations::addObservation(Observation obs) {
  _observations.push_back(obs);
}

int Observations::numAliveGhosts() const { return _numAliveGhosts; }

bool Observations::ghostAlive(int g) const { return _ghostAlive[g]; }

void Observations::ghostDied(int g) {
  _numAliveGhosts--;
  _ghostAlive[g] = false;
  _observations.push_back(Observation("Ghost caught", g, -1));
}
