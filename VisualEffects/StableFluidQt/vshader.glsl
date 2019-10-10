#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

attribute vec3 i_position;
attribute vec2 i_texcoord;

varying vec2 v_texcoord;

void main()
{
    gl_Position = vec4(i_position, 1.0);
    v_texcoord = i_texcoord;
}
