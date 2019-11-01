#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

attribute vec3 i_position;
attribute vec2 i_texcoord;
attribute float i_speed_x;
attribute float i_speed_y;

varying vec2 v_texcoord;
varying vec2 v_speed;

void main()
{
    gl_Position = vec4(i_position, 1.0);
    v_speed = vec2(i_speed_x, i_speed_y);
    v_texcoord = i_texcoord;
}
