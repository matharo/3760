#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "MyPlayer.h"

PYBIND11_MODULE(MyPlayer, m) {
  pybind11::class_<MyPlayer>(m, "MyPlayer")
      .def(pybind11::init<double>())
      .def("startTimer", &MyPlayer::startTimer)
      .def("turnOver", &MyPlayer::turnOver)
      .def("selectMove", &MyPlayer::selectMove)
      .def("getSelectedMove", &MyPlayer::getSelectedMove)
      .def("getConsideredMoves", &MyPlayer::getConsideredMoves)
      .def("getHorizon", &MyPlayer::getHorizon)
      .def("getValue", &MyPlayer::getValue)
      .def("maxPlayerMove", &MyPlayer::maxPlayerMove,
           pybind11::call_guard<pybind11::gil_scoped_release>())
      .def("minPlayerMove", &MyPlayer::minPlayerMove,
           pybind11::call_guard<pybind11::gil_scoped_release>());
}
