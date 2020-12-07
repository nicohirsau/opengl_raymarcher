#version 330 core
layout (location = 0) in vec2 vertexPosition;
out vec2 TexCoord;

uniform mat4 u_projectionMatrix;
uniform mat4 u_modelMatrix;

void main(){
    gl_Position = u_projectionMatrix * u_modelMatrix * vec4(vertexPosition.x, vertexPosition.y, 0.0, 1.0);
    TexCoord = vertexPosition;
}