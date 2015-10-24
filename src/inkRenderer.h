#ifndef __cardsProject__inkRenderer__
#define __cardsProject__inkRenderer__

#include "ofMain.h"
#include "particleSystem.h"
#include "lineRenderer.h"

class InkRenderer {
public:
    
    enum DRAWMODE
    {
        MANUAL,
        PARTICLES,
        FOLLOWERS,
        ERASE
    };
    DRAWMODE m_drawMode;
    
    InkRenderer();
    void setup(int width, int height, int precision);
    void update();
    void draw();
    void setBackgroundTexture(ofImage &background);
    void setBrushTexture(ofImage &brush);
    void setLineRenderer(LineRenderer *lr);
    void setDrawMode(DRAWMODE mode);
    string getDrawModeAsString();
    void clear();
    
private:
    
    void loadShaders();
    
    //-------------------------------------------------------------- General settings
    const ofColor   RGGRAY = ofColor(128, 128, 0);
    const int       MAX_FBO_PRECISION = 16;
    const int       MIN_FBO_PRECISION = 4;
    int             m_width;
    int             m_height;
    int             m_precision;
    bool            m_isRendering;
    float           m_noiseSpeed;
    float           m_noiseOffset;
    float           m_minBrushSize;
    float           m_maxBrushSize;
    
    //-------------------------------------------------------------- FBOs, shaders, and textures
    float           m_inkColor[3];
    ofColor         m_backgroundColor;
    ofImage         m_backgroundImage;
    ofImage         m_brushImage;
    ofImage         m_grungeImage;
    ofFbo           m_inkBlurFbo;
    ofFbo           m_ping;
    ofFbo           m_pong;
    ofFbo           m_renderFbo;
    ofFbo           m_paintFbo;
    ofFbo           m_blurXFbo;
    ofFbo           m_blurYFbo;
    ofFbo           m_edgeFbo;
    ofShader        m_displacementShader;
    ofShader        m_renderShader;
    ofShader        m_multiplyShader;
    ofShader        m_edgeShader;
    ofShader        m_paintShader;
    ofShader        m_blurXShader;
    ofShader        m_blurYShader;
    
    //-------------------------------------------------------------- Shader uniforms
    float           m_blurAmount;
    float           m_displacementAmount;
    float           m_displacementSpeed;
    
    //-------------------------------------------------------------- Geometry
    ParticleSystem  m_ps;
    LineRenderer    *m_lineRenderer;
    
};

#endif