#include "particle.h"

Particle::Particle()
{
    position.set(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
    velocity.set(ofRandom(-4.0, 4.0), ofRandom(-4.0, 4.0));
}

void Particle::update()
{
    position += velocity;
    checkBounds();
}

void Particle::checkBounds()
{
    if (position.x > ofGetWidth() || position.x < 0)
    {
        velocity.x *= -1;
    }
    if(position.y > ofGetHeight() || position.y < 0)
    {
        velocity.y *= -1;
    }
}

void Particle::draw()
{
    ofCircle(position, 20);
}

void Particle::reset()
{
    position.set(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
    velocity.set(ofRandom(-2.0, 2.0), ofRandom(-2.0, 2.0));
}