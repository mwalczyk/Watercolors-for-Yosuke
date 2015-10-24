#include "inkRenderer.h"

//--------------------------------------------------------------
//              Public functions and constructors
//--------------------------------------------------------------
InkRenderer::InkRenderer()
{

}

void InkRenderer::setup(int width = 512, int height = 512, int precision = 4)
{
    m_width =       width;
    m_height =      height;
    m_precision =   precision;
    m_isRendering = true;
    
    //Clamp the precision of the FBOs
    if (m_precision > MAX_FBO_PRECISION) m_precision = MAX_FBO_PRECISION;
    else if (m_precision < MIN_FBO_PRECISION) m_precision = MIN_FBO_PRECISION;
    
    m_inkColor[0] = 0.3;
    m_inkColor[1] = 0.5;
    m_inkColor[2] = 0.8;
    m_backgroundColor.set(249, 250, 234);
    
    //Allocate and clear all FBOs
    m_inkBlurFbo.allocate(m_width, m_height, GL_RGBA, m_precision);
    m_inkBlurFbo.begin();
    ofClear(m_backgroundColor);
    m_inkBlurFbo.end();
    
    m_ping.allocate(m_width, m_height, GL_RGBA, m_precision);
    m_ping.begin();
    ofClear(RGGRAY); //50% gray
    m_ping.end();
    
    m_pong.allocate(m_width, m_height, GL_RGBA, m_precision);
    m_pong.begin();
    ofClear(RGGRAY); //50% gray
    m_pong.end();
    
    m_paintFbo.allocate(m_width, m_height, GL_RGBA, m_precision);
    m_paintFbo.begin();
    ofClear(ofColor::white);
    m_paintFbo.end();
    
    m_blurXFbo.allocate(m_width, m_height, GL_RGBA, m_precision);
    m_blurXFbo.begin();
    ofClear(ofColor::white);
    m_blurXFbo.end();
    
    m_blurYFbo.allocate(m_width, m_height, GL_RGBA, m_precision);
    m_blurYFbo.begin();
    ofClear(ofColor::white);
    m_blurYFbo.end();
    
    m_edgeFbo.allocate(m_width, m_height, GL_RGBA, m_precision);
    m_edgeFbo.begin();
    ofClear(ofColor::white);
    m_edgeFbo.end();
    
    m_noiseSpeed =      0.01f;
    m_noiseOffset =     1000.0f;
    m_minBrushSize =    1.0f;
    m_maxBrushSize =    80.0f;
    
    loadShaders();
    m_backgroundImage.loadImage("paper.jpg");
    m_brushImage.loadImage("brush.png");
    m_drawMode = MANUAL;
    
    if (!m_backgroundImage.isAllocated())
    {
        cout << "Warning: no background image has been loaded...defaulting to a white background.\n";
        m_backgroundImage.allocate(m_width, m_height, OF_IMAGE_GRAYSCALE);
        m_backgroundImage.setColor(ofColor::white);
    }
    
    //Shader specific uniforms
    m_blurAmount =          5.5f / m_width;
    m_displacementAmount =  2.0f;
    m_displacementSpeed =   0.04f;
}

void InkRenderer::update()
{
    //Only update the particle system if we're using the corresponding draw mode
    if (m_drawMode == PARTICLES) m_ps.update();

    //Swap the ping / pong FBOs
    ofFbo temp;
    temp = m_ping;
    m_ping = m_pong;
    m_pong = temp;
    
    //-------------------------------------------------------------- Draw into the ping FBO
    m_ping.begin();
    
    ofPushStyle();
    if(m_isRendering)
    {
        switch (m_drawMode) {
            case MANUAL:
            {
                if (ofGetKeyPressed())
                {
                    float nx = ofNoise(ofGetFrameNum() * 0.01f) * 255.0f;
                    float ny = ofNoise(ofGetFrameNum() * 0.01f + 1000.0f) * 255.0f;
                    ofSetColor(nx, ny, 255, 30);
                    
                    float dist = ofDist(ofGetPreviousMouseX(), ofGetPreviousMouseY(), ofGetMouseX(), ofGetMouseY());
                    float r = ofMap(dist, 0, ofGetWidth() / 14.0, m_minBrushSize * 3, m_maxBrushSize, true);
                    m_brushImage.draw(ofGetMouseX() - r/2,
                                      ofGetMouseY() - r/2,
                                      r,
                                      r);
                    
                    ofLine(ofGetPreviousMouseX(), ofGetPreviousMouseY(), ofGetMouseX(), ofGetMouseY());
                    
                    //Optional secondary stroke
                    if (ofRandom(1.0f) > 0.95f)
                    {
                        float offsetX = ofRandom(-20.0f, 20.0f);
                        float offsetY = ofRandom(-20.0f, 20.0f);
                        
                        //Sometimes, we erase instead of draw
                        if (ofRandom(1.0f) > 0.5f) ofSetColor(RGGRAY);
                        
                        m_brushImage.draw(ofGetMouseX() - r/4 + offsetX,
                                          ofGetMouseY() - r/4 + offsetY,
                                          r / 2,
                                          r / 2);
                    }
                }
                break;
            }
            case PARTICLES:
            {
                m_ps.draw();
                if (ofGetFrameNum() % 800 == 0)
                {
                    m_ps.reset();
                    clear();
                }
                break;
            }
            case FOLLOWERS:
            {
                if(m_lineRenderer) m_lineRenderer->draw();
                break;
            }
            case ERASE:
            {
                ofSetColor(RGGRAY);
                float r = ofRandom(m_minBrushSize, m_maxBrushSize);
                m_brushImage.draw(ofGetMouseX() - r/2,
                                  ofGetMouseY() - r/2,
                                  r,
                                  r);
                break;
            }
            default:
                break;
        }
    }
    ofPopStyle();
    
    m_ping.end();
    //--------------------------------------------------------------
    
    
    //-------------------------------------------------------------- Render ping into pong FBO after displacement shader
    m_pong.begin();
    m_displacementShader.begin();
    m_displacementShader.setUniformTexture("tex0", m_ping.getTextureReference(), 0);
    m_displacementShader.setUniform1f("displacementAmount", m_displacementAmount);
    m_displacementShader.setUniform1f("displacementSpeed", m_displacementSpeed);
    m_ping.draw(0, 0);
    m_displacementShader.end();
    m_pong.end();
    //--------------------------------------------------------------
}

void InkRenderer::draw()
{
    //-------------------------------------------------------------- Apply the render shader
    m_inkBlurFbo.begin();
    m_renderShader.begin();
    m_renderShader.setUniform1f("time", ofGetElapsedTimef());
    m_renderShader.setUniform3fv("inkColor", m_inkColor);
    m_pong.draw(0, 0);
    m_renderShader.end();
    m_inkBlurFbo.end();
    //--------------------------------------------------------------
    
    
    //-------------------------------------------------------------- Paint stroke shader
    m_paintFbo.begin();
    m_paintShader.begin();
    m_paintShader.setUniformTexture("inputTexture", m_inkBlurFbo.getTextureReference(), 0);
    m_inkBlurFbo.draw(0, 0);
    m_paintShader.end();
    m_paintFbo.end();
    //--------------------------------------------------------------
    
    
    //-------------------------------------------------------------- X / Y blurring
    m_blurXFbo.begin();
    m_blurXShader.begin();
    m_blurXShader.setUniformTexture("tex0", m_paintFbo.getTextureReference(), 0);
    m_blurXShader.setUniform1f("blurAmount", m_blurAmount);
    m_paintFbo.draw(0, 0);
    m_blurXShader.end();
    m_blurXFbo.end();
    
    m_blurYFbo.begin();
    m_blurYShader.begin();
    m_blurYShader.setUniformTexture("tex0", m_blurXFbo.getTextureReference(), 0);
    m_blurYShader.setUniform1f("blurAmount", m_blurAmount);
    m_blurXFbo.draw(0, 0);
    m_blurYShader.end();
    m_blurYFbo.end();
    //--------------------------------------------------------------
    
    
    //-------------------------------------------------------------- Edge detection on the original paint stroke FBO
    m_edgeFbo.begin();
    m_edgeShader.begin();
    m_edgeShader.setUniformTexture("inputTexture", m_inkBlurFbo.getTextureReference(), 0);
    m_paintFbo.draw(0,0);
    m_edgeShader.end();
    m_edgeFbo.end();
    //--------------------------------------------------------------
    
    
    //-------------------------------------------------------------- Finally, multiply the result into the paper texture
    m_multiplyShader.begin();
    m_multiplyShader.setUniformTexture("tex0", m_blurYFbo.getTextureReference(), 0);
    m_multiplyShader.setUniformTexture("tex1", m_inkBlurFbo.getTextureReference(), 1);
    m_multiplyShader.setUniformTexture("tex2", m_edgeFbo.getTextureReference(), 2);
    m_multiplyShader.setUniformTexture("tex3", m_backgroundImage.getTextureReference(), 3);
    m_multiplyShader.setUniform1f("time", ofGetElapsedTimef());
    m_blurYFbo.draw(0, 0);
    m_multiplyShader.end();
    //--------------------------------------------------------------
}

void InkRenderer::setBackgroundTexture(ofImage &background)
{
    m_backgroundImage = background;
}

void InkRenderer::setBrushTexture(ofImage &brush)
{
    m_brushImage = brush;
}

void InkRenderer::setLineRenderer(LineRenderer *lr)
{
    m_lineRenderer = lr;
}

void InkRenderer::clear()
{
    m_ping.begin();
    ofClear(128, 128, 0);
    m_ping.end();
    
    m_pong.begin();
    ofClear(128, 128, 0);
    m_pong.end();
}

void InkRenderer::setDrawMode(DRAWMODE mode)
{
    m_drawMode = mode;
    clear();
}

string InkRenderer::getDrawModeAsString()
{
    switch (m_drawMode)
    {
        case MANUAL:
            return "manual (drawing)";
        case PARTICLES:
            return "particle system";
        case FOLLOWERS:
            return "line followers";
        case ERASE:
            return "manual (erasing)";
        default:
            break;
    }
}

//--------------------------------------------------------------
//              Functions for internal use only
//--------------------------------------------------------------
void InkRenderer::loadShaders()
{
    m_displacementShader.load("shaders/passthrough.vert", "shaders/displacement.frag");
    m_renderShader.load("shaders/passthrough.vert", "shaders/render.frag");
    m_multiplyShader.load("shaders/passthrough.vert", "shaders/multiply.frag");
    m_paintShader.load("shaders/passthrough.vert", "shaders/paintstroke.frag");
    m_blurXShader.load("shaders/passthrough.vert", "shaders/blurX.frag");
    m_blurYShader.load("shaders/passthrough.vert", "shaders/blurY.frag");
    m_edgeShader.load("shaders/passthrough.vert", "shaders/edgedetect.frag");
}
