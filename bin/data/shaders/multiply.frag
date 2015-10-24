#version 330

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;
uniform float time;

in vec2 vs_texcoord;

out vec4 out_color;

void main()
{
    // Add a little shake to the top pigment layer
    float noise = fract(sin(time * 40.0)) * 0.001;
    
    // Mix the blurred and dry layers
    vec4 sample1 = texture(tex0, vs_texcoord);
    vec4 sample2 = texture(tex1, vs_texcoord + noise);
    vec4 paintContribution = mix(sample1, sample2, 0.7); //0.6
    
    // Edges are black (0.0) everything else in this sampler is white (1.0)
    vec4 edge = 1.0 - texture(tex2, vs_texcoord);
    vec4 edgeContribution = mix(vec4(0.3, 0.5, 0.8, 1.0), vec4(1.0), edge.r) * 0.08;
    
    // Sample the paper texture
    vec4 background = texture(tex3, vs_texcoord);
    vec3 finalColor = clamp(paintContribution * background + edgeContribution, 0.0, 1.0).rgb;
    
    out_color = vec4(finalColor, 1.0); // Changing the addition to subtraction does more weird shit
}