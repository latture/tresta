#version 330
in vec3 vPosition;
in vec4 vColor;
in vec3 normalInterp;

const vec3 specColor = vec3(1.0, 1.0, 1.0);
const vec3 sceneAmbient = vec3(.3, .3, .3);

uniform vec3 lightPos = vec3(0.0, 10.0, 0.0);

out vec4 color;

void main() {

  vec3 diffuseColor = vec3(vColor);
  vec3 ambientColor = sceneAmbient * diffuseColor;

  vec3 normal = normalize(normalInterp);
  vec3 lightDir = normalize(lightPos - vPosition);

  float lambertian = max(dot(lightDir,normal), 0.0);
  float specular = 0.0;

  if(lambertian > 0.0) {

    vec3 viewDir = normalize(-vPosition);

    // this is blinn phong
    vec3 halfDir = normalize(lightDir + viewDir);
    float specAngle = max(dot(halfDir, normal), 0.0);
    specular = pow(specAngle, 64.0);

    // this is phong (for comparison)
    // vec3 reflectDir = reflect(-lightDir, normal);
    // specAngle = max(dot(reflectDir, viewDir), 0.0);
    // note that the exponent is different here
    //specular = pow(specAngle, 4.0);
  }

  color = vec4(ambientColor + lambertian * diffuseColor + specular * specColor, vColor.w);
}
