import argparse
import sys
from time import *

from Ghostbusting import *

parser = argparse.ArgumentParser(description='Test ghost location predication')
parser.add_argument('scenario', type=int, help="The scenario to attempt")
args = parser.parse_args()

numScenarios = 5
if not 1 <= args.scenario <= numScenarios:
  print('Scenario must be between 1 and', numScenarios)
  sys.exit(0)

maze = Maze()

# Define the scenarios
#    Each is a quad:  num_ghosts, input probs, observations, output probs
#    Note that all observations begin with setting the player position
scenarios = []

# Scenario 1:  ghost caught
numGhosts = 1
inputProb = [{ ((9,7),0) : .2, ((9,7),2) : .2, ((10,7),0) : .2, ((1,1),4) : .4 }]
observations = [('Player move', 9, 7), ('Ghost caught', 0, -1)]
outputProb = [{ ((9,7),0) : .5, ((9,7),2) : .5 }]
scenarios.append((numGhosts, inputProb, observations, outputProb))

# Scenario 2:  ghost not caught
numGhosts = 1
inputProb = [{ ((9,7),0) : .2, ((9,7),2) : .2, ((10,7),0) : .2, ((1,1),5) : .4 }, {((2,1),0) : 1.}, {((2,1),0) : 1.}]
observations = [('Player move', 9, 7), ('Ghost not caught', 0, -1)]
outputProb = [{ ((10,7),0) : .333333, ((1,1),5) : .666667 }]
scenarios.append((numGhosts, inputProb, observations, outputProb))

# Scenario 3:  noisy distance
numGhosts = 1
inputProb = [{ ((9,7),0) : .2, ((9,7),2) : .2, ((10,7),0) : .2, ((1,1),5) : .4 }]
observations = [('Player move', 8,7), ('Noisy distance', 0, 3)]
outputProb = [{ ((9,7),0) : .3125, ((9,7),2) : .3125, ((10,7),0) : .375}]
scenarios.append((numGhosts, inputProb, observations, outputProb))

# Scenario 4:  ghost running away with position merging
numGhosts = 1
inputProb = [ {((1,1),0) : .2, ((2,1), 1): .3, ((1,2), 1) : .5 } ]
observations = [('Player move', 9, 7), ('Ghost move', 0, -1)]
outputProb = [ {((1,1),0) : .2, ((1,1), 1): .8} ]
scenarios.append((numGhosts, inputProb, observations, outputProb))

# Scenario 5:  three ghosts
numGhosts = 3
inputProb = [ { ((9,7),0) : .2, ((9,7),2) : .2, ((10,7),0) : .2, ((1,1),5) : .4 }, {((1,1),0) : .2, ((2,1), 1): .3, ((1,2), 1) : .5 }, { ((9,7),0) : .2, ((9,7),2) : .2, ((10,7),0) : .2, ((1,1),5) : .4 }]
observations = [('Player move', 9, 7), ('Ghost caught', 0, -1), ('Ghost not caught', 1, -1), ('Ghost not caught', 2, -1), ('Ghost move', 1, -1), ('Ghost move', 2, -1), ('Player move', 9, 7), ('Noisy distance', 1, 12), ('Noisy distance', 2, 2)]
outputProb = [ { ((9,7),0):.5, ((9,7),2):.5}, {((1,1),0): .2, ((1,1),1): .8}, { ((10,7),0): 1.} ]
scenarios.append((numGhosts, inputProb, observations, outputProb))


# Test the specified scenario
(numGhosts, inputProb, obs, outputProb) = scenarios[args.scenario-1]
player = MyPlayer(maze, numGhosts)
player.clearCalculatedGhostProbabilities()
for g in range(numGhosts):
  for (k,v) in inputProb[g].items():
    player.setCalculatedGhostProbability(g, list(k[0]), k[1], v)
observations = Observations(numGhosts)
for (d,d1,d2) in obs:
  observations.addObservation(Observation(d,d1,d2))
player.getMove(observations)
resultProb = [player.calculatedGhostProbabilities(g) for g in range(3)]

# Display the scenario and results
print('')
for g in range(numGhosts):
  print('Input probabilities for ghost %d:\n' % g)
  print('\t Position\t Behavior\t Probability')
  for k in sorted(inputProb[g].keys()):
    print('\t',str(k[0]).ljust(8), '\t', str(k[1]).ljust(8),'\t','%6.4f' % inputProb[g][k])
  print('')
print('Observations:\n')
print('\t Description'.ljust(20),'\t Data 1\t Data 2')
for i in range(observations.numObservations()):
  o = observations.getObservation(i)
  d = o.description()
  d1 = o.data1()
  d2 = o.data2()
  print('\t', d.ljust(20),'\t',d1,'\t',d2)
print('')
for g in range(numGhosts):
  print('Output probabilities for ghost %d:\n' % g)
  print('\t Position\t Behavior\t Your prob\t Correct prob')
  keys = set(outputProb[g].keys())
  for k in resultProb[g].keys():
    keys.add(k)
  keys = list(keys)
  keys.sort()
  
  for k in keys:
    print('\t', str(k[0]).ljust(8),'\t',str(k[1]).ljust(8),'\t',('%6.4f' % resultProb[g].get(k,0.)).ljust(9), '\t %6.4f' % outputProb[g].get(k,0.))
