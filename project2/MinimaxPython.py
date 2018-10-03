from random import *
import threading
from copy import *

from Player import *

class MinimaxPython(Player):
  def __init__(self, startTime):
    Player.__init__(self, startTime)
    self._bestMove = {}
  
  def maxPlayerMove(self, s):
    for depth in range(2,10):
      possibleActions = self.candidateMoves(s)
      bestValue = -10000
      self.clearConsideredMoves()
      for move in possibleActions:
        if self.turnOver():
          return
        self.considerMove(move)
        valueAndHorizon = self.minPlayerValue(s.result(move), depth - 1)
        value = valueAndHorizon[0]
        if (value > bestValue):
          bestValue = value
          self.selectMove(move)
          self.selectHorizon(valueAndHorizon[1], value)
          self._bestMove[s] = move

  def minPlayerMove(self, s):
    for depth in range(2,10):
      possibleActions = self.candidateMoves(s)
      bestValue = 10000
      self.clearConsideredMoves()
      for move in possibleActions:
        if self.turnOver():
          return
        self.considerMove(move)
        valueAndHorizon = self.maxPlayerValue(s.result(move), depth - 1)
        value = valueAndHorizon[0]
        if (value < bestValue):
          bestValue = value
          self.selectMove(move)
          self.selectHorizon(valueAndHorizon[1], value)
          self._bestMove[s] = move           
          
  def maxPlayerValue(self, s, depth):
    if s.gameOver():
      return (1000*s.winner(), s)
      
    if depth == 0:
      return (self.heuristic(s), s)
      
    bestValue = -10000
    horizon = PenteState()
    for move in self.candidateMoves(s):
      valueAndHorizon = self.minPlayerValue(s.result(move), depth-1)
      value = valueAndHorizon[0]
      if (value > bestValue):
        bestValue = value
        self._bestMove[s] = move
        horizon = valueAndHorizon[1]        
        
    return (bestValue, horizon)        
          
  def minPlayerValue(self, s, depth):
    if s.gameOver():
      return (1000*s.winner(), s)
      
    if depth == 0:
      return (self.heuristic(s), s)
      
    bestValue = 10000
    horizon = PenteState()
    for move in self.candidateMoves(s):
      valueAndHorizon = self.maxPlayerValue(s.result(move), depth-1)
      value = valueAndHorizon[0]
      if (value < bestValue):
        bestValue = value
        self._bestMove[s] = move
        horizon = valueAndHorizon[1]        
        
    return (bestValue, horizon)
    
  def candidateMoves(self, s):
    moveValue = {}

    # Previous move is the best guess
    if s in self._bestMove:
      moveValue[tuple(self._bestMove[s])] = moveValue.get(tuple(self._bestMove[s]),0) + 1000000
      
    # Captures and blocking captures
    for m in s.patternLocation("BWW*"):
      moveValue[tuple(m)] = moveValue.get(tuple(m),0) + 50
    for m in s.patternLocation("WBB*"):
      moveValue[tuple(m)] = moveValue.get(tuple(m),0) + 50

    # Open runs of 4 and blocking them
    for m in s.patternLocation("BBBB*"):
      moveValue[tuple(m)] = moveValue.get(tuple(m),0) + 1000
    for m in s.patternLocation("WWWW*"):
      moveValue[tuple(m)] = moveValue.get(tuple(m),0) + 1000

    # Open runs of 2 and blocking them
    for m in s.patternLocation("BB*"):
      moveValue[tuple(m)] = moveValue.get(tuple(m),0) + 5
    for m in s.patternLocation("WW*"):
      moveValue[tuple(m)] = moveValue.get(tuple(m),0) + 5

    # Adjacent
    for m in s.patternLocation("B*"):
      moveValue[tuple(m)] = moveValue.get(tuple(m),0) + 1
    for m in s.patternLocation("W*"):
      moveValue[tuple(m)] = moveValue.get(tuple(m),0) + 1

    candidates = s.pieceNeighborhood(2)
    
    options = [ (moveValue.get(tuple(m),0), m) for m in candidates ]
    options.sort()
    options.reverse()
    
    candidates = [ b for (a,b) in options ]
    
    # Keep the 10 best move
    candidates = candidates[:10]

    return candidates

  def heuristic(self, s):
    score = 100 * s.numCaptures()[0] - 100 * s.numCaptures()[1]
    score += 10 * s.patternCount("BB ") - 10 * s.patternCount("WW ")
    score += s.patternCount("BB") - s.patternCount("WW")

    return score
