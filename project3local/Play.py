import argparse
import random
from time import *

from Ghostbusting import *

parser = argparse.ArgumentParser(description='Capture the ghosts.')
parser.add_argument('number_of_ghosts', type=int, help="The number of ghosts to capture (1, 2 or 3)")
parser.add_argument('--stationary', '-s', action="store_true")
parser.add_argument('--graphics', '-g', type=int, help="Display the solution using graphics with specified window size", default=-1)
args = parser.parse_args()

maze = Maze()

if args.graphics > 0:
  from Draw import *
  visualizer = MazeVisualizer(maze, args.graphics)
else:
  visualizer = None

ghosts = []
behaviors = []
for i in range(args.number_of_ghosts):
  if args.stationary:
    behavior = 0
  else:
    behavior = random.randint(0,4)
  behaviors.append(behavior)
  
  pos = [-1,-1]
  while pos[0] == -1 or pos == [9,7] or not maze.inMaze(pos):
    pos[0] = random.randint(0,19)
    pos[1] = random.randint(0,19)
  
  ghosts.append(Ghost(maze, behavior, pos))
    
  print('Ghost', i, 'created with behavior', behavior, 'at', ghosts[-1].getPosition()[0], ghosts[-1].getPosition()[1])
    
player = MyPlayer(maze, args.number_of_ghosts)
observations = Observations(args.number_of_ghosts)

startTime = time()
lastGhostMoveTime = time()
while observations.numAliveGhosts() > 0:
  print()
  print('Elapsed time %.2f' % (time()-startTime))
  print(observations.numAliveGhosts(), 'ghosts alive')
  
  print('Player', player.getPosition())  

  # Observe player position so that it's there for all future computations
  observations.addObservation(Observation('Player move', player.getPosition()[0], player.getPosition()[1]))

  
  # Observe noisy distances
  noisyDistances = []
  for g in range(args.number_of_ghosts):
    if observations.ghostAlive(g):
      print('Ghost', ghosts[g].getPosition(), 'behavior', behaviors[g])
      nd = maze.noisyDistance(player.getPosition(), ghosts[g].getPosition())
      md = maze.manhattanDistance(player.getPosition(), ghosts[g].getPosition())
      noisyDistances.append(nd)
      observations.addObservation(Observation('Noisy distance', g, nd))
      print('Noisy distance to ghost', g, 'is', nd, ', actual =', md)
    else:
      noisyDistances.append(-1)      
  
  # Display
  drawStart = time()
  print('Drawing ghost positions')
  ghostPositions = [ ghosts[i].getPosition() if observations.ghostAlive(i) else [-1000,-1000] for i in range(args.number_of_ghosts) ]
  while len(ghostPositions) < 3:
    ghostPositions.append([-1000,-1000])
  if visualizer:
    visualizer.draw(player, observations, player.getPosition(), ghostPositions, noisyDistances)
  else:
    player.draw(observations, player.getPosition(), ghostPositions, noisyDistances)
  extraTime = time() - drawStart
  lastGhostMoveTime += extraTime
  startTime += extraTime
  
  # Player move
  playerStartTime = time()
  player.startTimer()
  move = player.getMove(observations)
  waitTime = 2. - (time() - playerStartTime)
  if waitTime > 0:
    print('Waiting %.2f seconds for player move' % waitTime)
    sleep(waitTime)
  
  # Move ghosts as many times as is allowed
  while time() > lastGhostMoveTime + 3:
    print('%.2f seconds remaining for the ghosts to move' % (time()-lastGhostMoveTime))
    for g in range(args.number_of_ghosts):
      if observations.ghostAlive(g):
        ghosts[g].move(player.getPosition())
        observations.addObservation(Observation('Ghost move', g, -1))
        print('Ghost', g, 'moved to', ghosts[g].getPosition()[0], ghosts[g].getPosition()[1])
        if player.getPosition() == ghosts[g].getPosition():
          print('Ghost', g, 'is caught')
          observations.ghostDied(g)
        else:
          observations.addObservation(Observation('Ghost not caught', g, -1))
    
    lastGhostMoveTime += 3.
  
  # And then move the player
  player.movePlayer(move)
  observations.addObservation(Observation('Player move', player.getPosition()[0], player.getPosition()[1]))
  print('Player moved to', player.getPosition()[0], player.getPosition()[1])
  for g in range(args.number_of_ghosts):
    if observations.ghostAlive(g) and player.getPosition() == ghosts[g].getPosition():
      observations.ghostDied(g)
      print('Ghost', g, 'is caught')

print('Total elapsed time:', time() - startTime)
