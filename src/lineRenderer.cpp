#include "lineRenderer.h"

LineRenderer::LineRenderer()
{
    
}

void LineRenderer::setup()
{
    calculateJointCentroid();
    calculateAreaMinMax();
    m_brush.loadImage("brush.png");
}

void LineRenderer::update()
{
    for (auto &lf: m_lineFollowers) lf.update();
}

bool LineRenderer::isEmpty()
{
    return m_lineFollowers.empty();
}

void LineRenderer::draw()
{
    ofPushMatrix();
    for (int i = 0; i < m_lineFollowers.size(); i++)
    {
        //Calculate the current object's fill color from two independent noise values
        float nx = ofNoise(ofGetFrameNum() * 0.01f + i) * 255.0f;
        float ny = ofNoise(ofGetFrameNum() * 0.01f + i + 1000.0f) * 255.0f;
        ofSetColor(nx, ny, 255, 30);
        m_lineFollowers[i].draw(m_minArea, m_maxArea, &m_brush);
    }
    ofPopMatrix();
}

void LineRenderer::drawDebug()
{
    ofPushStyle();
    
    ofSetColor(ofColor::green);
    ofCircle(m_jointCentroid, 8);
    
    ofSetColor(ofColor::red);
    for (auto &lf: m_lineFollowers) lf.drawDebug();
    
    ofPopStyle();
}

void LineRenderer::addFollower(LineFollower &follower)
{
    m_lineFollowers.push_back(follower);
}

void LineRenderer::calculateJointCentroid()
{
    ofPoint sum;
    for (auto &lf: m_lineFollowers)
    {
        sum += lf.getPath().getCentroid2D();
    }
    m_jointCentroid.set(sum / m_lineFollowers.size());
}

void LineRenderer::calculateAreaMinMax()
{
    float min = ofGetWidth() * ofGetHeight();
    float max = 0;
    for (auto &lf: m_lineFollowers)
    {
        float lfArea = lf.getArea();
        if (lfArea < min) min = lfArea;
        if (lfArea > max) max = lfArea;
    }
    m_minArea = min;
    m_maxArea = max;
}

void LineRenderer::clear()
{
    m_lineFollowers.clear();
}

ofPoint LineRenderer::getJointCentroid()
{
    return m_jointCentroid;
}