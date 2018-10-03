#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "Minimax.h"

PYBIND11_MODULE(Minimax, m) {
  pybind11::class_<Minimax>(m, "Minimax")
      .def(pybind11::init<double>())
      .def("startTimer", &Minimax::startTimer)
      .def("turnOver", &Minimax::turnOver)
      .def("selectMove", &Minimax::selectMove)
      .def("getSelectedMove", &Minimax::getSelectedMove)
      .def("getConsideredMoves", &Minimax::getConsideredMoves)
      .def("getHorizon", &Minimax::getHorizon)
      .def("getValue", &Minimax::getValue)
      .def("maxPlayerMove", &Minimax::maxPlayerMove,
           pybind11::call_guard<pybind11::gil_scoped_release>())
      .def("minPlayerMove", &Minimax::minPlayerMove,
           pybind11::call_guard<pybind11::gil_scoped_release>());
}
