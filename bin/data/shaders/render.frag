#version 330

uniform sampler2D tex0;
uniform vec3 inkColor;
uniform float time;

in vec2 vs_texcoord;

out vec4 out_color;

const vec4 white = vec4(1.0);

void main()
{
    // Invert the blue channel (remember: the background has b = 0.0)
    float blueChannel = 1.0 - texture(tex0, vs_texcoord).b;
    
    // Add a bit of flicker
    float flicker = 0.1 * fract(sin(time * 4.0));
    
    // Use the value of the blue channel to mix between white and the ink color
    vec4 color = mix(vec4(inkColor, 1.0) + flicker, white, blueChannel);
    
    // Use this fragment's distance to the center to lerp between two new colors
    vec4 color2 = mix(vec4(0.3, 0.0, 0.1, 1.0), white, blueChannel);
    
    float dist = distance(vs_texcoord, vec2(0.5, 0.5));
    vec4 finalColor = mix(color, color2, dist);
    
    out_color = vec4(finalColor);
}