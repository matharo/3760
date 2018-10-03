from random import *
import threading
from copy import *
from time import *

from Player import *

class RandomLegal(Player):
  def __init__(self, timeLimit):
    Player.__init__(self, timeLimit)
    
  def maxPlayerMove(self, state):
    actions = state.actions()    
    for m in actions:
      self.considerMove(m)
    move = actions[randint(0,len(actions)-1)]
    self.selectMove(move)
    self.selectHorizon(state.result(move), 0)
    
  def minPlayerMove(self, state):
    self.maxPlayerMove(state)
