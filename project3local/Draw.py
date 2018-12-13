from cs1graphics import *

class MazeVisualizer:
  def __init__(self, maze, size):
    self._maze = maze
    self._boxSize = size/19
    
    self._canvas = Canvas(size,size*20./19)
    self._canvas.setAutoRefresh(False)
    self._canvas.setTitle('Catch the ghosts')
    self._canvas.setBackgroundColor('tan')
    
    self._boxes = {}
    for x in range(19):
      for y in range(19):
        b = Square(self._boxSize, Point((x+.5)*self._boxSize, (y+.5)*self._boxSize))
        b.setBorderWidth(0)
        if self._maze.inMaze([x,y]):
          b.setFillColor('black')
        else:
          b.setFillColor('tan')
        self._canvas.add(b)
        self._boxes[(x,y)] = b
        
    self._behaviorBoxes = []
    for i in range(3):
      group = []
      for j in range(5):
        b = Square(self._boxSize, Point((7*i+j+.5)*self._boxSize, 19.5*self._boxSize))
        b.setBorderWidth(0)
        b.setFillColor('black')
        group.append(b)
        self._canvas.add(b)
      self._behaviorBoxes.append(group)
      
    self._circles = []
    for i in range(3):
      p = Path()
      p.setBorderWidth(.1*self._boxSize)
      p.setBorderColor( ['red','green','blue'][i] )
      for j in range(5):
        p.addPoint(Point(-1000,-1000))
      self._circles.append(p)
      self._canvas.add(p)
        
    self._pacman = Circle(.45*self._boxSize)
    self._pacman.setFillColor('yellow')
    self._pacman.moveTo(-500,-500)
    self._canvas.add(self._pacman)
    
    self._ghosts = []
    for i in range(3):
      g = Circle(.45*self._boxSize)
      g.setFillColor(['red','green','blue'][i])
      self._canvas.add(g)
      g.moveTo(-500,-500)
      self._ghosts.append(g)
        
    self._canvas.refresh()
          
    
  def draw(self, player, observations, playerPosition, ghostPositions, noisyDistances):        
    self._pacman.moveTo( (playerPosition[0]+.5)*self._boxSize, (playerPosition[1]+.5)*self._boxSize )
    for g in range(3):
      if ghostPositions[g][0] >= 0:
        
        # Calculate behavior probabilities
        probs = [0.]*5
        for ( ((x,y),b), p ) in player.calculatedGhostProbabilities(g).items():
          probs[b] += p
        M = max(probs)
        if M == 0:
          M = 1
        for i in range(5):
          v = 255.*probs[i]/M
          c = Color()
          if g == 0:
            self._behaviorBoxes[g][i].getFillColor().setByValue((int(v),0,0))
          elif g == 1:
            self._behaviorBoxes[g][i].getFillColor().setByValue((0,int(v),0))
          else:
            self._behaviorBoxes[g][i].getFillColor().setByValue((0,0,int(v)))
            
        self._ghosts[g].moveTo( (ghostPositions[g][0]+.5)*self._boxSize, (ghostPositions[g][1]+.5)*self._boxSize )
        
        x = playerPosition[0] + .5
        y = playerPosition[1] + .5
        r = noisyDistances[g] + .2*(g-1)
        self._circles[g].setPoint(Point( (x+r)*self._boxSize, (y)*self._boxSize ), 4 )
        self._circles[g].setPoint(Point( (x+r)*self._boxSize, (y)*self._boxSize ), 0 )
        self._circles[g].setPoint(Point( (x)*self._boxSize, (y+r)*self._boxSize ), 1 )
        self._circles[g].setPoint(Point( (x-r)*self._boxSize, (y)*self._boxSize ), 2 )
        self._circles[g].setPoint(Point( (x)*self._boxSize, (y-r)*self._boxSize ), 3 )
          
      else:
        for i in range(5):
          self._behaviorBoxes[g][i].setFillColor('tan')
        self._ghosts[g].moveTo(-1000,-1000)
        if self._circles[g] in self._canvas:
          self._canvas.remove(self._circles[g])
          
    distributions = []
    for g in range(3):
      dist = {}
      for ( ((x,y),b), p ) in player.calculatedGhostProbabilities(g).items():
        dist[(x,y)] = dist.get((x,y),0) + p
      distributions.append(dist)
    


    maxs = [-1000.]*3
    for x in range(19):
      for y in range(19):
        if self._maze.inMaze([x,y]):
          for g in range(3):
            if ghostPositions[g][0] >= 0:
              maxs[g] = max(maxs[g], distributions[g].get((x,y),0))
    for g in range(3):
      if maxs[g] == 0.:
        maxs[g] = 1.
    for x in range(19):
      for y in range(19):
        if self._maze.inMaze([x,y]):
          c = []
          for g in range(3):
            if ghostPositions[g][0] >= 0:
              c.append(255./maxs[g]*distributions[g].get((x,y),0))
            else:
              c.append(0.)
              
          self._boxes[(x,y)].getFillColor().setByValue(tuple([int(x) for x in c]))
          
    self._canvas.refresh()
    
    
