#version 150

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

in vec2 rect_size_[];
in vec4 rect_color_[];

out vec4 rect_finalColor;

uniform mat4 rect_pmat;

void main()
{
    gl_Position = rect_pmat * (gl_in[0].gl_Position + vec4(0.0, 0.0, 0.0, 0.0));
    rect_finalColor = rect_color_[0];
    EmitVertex();

    gl_Position = rect_pmat * (gl_in[0].gl_Position + vec4(0.0, rect_size_[0].y, 0.0, 0.0));
    rect_finalColor = rect_color_[0];
    EmitVertex();

    gl_Position = rect_pmat * (gl_in[0].gl_Position + vec4(rect_size_[0].x, 0.0, 0.0, 0.0));
    rect_finalColor = rect_color_[0];
    EmitVertex();

    gl_Position = rect_pmat * (gl_in[0].gl_Position + vec4(rect_size_[0].x, rect_size_[0].y, 0.0, 0.0));
    rect_finalColor = rect_color_[0];
    EmitVertex();
    
    EndPrimitive();
}
