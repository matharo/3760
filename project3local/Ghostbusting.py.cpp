#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "Ghost.h"
#include "Maze.h"
#include "MyPlayer.h"
#include "Observations.h"
#include "Player.h"

PYBIND11_MODULE(Ghostbusting, m) {
  pybind11::class_<Ghost>(m, "Ghost")
      .def(pybind11::init<const Maze &, int, Position>())
      .def("move", &Ghost::move)
      .def("getInitial", &Ghost::getInitial)
      .def("getPosition", &Ghost::getPosition);

  pybind11::class_<Maze>(m, "Maze")
      .def(pybind11::init<>())
      .def("getStart", &Maze::getStart)
      .def("inMaze", &Maze::inMaze)
      .def("validMoves", &Maze::validMoves)
      .def("result", &Maze::result)
      .def("manhattanDistance", &Maze::manhattanDistance)
      .def("mazeDistance", &Maze::mazeDistance)
      .def("noisyDistance", &Maze::noisyDistance)
      .def("directionToward", &Maze::directionToward)
      .def("directionAway", &Maze::directionAway)
      .def("noisyDistanceProb", &Maze::noisyDistanceProb);

  pybind11::class_<Observation>(m, "Observation")
      .def(pybind11::init<std::string, int, int>())
      .def("description", &Observation::description)
      .def("data1", &Observation::data1)
      .def("data2", &Observation::data2);

  pybind11::class_<Observations>(m, "Observations")
      .def(pybind11::init<int>())
      .def("numObservations", &Observations::numObservations)
      .def("getObservation", &Observations::getObservation)
      .def("addObservation", &Observations::addObservation)
      .def("numAliveGhosts", &Observations::numAliveGhosts)
      .def("ghostAlive", &Observations::ghostAlive)
      .def("ghostDied", &Observations::ghostDied);

  pybind11::class_<Player>(m, "Player")
      .def(pybind11::init<const Maze &, int>())
      .def("clearCalculatedGhostProbabilities", &Player::clearCalculatedGhostProbabilities)
      .def("setCalculatedGhostProbability", &Player::setCalculatedGhostProbability)
      .def("getCalculatedGhostProbability", &Player::getCalculatedGhostProbability)
      .def("calculatedGhostProbabilities", &Player::calculatedGhostProbabilities_python)
           
      .def("numGhosts", &Player::numGhosts)
      .def("getPosition", &Player::getPosition)
      .def("movePlayer", &Player::movePlayer)
      .def("startTimer", &Player::startTimer)
      .def("timeUsed", &Player::timeUsed)
      .def("draw", &Player::draw);

  pybind11::class_<MyPlayer, Player>(m, "MyPlayer")
      .def(pybind11::init<const Maze &, int>())
      .def("getMove", &MyPlayer::getMove);
}
