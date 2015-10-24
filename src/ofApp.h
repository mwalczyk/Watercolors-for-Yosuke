#pragma once

#include "ofMain.h"
#include "particleSystem.h"
#include "inkRenderer.h"
#include "lineRenderer.h"
#define DEBUG

class ofApp : public ofBaseApp
{

public:
    
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    
    InkRenderer             m_inkRenderer;
    LineRenderer            m_lineRenderer;
    vector<ofPolyline>      m_polyLines;
    ofPolyline              m_currentLine;
    bool                    m_displayLines;
    bool                    m_showDrawLocations;
    vector<ofPoint>         m_drawLocations;
    
};
