#version 450

//outputting the colour of each vertex
layout(location = 0) out vec3 fragColor;

//position of each vertex
vec2 positions[3] = vec2[](
        vec2(0.0, -0.5),
        vec2(0.5, 0.5),
        vec2(-0.5, 0.5)
        );

//the colour of each vertex
vec3 colors[3] = vec3[](
        vec3(1.0, 0.0, 0.0),
        vec3(0.0, 1.0, 0.0),
        vec3(0.0, 0.0, 1.0)
        );

//main is invoked for every vertex
void main() {
    //gl_VertexIndex contains the index of the current vertex
    //gl_Position is the built in output
    gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);

    //setting the variable to pass
    fragColor = colors[gl_VertexIndex];
}