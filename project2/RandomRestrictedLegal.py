from random import *
import threading
from copy import *

from Player import *

class RandomRestrictedLegal(Player):
  def __init__(self, startTime):
    Player.__init__(self, startTime)
  
  def maxPlayerMove(self, state):
    actions = state.winningMoves()
    if len(actions) == 0:
      actions = state.blockingMoves()
    if len(actions) == 0:
      actions = state.pieceNeighborhood(2)
  
    for m in actions:
      self.considerMove(m)
    move = actions[randint(0,len(actions)-1)]
    self.selectMove(move)
    self.selectHorizon(state.result(move), 0)
    

  def minPlayerMove(self, state):
    self.maxPlayerMove(state)

