from Pente import *

import copy
import threading
import time

class Player:
  def __init__(self, timeLimit):
    self._timeLimit = timeLimit
    self._lock = threading.Lock()
    
    self._consideredMoves = []
    self._selectedMove = [-1,-1]
    self._horizon = PenteState()
    self._value = 0
    
  def startTimer(self):
    self._lock.acquire()
    self._startTime = time.time()
    self._lock.release()
    
  def turnOver(self):
    over = (time.time() - self._startTime > self._timeLimit)
    return over
    
  def selectMove(self, move):
    self._lock.acquire()
    self._selectedMove = move
    self._lock.release()
    
  def getSelectedMove(self):
    self._lock.acquire()
    move = copy.copy(self._selectedMove)
    self._lock.release()
    return move
    
  def clearConsideredMoves(self):
    self._lock.acquire()
    self._consideredMoves = []
    self._lock.release()
    
  def considerMove(self, move):
    self._lock.acquire()
    self._consideredMoves.append(move)
    self._lock.release()
    
  def getConsideredMoves(self):
    self._lock.acquire()
    considered = copy.copy(self._consideredMoves)
    self._lock.release()
    return considered
    
  def selectHorizon(self, h, value):
    self._lock.acquire()
    self._horizon = h
    self._value = value
    self._lock.release()
    
  def getHorizon(self):
    self._lock.acquire()
    h = self._horizon
    self._lock.release()
    return h
    
  def getValue(self):
    self._lock.acquire()
    v = copy.copy(self._value)
    self._lock.release()
    return v 

  def maxPlayerMove(self, state):
    pass

  def minPlayerMove(self, state):
    pass
    
