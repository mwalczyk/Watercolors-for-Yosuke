#version 150

uniform sampler2DRect tex0; // paint image
uniform sampler2DRect tex1; // displacement image
uniform float time;
uniform float displacementAmountX;
uniform float displacementAmountY;

in vec2 texCoordVarying;
out vec4 outputColor;

void main()
{
    vec2 st = texCoordVarying.st;
    vec2 stOrig = st;
    
    st *= vec2(0.3, 0.3);  // zooms in on the rock texture was 0.5
    vec4 color = texture(tex1, st);
    
    // 50.0 = how much displacement, try 50 or 100, etc.  always use decimal nums (10.0, etc)
    vec2 st2 = stOrig + vec2( (color.r - 0.5) * displacementAmountX, (color.b - 0.5) * displacementAmountY);
    vec4 color2 = texture(tex0, st2);
    
    outputColor = vec4(color2.r, color2.g, color2.b, color2.a);
}
