#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D texture;
varying vec2 v_speed;
varying vec2 v_texcoord;
uniform float i_time;
uniform float i_deltaTime;
uniform vec2 i_forceOrigin;
uniform float i_forceExponent;
void main()
{
    vec3 color = texture2D(texture, v_texcoord);
    // + v_speed * 0.01f);
    vec3 dye = saturate(sin(i_time * vec3(2.72, 5.12, 4.98)) + 0.5);
    vec2 pos = v_texcoord;
    float amp = exp(-i_forceExponent * distance(i_forceOrigin, pos));
    color = lerp(color, dye, saturate(amp));

    gl_FragColor = vec4(color, 1.0);
}
