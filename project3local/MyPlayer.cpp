#include "MyPlayer.h"
#include <iostream>
MyPlayer::MyPlayer(const Maze& maze, int numGhosts) : Player(maze, numGhosts) {}

Direction MyPlayer::getMove(Observations &observations)
{
    int obsIterator = 0;
    std::map< std::tuple<Position, int>, double> newProbMap[3];
    for (int ghostId = 0; ghostId <= 2; ghostId++)  //....looping ghosts
    {
        if (observations.ghostAlive(ghostId))
        {
            std::map<std::tuple<Position,int>,double> currProbMap = calculatedGhostProbabilities(ghostId);

            for (auto item : currProbMap)
            {
                Position currGhostPos = std::get<0>(item.first);
                int currGhostBeh = std::get<1>(item.first);
                double currGhostProb = item.second;

                Ghost hypGhost = Ghost(_maze, currGhostBeh, currGhostPos);
                Position hypGhostPos = currGhostPos;
                int hypGhostBeh = currGhostBeh;
                double hypGhostProb = currGhostProb;

                Position playerPos = getPosition(); //curr player position

                int currObsNum = obsIterator;
                int numObs = observations.numObservations();
                for (currObsNum; currObsNum < numObs; currObsNum++)
                {
                    Observation currObs = observations.getObservation(currObsNum);

                    std::string descr = currObs.description();
                    if (descr.compare("Player move") == 0) //update player position
                    {
                        playerPos[0] = currObs.data1();
                        playerPos[1] = currObs.data2();
                    }
                    if (descr.compare("Ghost move") == 0) //update hypothetical ghost position
                        hypGhostPos = hypGhost.getPosition();

                    if (descr.compare("Ghost not caught") == 0) //if ghost is in same position as player, zero out
                        if (playerPos == hypGhostPos) { hypGhostProb = 0; }

                    if (descr.compare("Ghost caught") == 0) //if ghost is in different position as player, zero out
                        if (playerPos != hypGhostPos) { hypGhostProb = 0; }

                    if (descr.compare("Noisy distance") == 0) //get noisy distance
                    {
                        int noisyDist = currObs.data2();
                        int actualDist = _maze.manhattanDistance(playerPos, hypGhostPos);
                        hypGhostProb *= _maze.noisyDistanceProb(noisyDist, actualDist);
                    }

                }//-----Finish Looping Observations
                if (hypGhostProb > 0)
                {
                    double updatedProb = currGhostProb + hypGhostProb;
                    currGhostProb = updatedProb;
                    newProbMap[ghostId][std::make_tuple(currGhostPos,currGhostBeh)] += updatedProb;
                }
            }//*******Finish Looping Hypothetical Ghost ghostId
        } //if ghost alive
    }//.....Finish Looping Ghosts
    obsIterator = observations.numObservations();
    clearCalculatedGhostProbabilities();
    
    Position playPos = getPosition();
    int minDist = 1000;
    Position closestGhostPos;

    double sum;
    for (int gId = 0; gId <= 2; gId++)
    {
        sum = -1;
        for (auto newItem : newProbMap[gId])
        {
            if (sum == -1) { sum = newItem.second; }
            else { sum += newItem.second; }
        }
        for (auto newItem: newProbMap[gId]) //make all probabilities add up to 1
        {
            Position pos = std::get<0>(newItem.first);
            int beh = std::get<1>(newItem.first);
            double p = newItem.second/sum;

            int diff = _maze.mazeDistance(playPos, pos);
            if (diff < minDist) { 
                minDist = diff; 
                closestGhostPos = pos;
            }

            setCalculatedGhostProbability(gId, pos, beh, p);
        }
    }

    auto possibleMoves = _maze.validMoves(getPosition());
    //Direction move = possibleMoves[rand() % possibleMoves.size()];
    Direction move = _maze.directionToward(playPos, closestGhostPos);
    return move;
}
