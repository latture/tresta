#version 330
in vec3 vertexPosition;
in vec3 vertexNormal;

uniform mat4 vertexView;
uniform vec4 vertexColor;
uniform mat4 modelview;
uniform mat4 modelnormal;
uniform mat4 projection;

out vec3 vPosition;
out vec3 normalInterp;
out vec4 vColor;

void main(){
    vec4 offsetPos4 = vec4(vertexPosition, 1.0);

    gl_Position = projection * modelview * (vertexView * offsetPos4);
    vPosition = vec3(gl_Position);
    normalInterp = vec3(modelnormal * vec4(vertexNormal, 0.0));
    vColor = vertexColor;
}
