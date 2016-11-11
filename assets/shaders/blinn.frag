#version 410
in vec4 vPosition;
in vec4 vColor;
in vec3 normalInterp;

uniform float alphaCutoff = 0.0;
uniform mat4 modelview_inv;

const vec3 specColor = vec3(1.0, 1.0, 1.0);
const vec3 sceneAmbient = vec3(0.3, 0.3, 0.3);
const float shininess = 50.0;
const float tolerance = 1.e-6;
const int numberOfLights = 4;

out vec4 color;

struct lightSource {
    vec3 position;
    vec3 diffuse;
    vec3 specular;
};

lightSource light0 = lightSource(
    vec3(1.0,  0.0,  0.0),
    vec3(1.0,  1.0,  1.0),
    vec3(1.0,  1.0,  1.0)
);

lightSource light1 = lightSource(
    vec3(-1.0, 0.0,  0.0),
    vec3(1.0,  1.0,  1.0),
    vec3(1.0,  1.0,  1.0)
);

lightSource light2 = lightSource(
    vec3(0.0,  0.0,  1.0),
    vec3(1.0,  1.0,  1.0),
    vec3(1.0,  1.0,  1.0)
);

lightSource light3 = lightSource(
    vec3(0.0,  0.0, -1.0),
    vec3(1.0,  1.0,  1.0),
    vec3(1.0,  1.0,  1.0)
);

lightSource lights[numberOfLights];

struct Material {
    vec3 ambient;
    vec4 diffuse;
    vec3 specular;
    float shininess;
};

bool lessThan(float a, float b, float tolerance) {
    return (b - a) > ( (abs(a) < abs(b) ? abs(b) : abs(a)) * tolerance);
}

void main() {
    if (lessThan(vColor.w, alphaCutoff, tolerance))
        discard;

    Material material = Material(
        sceneAmbient,
        vColor,
        specColor,
        shininess
    );

    lights[0] = light0;
    lights[1] = light1;
    lights[2] = light2;
    lights[3] = light3;

    vec3 normalDirection = normalize(normalInterp);
    vec3 viewDirection = normalize(vec3(modelview_inv * vec4(0.0, 0.0, 0.0, 1.0) - vPosition));
    vec3 lightDirection, specularReflection, diffuseReflection;
    float angle;

    // initialize total lighting with ambient lighting
    vec3 totalLighting = sceneAmbient * material.ambient;

    for (int index = 0; index < numberOfLights; ++index) {
        lightDirection = lights[index].position;
        angle =  dot(normalDirection, lightDirection);
        diffuseReflection = lights[index].diffuse * vec3(material.diffuse) * max(0.0, angle);
        if (lessThan(angle, 0.0, tolerance)) { // light source on the wrong side?
            specularReflection = vec3(0.0, 0.0, 0.0); // no specular reflection
        }
        else { // light source on the right side
            specularReflection = lights[index].specular * material.specular * pow(max(0.0, dot(reflect(-lightDirection, normalDirection), viewDirection)), material.shininess);
        }
        totalLighting += diffuseReflection + specularReflection;
    }

    color = vec4(totalLighting, vColor.w);
}
