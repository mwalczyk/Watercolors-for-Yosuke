#version 330

in vec2 vs_texcoord;
uniform sampler2D inputTexture;
uniform float intensity;
uniform float time;
const vec3 W = vec3(0.2125, 0.7154, 0.0721);

out vec4 outputColor;

void main()
{
    float imageWidthFactor = textureSize(inputTexture, 0).x;
    float imageHeightFactor = textureSize(inputTexture, 0).y;
    vec3 textureColor = texture(inputTexture, vs_texcoord).rgb;
    
    vec2 stp0 = vec2(1.0 / imageWidthFactor, 0.0);
    vec2 st0p = vec2(0.0, 1.0 / imageHeightFactor);
    vec2 stpp = vec2(1.0 / imageWidthFactor, 1.0 / imageHeightFactor);
    vec2 stpm = vec2(1.0 / imageWidthFactor, -1.0 / imageHeightFactor);
    
    float i00   = dot( textureColor, W);
    float im1m1 = dot( texture(inputTexture, vs_texcoord - stpp).rgb, W);
    float ip1p1 = dot( texture(inputTexture, vs_texcoord + stpp).rgb, W);
    float im1p1 = dot( texture(inputTexture, vs_texcoord - stpm).rgb, W);
    float ip1m1 = dot( texture(inputTexture, vs_texcoord + stpm).rgb, W);
    float im10 = dot( texture(inputTexture, vs_texcoord - stp0).rgb, W);
    float ip10 = dot( texture(inputTexture, vs_texcoord + stp0).rgb, W);
    float i0m1 = dot( texture(inputTexture, vs_texcoord - st0p).rgb, W);
    float i0p1 = dot( texture(inputTexture, vs_texcoord + st0p).rgb, W);
    float h = -im1p1 - 2.0 * i0p1 - ip1p1 + im1m1 + 2.0 * i0m1 + ip1m1;
    float v = -im1m1 - 2.0 * im10 - im1p1 + ip1m1 + 2.0 * ip10 + ip1p1;
    
    float mag = 1.0 - length(vec2(h, v));
    vec3 target = vec3(mag);
    
    outputColor = vec4(target, 1.0);
}