#version 330

in vec2 vs_texcoord;
uniform sampler2D inputTexture;
uniform float time;

int radius = 8; // Large values (>= 20) result in very intense quantization and LOW framerates - a value of 8 is pretty solid

out vec4 outputColor;
vec2 src_size = textureSize(inputTexture, 0);

void main()
{
    // Get the UV coordinates of this fragment from the vertex shader - rename it uv
    vec2 uv = vs_texcoord;
   
    // For a radius value of 8, this equals 81
    float n = float((radius + 1) * (radius + 1));
    
    // Decalre two arrays, each of four vec3 variables
    vec3 m[4];
    vec3 s[4];
    
    // Zero out all of the vec3s
    for (int k = 0; k < 4; ++k) {
        m[k] = vec3(0.0);
        s[k] = vec3(0.0);
    }
    
    for (int j = -radius; j <= 0; ++j)  {
        for (int i = -radius; i <= 0; ++i)  {
            vec3 c = texture(inputTexture, uv + vec2(i,j) / src_size).rgb;
            m[0] += c;
            s[0] += c * c;
        }
    }
    
    for (int j = -radius; j <= 0; ++j)  {
        for (int i = 0; i <= radius; ++i)  {
            vec3 c = texture(inputTexture, uv + vec2(i,j) / src_size).rgb;
            m[1] += c;
            s[1] += c * c;
        }
    }
    
    for (int j = 0; j <= radius; ++j)  {
        for (int i = 0; i <= radius; ++i)  {
            vec3 c = texture(inputTexture, uv + vec2(i,j) / src_size).rgb;
            m[2] += c;
            s[2] += c * c;
        }
    }
    
    for (int j = 0; j <= radius; ++j)  {
        for (int i = -radius; i <= 0; ++i)  {
            vec3 c = texture(inputTexture, uv + vec2(i,j) / src_size).rgb;
            m[3] += c;
            s[3] += c * c;
        }
    }
    
    float min_sigma2 = 1e+2;
    for (int k = 0; k < 4; ++k) {
        m[k] /= n;
        s[k] = abs(s[k] / n - m[k] * m[k]);
        
        float sigma2 = s[k].r + s[k].g + s[k].b;
        if (sigma2 < min_sigma2) {
            min_sigma2 = sigma2;
            outputColor = vec4(m[k], 1.0);
        }
    }
}