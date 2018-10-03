#include <map>

#include "Minimax.h"

Minimax::Minimax(double timeLimit) : Player(timeLimit) {}

void Minimax::maxPlayerMove(PenteState s) {
  for (int depth = 2; depth < 10; depth++) {
    auto possibleActions = candidateMoves(s);
    int bestValue = -10000;
    clearConsideredMoves(); // For visualizing move considerations
    for (Position move : possibleActions) {
      if (turnOver()) {
        return;
      }
      considerMove(move); // For visualizing move considerations
      auto valueAndHorizon = minPlayerValue(s.result(move), depth - 1);
      int value = std::get<0>(valueAndHorizon);
      if (value > bestValue) {
        bestValue = value;
        selectMove(move);
        selectHorizon(std::get<1>(valueAndHorizon), value);
        _bestMove[s] = move;
      }
    }
  }
}

void Minimax::minPlayerMove(PenteState s) {
  for (int depth = 2; depth < 10; depth++) {
    auto possibleActions = candidateMoves(s);
    int bestValue = 10000;
    clearConsideredMoves(); // For visualizing move considerations
    for (Position move : possibleActions) {
      if (turnOver()) {
        return;
      }
      considerMove(move); // For visualizing move considerations
      auto valueAndHorizon = maxPlayerValue(s.result(move), depth - 1);
      int value = std::get<0>(valueAndHorizon);
      if (value < bestValue) {
        bestValue = value;
        selectMove(move);
        selectHorizon(std::get<1>(valueAndHorizon), value);
        _bestMove[s] = move;
      }
    }
  }
}

std::tuple<int, PenteState> Minimax::maxPlayerValue(PenteState s, int depth) {
  if (s.gameOver())
    return std::make_tuple(1000 * s.winner(), s);

  if (depth == 0)
    return std::make_tuple(heuristic(s), s);

  int bestValue = -10000;
  PenteState horizon;
  for (Position move : candidateMoves(s)) {
    auto valueAndHorizon = minPlayerValue(s.result(move), depth - 1);
    int value = std::get<0>(valueAndHorizon);
    if (value > bestValue) {
      bestValue = value;
      _bestMove[s] = move;
      horizon = std::get<1>(valueAndHorizon);
    }
  }

  return std::make_tuple(bestValue, horizon);
}

std::tuple<int, PenteState> Minimax::minPlayerValue(PenteState s, int depth) {
  if (s.gameOver())
    return std::make_tuple(1000 * s.winner(), s);

  if (depth == 0)
    return std::make_tuple(heuristic(s), s);

  int bestValue = 10000;
  PenteState horizon;
  for (Position move : candidateMoves(s)) {
    auto valueAndHorizon = maxPlayerValue(s.result(move), depth - 1);
    int value = std::get<0>(valueAndHorizon);
    if (value < bestValue) {
      bestValue = value;
      _bestMove[s] = move;
      horizon = std::get<1>(valueAndHorizon);
    }
  }

  return std::make_tuple(bestValue, horizon);
}

std::vector<Position> Minimax::candidateMoves(PenteState s) {
  std::map<Position, int> moveValue;

  // Previous move is the best guess
  if (_bestMove.find(s) != _bestMove.end()) {
    moveValue[_bestMove[s]] += 1000000;
  }

  // Captures and blocking captures
  for (Position m : s.patternLocation("BWW*"))
    moveValue[m] += 50;
  for (Position m : s.patternLocation("WBB*"))
    moveValue[m] += 50;

  // Open runs of 4 and blocking them
  for (Position m : s.patternLocation("BBBB*"))
    moveValue[m] += 1000;
  for (Position m : s.patternLocation("WWWW*"))
    moveValue[m] += 1000;

  // Open runs of 2 and blocking them
  for (Position m : s.patternLocation("BB*"))
    moveValue[m] += 5;
  for (Position m : s.patternLocation("WW*"))
    moveValue[m] += 5;

  // Adjacent
  for (Position m : s.patternLocation("B*"))
    moveValue[m] += 1;
  for (Position m : s.patternLocation("W*"))
    moveValue[m] += 1;

  // Use the move values to choose the 10 best options
  class Comparator {
  private:
    std::map<Position, int> &_value;

  public:
    Comparator(std::map<Position, int> &value) : _value(value) {}

    bool operator()(const Position &move1, const Position &move2) {
      return _value[move1] > _value[move2];
    }
  };

  std::vector<Position> candidates = s.pieceNeighborhood(2);
  std::sort(candidates.begin(), candidates.end(), Comparator(moveValue));

  // Keep the 10 best move
  while (candidates.size() > 10)
    candidates.pop_back();

  return candidates;
}

int Minimax::heuristic(PenteState s) {
  int score = 100 * s.numCaptures()[0] - 100 * s.numCaptures()[1];
  score += 10 * s.patternCount("BB ") - 10 * s.patternCount("WW ");
  score += s.patternCount("BB") - s.patternCount("WW");

  return score;
}
