#include "Player.h"

Player::Player(double timeLimit) : _timeLimit(timeLimit) { srand(time(NULL)); }

void Player::startTimer() {
  std::lock_guard<std::mutex> lock(_accessLock);
  _startTime = std::chrono::high_resolution_clock::now();
}

bool Player::turnOver() const {
  //  std::lock_guard<std::mutex> lock(_accessLock);
  auto currentTime = std::chrono::high_resolution_clock::now();
  double duration = std::chrono::duration_cast<std::chrono::seconds>(
      currentTime - _startTime).count();
  return duration > _timeLimit;
}

void Player::selectMove(Position m) {
  std::cout << "(best now " << (char)(m[1] + 'A') << (int)(m[0] + 1) << ") ";
  std::lock_guard<std::mutex> lock(_accessLock);
  _selectedMove = m;
}

Position Player::getSelectedMove() const {
  std::lock_guard<std::mutex> lock(_accessLock);
  return _selectedMove;
}

void Player::clearConsideredMoves() {
  std::cout << "\nSearch new depth tree.  Nodes considered: ";
  std::lock_guard<std::mutex> lock(_accessLock);
  _consideredMoves.clear();
}

void Player::considerMove(Position m) {
  std::cout << (char)(m[1] + 'A') << (int)(m[0] + 1) << " ";
  std::lock_guard<std::mutex> lock(_accessLock);
  _consideredMoves.push_back(m);
}

std::vector<Position> Player::getConsideredMoves() const {
  std::lock_guard<std::mutex> lock(_accessLock);
  return _consideredMoves;
}

void Player::selectHorizon(PenteState h, int v) {
  std::lock_guard<std::mutex> lock(_accessLock);
  _moveValue = v;
  _horizon = h;
}

PenteState Player::getHorizon() const {
  std::lock_guard<std::mutex> lock(_accessLock);
  return _horizon;
}

int Player::getValue() const {
  std::lock_guard<std::mutex> lock(_accessLock);
  return _moveValue;
}

void Player::maxPlayerMove(PenteState s) {}

void Player::minPlayerMove(PenteState s) {}
