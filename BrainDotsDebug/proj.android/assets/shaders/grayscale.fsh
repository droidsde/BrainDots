#ifdef GL_ES
precision lowp float;
#endif

// Input
varying vec4        v_fragmentColor;
varying vec2        v_texCoord;
uniform sampler2D   u_texture;

// Gray
const float gRed    = 0.298912;
const float gGreen  = 0.586611;
const float gBlue   = 0.114478;
const vec3  gScale  = vec3(gRed, gGreen, gBlue);

uniform float u_Rate;

void main()
{
    vec4  baseColor = texture2D(u_texture, v_texCoord);
    float grayColor = dot(baseColor.rgb, gScale);
    
    // Changes colors.
    vec4 ansColor = vec4(vec3(grayColor), baseColor.a);
    vec4 blendColor = vec4(baseColor.rgb * u_Rate,1.0);
    
    ansColor = ansColor * (1.0 - u_Rate);
    ansColor = ansColor + blendColor;
    
    gl_FragColor = ansColor * v_fragmentColor.a;
}