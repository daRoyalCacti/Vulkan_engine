#version 450

//there is no built in variable to output the colour
//location specifies the index of the framebuffer
layout(location = 0) out vec4 outColor;

//grabbing the fragment colour outputted from the vertex shader
layout(location = 0) in vec2 fragTexCoord;

//getting the image data
layout(binding = 1) uniform sampler2D texSampler;

//main is run for every fragment
void main() {
    outColor = texture(texSampler, fragTexCoord);    //fragColor will be automatically interpolated
}