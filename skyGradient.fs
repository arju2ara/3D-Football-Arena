#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
in float vSkyT;
uniform sampler2D skyTex;
uniform bool useTexture;
uniform vec3 skyTop;
uniform vec3 skyMiddle;
uniform vec3 skyBottom;

void main()
{
    if(useTexture){
        FragColor = texture(skyTex, TexCoord);
    } else {
        float t = clamp(vSkyT, 0.0, 1.0);

        // Realistic sky: horizon warm glow blending into deep blue zenith
        // Use smooth hermite interpolation for natural color transitions
        vec3 horizon   = skyBottom;   // warm light near horizon
        vec3 mid       = skyMiddle;   // transition zone
        vec3 zenith    = skyTop;      // deep blue at top

        // Smooth 3-band blend with wide overlap for soft, natural look
        float lowBlend  = smoothstep(0.0, 0.45, t);
        float highBlend = smoothstep(0.30, 0.85, t);

        vec3 lowerSky = mix(horizon, mid, lowBlend);
        vec3 upperSky = mix(mid, zenith, highBlend);
        vec3 sky = mix(lowerSky, upperSky, smoothstep(0.25, 0.60, t));

        // Subtle warm haze glow near the horizon (sun scatter effect)
        float hazeStrength = 0.12;
        float haze = exp(-t * 4.5);   // exponential falloff from horizon
        vec3 hazeColor = vec3(1.0, 0.92, 0.75);  // warm golden haze
        sky = mix(sky, hazeColor, haze * hazeStrength);

        // Very subtle brightness boost at the center-bottom (sun direction)
        float sunGlow = exp(-pow((TexCoord.x - 0.5) * 2.5, 2.0) - pow(t * 3.0, 2.0));
        sky += vec3(0.08, 0.06, 0.02) * sunGlow;

        FragColor = vec4(sky, 1.0);
    }
}
