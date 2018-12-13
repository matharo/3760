#include "Player.h"

#include <iomanip>
#include <iostream>

Player::Player(const Maze &maze, int numGhosts)
    : _maze(maze), _numGhosts(numGhosts), _position(maze.getStart()) {
  for (int g = 0; g < _numGhosts; g++) {
    for (int i = 0; i < 5; i++)
      for (auto pos : maze.validPositions()) {
        _calculatedGhostProbability[g][std::make_tuple(pos, i)] =
            .2 / maze.validPositions().size();
    }
  }
}

void Player::clearCalculatedGhostProbabilities() {
  for (int g=0; g<3; g++)
    _calculatedGhostProbability[g].clear();
}

void Player::setCalculatedGhostProbability(int ghost, Position position, int behavior, double p) {
  _calculatedGhostProbability[ghost][std::make_tuple(position, behavior)] = p;
}

double Player::getCalculatedGhostProbability(int ghost, Position position, int behavior) const {
  return _calculatedGhostProbability[ghost][std::make_tuple(position, behavior)];
}

const std::map<std::tuple<Position, int>, double> Player::calculatedGhostProbabilities(int ghost) const {
  return _calculatedGhostProbability[ghost];
}

const std::map<std::tuple<std::tuple<int,int>, int>, double> Player::calculatedGhostProbabilities_python(int ghost) const {
  std::map<std::tuple<std::tuple<int,int>, int>, double> m;
  for (auto item : _calculatedGhostProbability[ghost]) 
    m[std::make_tuple(std::make_tuple(std::get<0>(item.first)[0], std::get<0>(item.first)[1]), std::get<1>(item.first))] = item.second;
    
  return m;
}

void Player::draw(const Observations &observations, Position playerPos,
                  std::vector<Position> ghostPositions,
                  std::vector<int> noisyDistances) const {
                    
  std::array<std::map<Position, double>, 3> distributions;
  std::array<std::array<double,5>, 3> behaviors;
  for (int g=0; g<3; g++)
    for (int i=0; i<5; i++)
      behaviors[g][i] = 0.;
  
  for (int i=0; i<_numGhosts; i++)
    for (auto item : _calculatedGhostProbability[i]) {
      Position pos = std::get<0>(item.first);
      int behavior = std::get<1>(item.first);
      double prob = item.second;
      
      distributions[i][pos] += prob;
      behaviors[i][behavior] += prob;
    }

  std::cout << std::endl;

  std::cout << "Ghost behaviors:" << std::endl;
  for (int g = 0; g < _numGhosts; g++) {
    for (int i = 0; i < 5; i++)
      std::cout << std::setprecision(3) << std::fixed << std::setw(6)
                << behaviors[g][i] << " ";
    std::cout << std::endl;
  }
  std::cout << std::endl;

  for (int i = 0; i < 19; i++) {
    std::cout << "+";
    for (int i = 0; i < 3; i++)
      std::cout << "-";
  }
  std::cout << "+" << std::endl;

  for (int r = 0; r < 19; r++) {
    for (int i = 0; i < 3; i++) {
      std::cout << "|";
      for (int c = 0; c < 19; c++) {
        // Set colors
        if (playerPos == Position{c, r})
          std::cout << "\u001b[43;1m";
        else if (ghostPositions[i] == Position{c, r}) {
          switch (i) {
          case 0:
            std::cout << "\u001b[31;1m";
            break;
          case 1:
            std::cout << "\u001b[32;1m";
            break;
          case 2:
            std::cout << "\u001b[34;1m";
            break;
          }
        } else if (abs(playerPos[0] - c) + abs(playerPos[1] - r) ==
                   noisyDistances[i]) {
          switch (i) {
          case 0:
            std::cout << "\u001b[41;1m";
            break;
          case 1:
            std::cout << "\u001b[42;1m";
            break;
          case 2:
            std::cout << "\u001b[44;1m";
            break;
          }
        }

        if (!_maze.inMaze({c, r}))
          std::cout << "WWW";
        else if (i < _numGhosts && distributions[i].size() > 0) {
          int p = static_cast<int>(1000 * distributions[i][{c, r}]);
          std::cout << (p / 100) << ((p % 100) / 10) << (p % 10);
        } else {
          std::cout << "   ";
        }

        // Reset colors
        std::cout << "\u001b[0m";

        std::cout << "|";
      }
      std::cout << std::endl;
    }
    for (int i = 0; i < 19; i++) {
      std::cout << "+";
      for (int i = 0; i < 3; i++)
        std::cout << "-";
    }
    std::cout << "+" << std::endl;
  }
}

int Player::numGhosts() const { return _numGhosts; }

Position Player::getPosition() const { return _position; }

void Player::movePlayer(Direction d) { _position = _maze.result(_position, d); }

void Player::startTimer() { _startTime = std::chrono::high_resolution_clock::now(); }

double Player::timeUsed() const {
  auto currentTime = std::chrono::high_resolution_clock::now();
  return std::chrono::duration_cast<std::chrono::duration<double>>(currentTime - _startTime).count();
}

