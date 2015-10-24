#ifndef __cardsProject__particle__
#define __cardsProject__particle__

#include "ofMain.h"

class Particle {
    
public:
    
    Particle();
    void update();
    void draw();
    void reset();
    void checkBounds();
    
    ofVec2f position;
    ofVec2f velocity;
};

#endif