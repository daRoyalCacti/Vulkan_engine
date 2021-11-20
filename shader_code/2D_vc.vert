#version 450

//outputting the colour of each vertex
layout(location = 0) out vec3 fragColor;

//inputting the vertex positions and colors
layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec3 inColor;

//main is invoked for every vertex
void main() {
    //gl_VertexIndex contains the index of the current vertex
    //gl_Position is the built in output
    gl_Position = vec4(inPosition, 0.0, 1.0);

    //setting the variable to pass
    fragColor = inColor;
}