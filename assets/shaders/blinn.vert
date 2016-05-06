#version 330
in vec3 vertexPosition;
in vec3 vertexNormal;
in vec4 vertexViewCol1;
in vec4 vertexViewCol2;
in vec4 vertexViewCol3;
in vec4 vertexViewCol4;
in vec4 vertexColor;

uniform mat4 modelview;
uniform mat4 modelnormal;
uniform mat4 projection;

out vec4 vPosition;
out vec3 normalInterp;
out vec4 vColor;

void main(){
    vec4 offsetPos4 = vec4(vertexPosition, 1.0);
    mat4 vertexView = mat4(vertexViewCol1, vertexViewCol2, vertexViewCol3, vertexViewCol4);

    gl_Position = projection * modelview * (vertexView * offsetPos4);

    vPosition = gl_Position;
    normalInterp = vec3(modelnormal * vec4(vertexNormal, 0.0));
    vColor = vertexColor;
}
