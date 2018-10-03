#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "PenteState.h"

PYBIND11_MODULE(Pente, m) {
  pybind11::class_<PenteState>(m, "PenteState")
      .def(pybind11::init<>())
      .def("getState", &PenteState::getState)
      .def("getTurn", &PenteState::getTurn)
      .def("numCaptures", &PenteState::numCaptures)
      .def("winner", &PenteState::winner)
      .def("gameOver", &PenteState::gameOver)
      .def("actions", &PenteState::actions)
      .def("winningMoves", &PenteState::winningMoves)
      .def("blockingMoves", &PenteState::blockingMoves)
      .def("pieceNeighborhood", &PenteState::pieceNeighborhood)
      .def("captureCount", &PenteState::captureCount)
      .def("result", &PenteState::result)
      .def("patternCount", &PenteState::patternCount)
      .def("patternLocation", &PenteState::patternLocation)
      .def("display", &PenteState::display);
}
