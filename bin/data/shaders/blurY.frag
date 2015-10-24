#version 150

uniform sampler2D tex0;
uniform float blurAmount;

in vec2 vs_texcoord;
out vec4 outputColor;

//void main()
//{
//  vec4 color;
//
//  // Convolution Kernel: http://en.wikipedia.org/wiki/Kernel_(image_processing)#Convolution
//  color += 1.0 * texture(tex0, vs_texcoord + vec2(0.0, blurAmount * 4.0));
//  color += 2.0 * texture(tex0, vs_texcoord + vec2(0.0, blurAmount * 3.0));
//  color += 3.0 * texture(tex0, vs_texcoord + vec2(0.0, blurAmount * 2.0));
//  color += 4.0 * texture(tex0, vs_texcoord + vec2(0.0, blurAmount * 1.0));
//
//  color += 5.0 * texture(tex0, vs_texcoord + vec2(0.0, blurAmount));
//
//  color += 4.0 * texture(tex0, vs_texcoord + vec2(0.0, blurAmount * -1.0));
//  color += 3.0 * texture(tex0, vs_texcoord + vec2(0.0, blurAmount * -2.0));
//  color += 2.0 * texture(tex0, vs_texcoord + vec2(0.0, blurAmount * -3.0));
//  color += 1.0 * texture(tex0, vs_texcoord + vec2(0.0, blurAmount * -4.0));
//
//  color /= 25.0;
//
//  outputColor = color;
//}


float SCurve (float value) {
    
    
    // How to do this without if-then-else?
    // +edited the too steep curve value
    
    if (value < 0.5)
    {
        return value * value * 2.0;
    }
    
    else
    {
        value -= 1.0;
        
        return 1.0 - value * value * 2.0;
    }
}

vec4 BlurH (sampler2D source, vec2 size, vec2 uv, float radius) {
    
    if (radius >= 1.0)
    {
        vec4 A = vec4(0.0);
        vec4 C = vec4(0.0);
        
        float width = 1.0 / size.x;
        
        float divisor = 0.0;
        float weight = 0.0;
        
        float radiusMultiplier = 1.0 / radius;
        
        // Hardcoded for radius 20 (normally we input the radius
        // in there), needs to be literal here
        
        for (float x = -20.0; x <= 20.0; x++)
        {
            A = texture(source, uv + vec2(x * width, 0.0));
            
            weight = SCurve(1.0 - (abs(x) * radiusMultiplier));
            
            C += A * weight;
            
            divisor += weight;
        }
        
        return vec4(C.r / divisor, C.g / divisor, C.b / divisor, 1.0);
    }
    
    return texture(source, uv);
}

vec4 BlurV (sampler2D source, vec2 size, vec2 uv, float radius) {
    
    if (radius >= 1.0)
    {
        vec4 A = vec4(0.0);
        vec4 C = vec4(0.0);
        
        float height = 1.0 / size.y;
        
        float divisor = 0.0;
        float weight = 0.0;
        
        float radiusMultiplier = 1.0 / radius;
        
        for (float y = -20.0; y <= 20.0; y++)
        {
            A = texture(source, uv + vec2(0.0, y * height));
            
            weight = SCurve(1.0 - (abs(y) * radiusMultiplier));
            
            C += A * weight;
            
            divisor += weight;
        }
        
        return vec4(C.r / divisor, C.g / divisor, C.b / divisor, 1.0);
    }
    
    return texture(source, uv);
}

void main()
{
    
    vec2 uv = vs_texcoord.xy;// / textureSize(tex0, 0).xy;
    
    vec4 A = BlurV(tex0, textureSize(tex0, 0).xy, uv, 20.0);
    
    // second pass (How do I render to an intermediate texture
    // in Shadertoy? Is it possible? Or is it just the wind,
    // whistling by the castle window?
    // vec4 A = BlurV(iChannel0, iResolution.xy, uv, radius);
    
    outputColor = A;
}
