#version 450

//there is no built in variable to output the colour
//location specifies the index of the framebuffer
layout(location = 0) out vec4 outColor;

//grabbing the fragment colour outputted from the vertex shader
layout(location = 0) in vec3 fragColor;

//main is run for every fragment
void main() {
    outColor = vec4(fragColor, 1.0);    //fragColor will be automatically interpolated
}