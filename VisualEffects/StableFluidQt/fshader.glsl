#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D texture;
varying vec2 v_texcoord;

void main()
{
    vec4 c = texture2D(texture, v_texcoord);
    c.x += 0.003f;
    c.y += 0.003f;
    c.z += 0.003f;
    c.x -= 1.0f - step(c.x, 1.0f);
    c.y -= 1.0f - step(c.y, 1.0f);
    c.z -= 1.0f - step(c.z, 1.0f);
    gl_FragColor = c;
}
