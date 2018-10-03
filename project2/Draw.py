from cs1graphics import *

from Pente import *

class PenteVisualizer:
  def __init__(self, size, visualize):
    self._size = size
    self._visualize = visualize
    self._diagnosticBoards = []
    self._currentLevel = 0
    
    if visualize:
      self._canvas = Canvas(size, size*.55)
      self._canvas.setAutoRefresh(False)
      self._board = self.Board(size/2.)
      for i in range(4):
        self._diagnosticBoards.append(self.Board(size/4.))
        self._canvas.add(self._diagnosticBoards[i])
      self._diagnosticBoards[0].moveTo(.5*size,0)
      self._diagnosticBoards[1].moveTo(.75*size,0)
      self._diagnosticBoards[2].moveTo(.5*size,.275*size)
      self._diagnosticBoards[3].moveTo(.75*size,.275*size)
      
    else:
      self._canvas = Canvas(size, size*1.1)
      self._canvas.setAutoRefresh(False)
      self._board = self.Board(size)
      
    self._board.setLabel('Turn 1: Black to move')
    
    self._canvas.setBackgroundColor('tan')
    self._canvas.setTitle('Pente')
    self._canvas.add(self._board)
      
    self._canvas.refresh()
    
  def draw(self, state):
    self._currentLevel = state.getTurn()
    self._board.draw(state)
    if self._visualize:
      self._diagnosticBoards[2*(state.getTurn()%2)].clear()
      self._diagnosticBoards[2*(state.getTurn()%2)].draw(state)
      
    if state.getTurn() % 2 == 0:
      self._board.setLabel('Turn %d: Black to move' % (1+state.getTurn()))
    else:
      self._board.setLabel('Turn %d: White to move' % (1+state.getTurn()))
      
    if state.gameOver():
      if state.winner() == 1:
        self._board.setLabel('Black won!')
      elif state.winner() == -1:
        self._board.setLabel('White won!')
      else:
        self._board.setLabel('It\'s a draw!')
    self._canvas.refresh()

  def clear(self, player):
    self._diagnosticBoards[2*player].clear()

  def considering(self, player, moves, selected):
    self._diagnosticBoards[2*player].considering(moves, selected)
   
  def horizon(self, player, state, value):
    self._diagnosticBoards[2*player+1].draw(state)
    self._diagnosticBoards[2*player+1].setLabel('Black horizon position, value = %d' % value)
    self._diagnosticBoards[2*player].setLabel('Horizon depth = %d' % (state.getTurn()-self._currentLevel))

  class Board(Layer):
    def __init__(self, size):
      Layer.__init__(self)
      self._stoneRadius = size/40.
      
      # Draw labels
      for i in range(19):
        t = Text(chr(i+ord('A')), self._stoneRadius*1.5, Point(2*(i+1.5)*self._stoneRadius,5*self._stoneRadius))
        self.add(t)
        
        t = Text(str(i+1), self._stoneRadius*1.5, Point(self._stoneRadius, 2*(i+3.5)*self._stoneRadius))
        self.add(t)
            
      # Draw the lines
      for i in range(19):
        l = Path()
        l.addPoint(Point(2*(i+1.5)*self._stoneRadius, 7*self._stoneRadius))
        l.addPoint(Point(2*(i+1.5)*self._stoneRadius, 43*self._stoneRadius))
        l.setDepth(100)
        self.add(l)
        
        l = Path()
        l.addPoint(Point(3*self._stoneRadius, 2*(i+3.5)*self._stoneRadius))
        l.addPoint(Point(39*self._stoneRadius, 2*(i+3.5)*self._stoneRadius))
        l.setDepth(100)
        self.add(l)
        
      # Setup text label at top
      self._label = Text('', self._stoneRadius*1.5, Point(21*self._stoneRadius,self._stoneRadius))
      self.add(self._label)
      
      self._circles = set()
  
      # Capture stones
      self._blackCaptures = []
      for i in range(5):
        c = Circle(self._stoneRadius, Point(2*(i+1.5)*self._stoneRadius, 2.75*self._stoneRadius))
        c.setFillColor('black')
        self._blackCaptures.append(c)
        
      self._whiteCaptures = []
      for i in range(5):
        c = Circle(self._stoneRadius, Point(2*(19.5-i)*self._stoneRadius, 2.75*self._stoneRadius))
        c.setFillColor('white')
        self._whiteCaptures.append(c)
        
      # Stones
      self._stones = []
      for r in range(19):
        row = []
        for c in range(19):
          stone = Circle(self._stoneRadius, Point(2*(c+1.5)*self._stoneRadius, 2*(r+3.5)*self._stoneRadius))
          row.append(stone)
        self._stones.append(row)
        
    
      self._considered = []
      self._selected = None
      
      # Numbers for move ordering
      self._numbers = []
      self._numbersShown = set()
      for i in range(50):
        t = Text(str(i+1), self._stoneRadius*1.5, Point(2*(i+1.5)*self._stoneRadius,5*self._stoneRadius))
        t.setDepth(-10)
        self._numbers.append(t)

    def setLabel(self, text):
      self._label.setMessage(text)

    def draw(self, state):
      # Stones
      for i in range(min(5,state.numCaptures()[0])):
        if self._blackCaptures[i] not in self._circles:
          self._circles.add(self._blackCaptures[i])
          self.add(self._blackCaptures[i])
      for i in range(min(5,state.numCaptures()[0]),5):
        if self._blackCaptures[i] in self._circles:
          self._circles.remove(self._blackCaptures[i])
          self.remove(self._blackCaptures[i])
      for i in range(min(5,state.numCaptures()[1])):
        if self._whiteCaptures[i] not in self._circles:
          self._circles.add(self._whiteCaptures[i])
          self.add(self._whiteCaptures[i])
      for i in range(min(5,state.numCaptures()[1]),5):
        if self._whiteCaptures[i] in self._circles:
          self._circles.remove(self._whiteCaptures[i])
          self.remove(self._whiteCaptures[i])
      
      for r in range(19):
        for c in range(19):
          s = state.getState([r,c])
          stone = self._stones[r][c]
          if s != 0 and stone not in self._circles:
            self._circles.add(stone)
            self.add(stone)
            
          if s == 1:
            stone.setFillColor('black')
          elif s == -1:
            stone.setFillColor('white')
            
          if s == 0 and stone in self._circles:
            self._circles.remove(stone)
            self.remove(stone)

    def clear(self):
      for c in self._considered:
        if c not in self._circles:
          self.remove(c)
      self._considered = []
 
    def considering(self, moves, selected):
      for m in moves:
        (r,c) = m
        stone = self._stones[r][c]
        
        if stone not in self._circles:   
          if m == selected:
            stone.setFillColor('red')
          else:
            stone.setFillColor('blue')         
          if stone not in self._considered:
            self.add(stone)
            self._considered.append(stone)
          
      for i in range(len(self._considered)):
        piece = self._considered[i]
        center = piece.getReferencePoint()
        number = self._numbers[i]
        number.moveTo(center.getX(), center.getY())
        if number not in self._numbersShown:
          self._numbersShown.add(number)
          self.add(number)
          
      for i in range(len(moves), 50):
        number = self._numbers[i]
        if number in self._numbersShown:
          self._numbersShown.remove(number)
          self.remove(number)
