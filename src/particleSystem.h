#ifndef __cardsProject__particleSystem__
#define __cardsProject__particleSystem__

#include "ofMain.h"
#include "particle.h"

class ParticleSystem
{
    
public:
    
    ParticleSystem();
    void update();
    void draw();
    void reset();

private:
    
    const ofColor       RGGRAY = ofColor(128, 128, 0);
    const int           NUM_PARTICLES = 100;
    const int           DIST_THRESHOLD = 50;
    const int           BRUSH_SIZE = 40;
    ofImage             m_brushImage;
    vector<Particle>    m_particles;
};

#endif