from Pente import *
import importlib
import threading
import time

class Play:
  def __init__(self, timeLimit, player1, player2, graphics, visualize):
      self._timeLimit = timeLimit
      self._player1Name = player1
      self._player2Name = player2
      self.Player1 = importlib.import_module(player1)
      exec('self._player1 = self.Player1.'+player1+'(timeLimit)')
      self.Player2 = importlib.import_module(player2)
      exec('self._player2 = self.Player2.'+player2+'(timeLimit)')
    
      self._state = PenteState()

      if graphics:
        import Draw
        self._draw = Draw.PenteVisualizer(graphics,visualize)
      else:
        self._draw = None
        
      self._visualize = visualize
      
  def visualize(self, player):
    while True:
      self._draw.considering(self._state.getTurn() % 2, player.getConsideredMoves(), player.getSelectedMove())
      self._draw.horizon(self._state.getTurn() % 2, player.getHorizon(), player.getValue())
      self._draw._canvas.refresh()
      
      if player.turnOver():
        break
      time.sleep(.1)
    
    self._draw.considering(self._state.getTurn() % 2, player.getConsideredMoves(), player.getSelectedMove())
    self._draw.horizon(self._state.getTurn() % 2, player.getHorizon(), player.getValue())
    self._draw._canvas.refresh()
        
  def run(self):
    loser = None
    while not self._state.gameOver():
      if self._visualize:
        self._draw.clear(self._state.getTurn() % 2)
      print()
      print('Turn', self._state.getTurn())
      moves = self._state.actions()
      if self._state.getTurn() % 2 == 0:
        moveThread = threading.Thread(target=self._player1.maxPlayerMove, args=(self._state,))
        self._player1.startTimer()
        moveThread.start()
        if self._visualize:
          self.visualize(self._player1)
        else:
          time.sleep(self._timeLimit)
        move = self._player1.getSelectedMove()
        moveThread.join()
        if self._visualize:
          self.visualize(self._player1)
      else:
        moveThread = threading.Thread(target=self._player2.minPlayerMove, args=(self._state,))
        self._player2.startTimer()
        moveThread.start()
        if self._visualize:
          self.visualize(self._player2)
        else:
          time.sleep(self._timeLimit)
        move = self._player2.getSelectedMove()
        moveThread.join()
        if self._visualize:
          self.visualize(self._player1)
          
      if self._state.getTurn() % 2 == 0:
        print ('Black move', chr(ord('A')+move[1])+str(move[0]+1))
      else:
        print ('White move', chr(ord('A')+move[1])+str(move[0]+1))
        

      if self._state.getTurn() % 2 == 0:
        value = self._player1.getValue()
      else:
        value = self._player2.getValue()
      print ('\nHorizon value = ', value)
      print ('Horizon state:')
      if self._state.getTurn() % 2 == 0:
        self._player1.getHorizon().display()
      else:
        self._player2.getHorizon().display()
        
      if move in self._state.actions():
        self._state = self._state.result(move)
      else:
        print ('Illegal move!')
        loser = self._state.getTurn() % 2
        break
      
      if self._draw:
        self._draw.draw(self._state)
  
      print('\nAfter move:')
      self._state.display()
      print()
        
      
    winner = self._state.winner()
    if winner == 1 or loser == 0:
      print('Black wins!')
    elif winner == -1 or loser == 1:
      print('White wins!')
    else:
      print("It's a draw")
      
    return winner
        
      
if __name__ == '__main__':
  import sys
  if int(sys.argv[4]) > 0:
    p = Play(float(sys.argv[1]), sys.argv[2], sys.argv[3], int(sys.argv[4]), True)
  else:
    p = Play(float(sys.argv[1]), sys.argv[2], sys.argv[3], int(sys.argv[4]), False)
  p.run()
