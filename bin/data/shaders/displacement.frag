#version 330

uniform sampler2D tex0;
uniform float displacementAmount;
uniform float displacementSpeed;

in vec2 vs_texcoord;

out vec4 out_color;

void main()
{
    vec2 res = textureSize(tex0, 0);
    
    // Use the R and G channels to displace the surrounding fragments
    vec2 noiseValues = texture(tex0, vs_texcoord).rg;
    vec2 displace = (noiseValues - vec2(0.5, 0.5)) / res.x;
    
    // Scale the displacement
    displace *= displacementAmount;
    float b = texture(tex0, vs_texcoord + displace).b;
    
    // We mix the displacement with a neutral value to slow down the noise
    noiseValues = mix(noiseValues, vec2(0.5, 0.5), displacementSpeed);
    
    out_color = vec4(noiseValues.r, noiseValues.g, b, 1.0);
}