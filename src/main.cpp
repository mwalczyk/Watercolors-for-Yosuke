#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main() {
    ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
    
    //----CARDS will be 2.5x3.5" with an 1/8th inch bleed, so at 300DPI = 2.75x3.75" or 825x1125 pixels
	ofSetupOpenGL(825/2, 1125/2, OF_WINDOW);
    ofRunApp(new ofApp());
}
