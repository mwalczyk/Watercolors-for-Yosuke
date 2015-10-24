#include "particleSystem.h"

ParticleSystem::ParticleSystem()
{
    for (int i = 0; i < NUM_PARTICLES; i++)
    {
        m_particles.push_back(Particle());
    }
    m_brushImage.loadImage("brush.png");
}

void ParticleSystem::update()
{
    for(auto &particle: m_particles) particle.update();
}

void ParticleSystem::draw()
{
    for (int i = 0; i < m_particles.size(); i++) {
        for (int j = 0; j < m_particles.size(); j++) {
            ofVec2f sep = m_particles[i].position - m_particles[j].position;
            float dist = sep.length();
            if (dist > 0 && dist < DIST_THRESHOLD)
            {
                float nx = ofNoise(ofGetFrameNum() * 0.01f + i + j) * 255.0f;
                float ny = ofNoise(ofGetFrameNum() * 0.01f + i + j + 1000.0f) * 255.0f;
                ofSetColor(nx, ny, 255, 2);
                ofLine(m_particles[i].position.x, m_particles[i].position.y, m_particles[j].position.x, m_particles[j].position.y);
                
                float brushSize = ofRandom(1.0f, BRUSH_SIZE);
            
                m_brushImage.draw(m_particles[i].position.x - brushSize/2,
                                  m_particles[i].position.y - brushSize/2,
                                  brushSize,
                                  brushSize);
                m_brushImage.draw(m_particles[j].position.x - brushSize/2,
                                  m_particles[j].position.y - brushSize/2,
                                  brushSize,
                                  brushSize);
                
                if (ofRandom(1.0f) > 0.95f)
                {
                    float offsetX = ofRandom(-20.0f, 20.0f);
                    float offsetY = ofRandom(-20.0f, 20.0f);
                    
                    if (ofRandom(1.0f) > 0.9f) ofSetColor(RGGRAY);
                    
                    m_brushImage.draw(m_particles[i].position.x + offsetX - brushSize / 4,
                                      m_particles[i].position.y + offsetY - brushSize / 4,
                                      brushSize / 4,
                                      brushSize / 4);
                }
                
            }
        }
    }
}

void ParticleSystem::reset()
{
    for(auto &particle: m_particles) particle.reset();
}