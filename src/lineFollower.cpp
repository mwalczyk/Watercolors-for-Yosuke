#include "lineFollower.h"

LineFollower::LineFollower()
{
    
}

void LineFollower::setupFromPoints(vector<ofPoint> &points)
{
    for (auto &point: points)
    {
        m_path.addVertex(point);
    }
    m_path = m_path.getSmoothed(3);
    init();
}

void LineFollower::setupFromPolyline(ofPolyline &line)
{
    m_path = line.getSmoothed(3);
    init();
}

void LineFollower::init()
{
    m_area =            m_path.getArea() * -1;
    m_perimeterLength = m_path.getPerimeter();
    m_noiseOffset =     ofRandom(1000.0f);
    m_travelSpeed =     0.03f * (100.0f / m_perimeterLength);
    m_pctTraveled =     0.0f;
    m_currentPosition = m_path.getPointAtPercent(m_pctTraveled);
    
    //We don't necessarily want to draw the whole path - we just want to trace it
    m_shouldDrawPath =  false;
}

void LineFollower::update()
{
    m_pctTraveled += m_travelSpeed;
    if (m_pctTraveled > 1.0f) m_pctTraveled = 0.0f;
    m_currentPosition = m_path.getPointAtPercent(m_pctTraveled);
}

void LineFollower::draw()
{
    if (m_shouldDrawPath) m_path.draw();
    float n = ofNoise(ofGetElapsedTimef() + m_noiseOffset);
    float r = ofMap(n, 0.0f, 1.0f, MIN_RADIUS, MAX_RADIUS, true);
    ofCircle(m_currentPosition, r);
    
    if (ofRandom(1.0f) > 0.95f)
    {
        float offsetX = ofRandom(-SPLATTER_OFFSET, SPLATTER_OFFSET);
        float offsetY = ofRandom(-SPLATTER_OFFSET, SPLATTER_OFFSET);
        if (ofRandom(1.0) > 0.5) ofSetColor(128, 128, 0);
        ofCircle(m_currentPosition.x + offsetX,
                 m_currentPosition.y + offsetY,
                 r / 2,
                 r / 2);
    }
}

/*
 *
 * This version of the draw function takes a MIN and MAX area, which are used to scale the stroke width
 * based on the area of this polyline
 *
 */
void LineFollower::draw(int minArea, int maxArea, ofImage *brush)
{
    if (m_shouldDrawPath) m_path.draw();
    float n = ofNoise(ofGetElapsedTimef() + m_noiseOffset);
    float r = ofMap(n, -1, 1, MIN_RADIUS, MAX_RADIUS, true);
    float scaleR = ofMap(m_area, minArea, maxArea, 0.5f, 1.0f);
    r *= scaleR;

    brush->draw(m_currentPosition.x - r/2, m_currentPosition.y - r/2, r, r);
    
    if (ofRandom(1.0f) > 0.95f)
    {
        float offsetX = ofRandom(-SPLATTER_OFFSET, SPLATTER_OFFSET);
        float offsetY = ofRandom(-SPLATTER_OFFSET, SPLATTER_OFFSET);
        if (ofRandom(1.0f) > 0.98f) r *= 4;
        if (ofRandom(1.0f) > 0.5f) ofSetColor(128, 128, 0);
        brush->draw(m_currentPosition.x + offsetX - r / 4,
                    m_currentPosition.y + offsetY - r / 4,
                    r / 2,
                    r / 2);
    }
}

void LineFollower::drawDebug()
{
    ofPushStyle();
    ofNoFill();
    m_path.draw();
    ofRectangle rect = m_path.getBoundingBox();
    ofRect(rect.x, rect.y, rect.width, rect.height);
    ofPopStyle();
    
    ofCircle(m_path.getCentroid2D(), 4);
}

float LineFollower::getArea()
{
    return m_area;
}

ofPolyline LineFollower::getPath()
{
    return m_path;
}

