#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <vector>

#include "MySolver.h"
#include "SlidingTilePuzzle.h"

PYBIND11_MODULE(SlidingTilePuzzleSolver, m) {
  pybind11::class_<SlidingTilePuzzle>(m, "SlidingTilePuzzle")
      .def(pybind11::init<int, std::vector<int>>())
      .def("getInitial", &SlidingTilePuzzle::getInitial)
      .def("isGoal", &SlidingTilePuzzle::isGoal)
      .def("actions", &SlidingTilePuzzle::actions)
      .def("result", &SlidingTilePuzzle::result)
      .def("randomState", &SlidingTilePuzzle::randomState);

  pybind11::class_<MySolver>(m, "MySolver")
      .def(pybind11::init<int, std::vector<int>>())
      .def("solution", &MySolver::solution);
}
