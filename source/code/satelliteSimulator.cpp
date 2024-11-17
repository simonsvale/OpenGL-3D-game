#include <vector>
#include <memory>
#include <math.h>

#include "satelliteSimulator.h"
#include "gameElementHandler.h"

using namespace std;

void SatelliteSimulator::UpdatePositions(vector< unique_ptr<GameElement> > &GameElementVector, double timeStamp) {

    for (int index = 1; index < GameElementVector.size(); ++index) {
        //GameElementVector[index]->WorldPosition[0] = cos(timeStamp+index/5)*0.1;
        //GameElementVector[index]->WorldPosition[1] = sin(timeStamp)*0.1;
        GameElementVector[index]->WorldPosition[2] = cos(timeStamp+double(index)/5.0)*0.1;
    }
}

void SatelliteSimulator::UpdateLinks(vector< unique_ptr<GameElement> > &GameElementVector, double timeStamp) {
    
}