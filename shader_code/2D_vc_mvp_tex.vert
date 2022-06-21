#version 450

//the rotation data
layout(binding = 0) uniform UniformBufferObject1 {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

//outputting the colour of each vertex
layout(location = 0) out vec2 fragTexCoord;

//inputting the vertex positions and colors
layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec2 inTexCoord;

//main is invoked for every vertex
void main() {
    //gl_VertexIndex contains the index of the current vertex
    //gl_Position is the built in output
    //outputting the rotated vertex data
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 0.0, 1.0);

    //setting the variable to pass to the fragment shader
    fragTexCoord = inTexCoord;
}