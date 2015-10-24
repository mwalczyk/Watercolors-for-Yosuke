#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    ofDisableArbTex();
    ofEnableAlphaBlending();
    
    m_inkRenderer.setup(ofGetWidth(), ofGetHeight(), 8);
    m_displayLines = true;
    
    int numCircles = (int)ofRandom(3, 6);
    for (int i = 0; i <= numCircles; i++)
    {
        m_drawLocations.push_back(ofPoint(ofRandom(80, ofGetWidth()-80),
                                          ofRandom(120, ofGetHeight()-120)));
    }
    m_showDrawLocations = false;
}

//--------------------------------------------------------------
void ofApp::update()
{
    m_inkRenderer.update();
    m_lineRenderer.update();
}

//--------------------------------------------------------------
void ofApp::draw()
{
    m_inkRenderer.draw();
    ofSetColor(255, 70);
    if (m_displayLines)
    {
        m_currentLine.draw();
        for (auto &line: m_polyLines) line.draw();
    }
    
    
    if (m_showDrawLocations)
    {
        ofSetColor(ofColor::red);
        for (auto &p: m_drawLocations) ofCircle(p, 4);
    }
  
    ofSetColor(80, 80, 80);
    ofDrawBitmapString("Mode: " + m_inkRenderer.getDrawModeAsString(), 20, 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    switch (key) {
        case 'd':
            m_displayLines = !m_displayLines;
            break;
        case 't':
            m_showDrawLocations = !m_showDrawLocations;
            break;
        case 'p':
            m_inkRenderer.setDrawMode(InkRenderer::PARTICLES);
            break;
        case 'm':
            m_inkRenderer.setDrawMode(InkRenderer::MANUAL);
            break;
        case ' ':
            for (auto &line: m_polyLines)
            {
                LineFollower lf;
                lf.setupFromPolyline(line);
                m_lineRenderer.addFollower(lf);
            }
            m_lineRenderer.setup();
            m_inkRenderer.setLineRenderer(&m_lineRenderer);
            m_inkRenderer.setDrawMode(InkRenderer::FOLLOWERS);
            break;
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
    m_currentLine.addVertex(ofPoint(x, y));
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
    m_currentLine.clear();
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
    m_polyLines.push_back(m_currentLine);
}
