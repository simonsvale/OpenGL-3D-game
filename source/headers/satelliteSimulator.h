#ifndef SATELLITE_SIMULATOR_H
#define SATELLITE_SIMULATOR_H

#include <vector>
#include <memory>

#include "gameElementHandler.h"


using namespace std;

class SatelliteSimulator  {

    public:
        SatelliteSimulator(vector< unique_ptr<GameElement> > &GameElementVector) {
            // Set initial sat positions.
        }

        void UpdatePositions(vector< unique_ptr<GameElement> > &GameElementVector);
};

#endif