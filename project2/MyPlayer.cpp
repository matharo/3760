#include <map>

#include "MyPlayer.h"

MyPlayer::MyPlayer(double timeLimit) : Player(timeLimit) {}

void MyPlayer::maxPlayerMove(PenteState s) {
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

void MyPlayer::minPlayerMove(PenteState s) {
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

std::tuple<int, PenteState> MyPlayer::maxPlayerValue(PenteState s,
                                                               int depth) {
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
// give possible value of a move
std::tuple<int, PenteState> MyPlayer::minPlayerValue(PenteState s,
                                                               int depth) {
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

// Decides which move to make next
std::vector<Position> MyPlayer::candidateMoves(PenteState s) {
  std::map<Position, int> moveValue;

  // Checks for a winning move
  if (!(s.winningMoves()).empty()) {
    // Any winning move is equal value
    for (Position m : s.winningMoves())
      moveValue[m] += 1000;
  } else {
    // Checks for a blocking move
    if (!(s.blockingMoves()).empty()) {
      for (Position m : s.blockingMoves())
        moveValue[m] += 50;
    }

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

    // Open runs of 3 and blocking
    for (Position m : s.patternLocation("BBB*"))
      moveValue[m] += 10;
    for (Position m : s.patternLocation("WWW*"))
      moveValue[m] += 10;

    // Empty space between 2 and 1 and blocking, 4 in a row
    for (Position m : s.patternLocation("BB*B"))
      moveValue[m] += 50;
    for (Position m : s.patternLocation("WW*W"))
      moveValue[m] += 50;

    // Empty space in between 3 and 1 and blocking, 5 in a row
    for (Position m : s.patternLocation("BBB*B"))
      moveValue[m] += 1000;
    for (Position m : s.patternLocation("WWW*W"))
      moveValue[m] += 1000;

    for (Position m : s.patternLocation("B*BBB"))
      moveValue[m] += 1000;
    for (Position m : s.patternLocation("W*WWW"))
      moveValue[m] += 1000;

    // Empty space between 2 and 2 and blocking, 5 in a row
    for (Position m : s.patternLocation("BB*BB"))
      moveValue[m] += 1000;
    for (Position m : s.patternLocation("WW*WW"))
      moveValue[m] += 1000;

  } // end of else statement for s.winningMoves==empty

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

// Looks at placement of pieces on the board and decides score
// Do not look at too many pieces
// chapter 5.2 or 5.3 pseudocode, minmax
int MyPlayer::heuristic(PenteState s) {
  int score = 0;
  score += 100 * s.numCaptures()[0] - 100 * s.numCaptures()[1];
  score += 10 * s.patternCount("BB ") - 10 * s.patternCount("WW ");
  score += 25 * s.patternCount("BBB ") - 25 * s.patternCount("WWW ");
  score += 150 * s.patternCount("BBBB ") - 150 * s.patternCount("WWWW ");
  score += 150 * s.patternCount("BBB B") - 150 * s.patternCount("WWW W");
  score += 150 * s.patternCount("B BBB") - 150 * s.patternCount("W WWW");
  score += 150 * s.patternCount("BB BB") - 150 * s.patternCount("WW WW");
  return score;
}
