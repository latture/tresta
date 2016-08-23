#version 330
in vec4 vPosition;
in vec4 vColor;
in vec3 normalInterp;

uniform float alphaCutoff = 0.01;
uniform mat4 modelview_inv;

const vec4 specColor = vec4(1.0, 1.0, 1.0, 1.0);
const vec4 sceneAmbient = vec4(0.3, 0.3, 0.3, 1.0);
const float shininess = 50.0;
const float tolerance = 1.e-6;
const int numberOfLights = 4;

out vec4 color;

struct lightSource {
    vec4 position;
    vec4 diffuse;
    vec4 specular;
    float constantAttenuation, linearAttenuation, quadraticAttenuation;
    float spotCutoff, spotExponent;
    vec3 spotDirection;
};

lightSource light0 = lightSource(
    vec4(1.0,  0.0,  0.0, 0.0),
    vec4(1.0,  1.0,  1.0, 1.0),
    vec4(1.0,  1.0,  1.0, 1.0),
    1.0, 0.0, 0.0,
    180.0, 0.0,
    vec3(0.0, 0.0, 0.0)
);

lightSource light1 = lightSource(
    vec4(-1.0, 0.0,  0.0, 0.0),
    vec4(1.0,  1.0,  1.0, 1.0),
    vec4(1.0,  1.0,  1.0, 1.0),
    1.0, 0.0, 0.0,
    180.0, 0.0,
    vec3(0.0, 0.0, 0.0)
);

lightSource light2 = lightSource(
    vec4(0.0,  0.0,  1.0, 0.0),
    vec4(1.0,  1.0,  1.0, 1.0),
    vec4(1.0,  1.0,  1.0, 1.0),
    1.0, 0.0, 0.0,
    180.0, 0.0,
    vec3(0.0, 0.0, 0.0)
);

lightSource light3 = lightSource(
    vec4(0.0,  0.0, -1.0, 0.0),
    vec4(1.0,  1.0,  1.0, 1.0),
    vec4(1.0,  1.0,  1.0, 1.0),
    1.0, 0.0, 0.0,
    180.0, 0.0,
    vec3(0.0, 0.0, 0.0)
);

lightSource lights[numberOfLights];

struct Material {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
};

bool lessThan(float a, float b, float tolerance) {
    return (b - a) > ( (abs(a) < abs(b) ? abs(b) : abs(a)) * tolerance);
}

bool equal(float a, float b, float tolerance) {
    return abs(a - b) <= ( (abs(a) < abs(b) ? abs(b) : abs(a)) * tolerance);
}

void main() {
    if (lessThan(vColor.w, alphaCutoff, tolerance))
        discard;

    Material material = Material(
        sceneAmbient,
        vColor,
        clamp(1.3 * vColor, 0.0, 1.0),
        shininess
    );

    lights[0] = light0;
    lights[1] = light1;
    lights[2] = light2;
    lights[3] = light3;

    vec3 normalDirection = normalize(normalInterp);
    vec3 viewDirection = normalize(vec3(modelview_inv * vec4(0.0, 0.0, 0.0, 1.0) - vPosition));
    vec3 lightDirection;
    float attenuation;

    // initialize total lighting with ambient lighting
    vec3 totalLighting = vec3(sceneAmbient) * vec3(material.ambient);

    for (int index = 0; index < numberOfLights; ++index) {
        if (equal(lights[index].position.w, 0.0, tolerance)) { // directional light?
            attenuation = 1.0; // no attenuation
            lightDirection = normalize(vec3(lights[index].position));
        }
        else { // point light or spotlight (or other kind of light)
            vec3 positionToLightSource = vec3(lights[index].position - vPosition);
            float distance = length(positionToLightSource);
            lightDirection = normalize(positionToLightSource);
            attenuation = 1.0 / (lights[index].constantAttenuation + lights[index].linearAttenuation * distance + lights[index].quadraticAttenuation * distance * distance);

            if (lessThan(lights[index].spotCutoff, 90.0, tolerance)) { // spotlight?
                float clampedCosine = max(0.0, dot(-lightDirection, normalize(lights[index].spotDirection)));

                if (lessThan(clampedCosine, cos(radians(lights[index].spotCutoff)), tolerance)) { // outside of spotlight cone?
                    attenuation = 0.0;
                }
                else {
                    attenuation = attenuation * pow(clampedCosine, lights[index].spotExponent);
                }
            }
        }

        vec3 diffuseReflection = attenuation * vec3(lights[index].diffuse) * vec3(material.diffuse)* max(0.0, dot(normalDirection, lightDirection));
        vec3 specularReflection;
        if (lessThan(dot(normalDirection, lightDirection), 0.0, tolerance)) { // light source on the wrong side?
            specularReflection = vec3(0.0, 0.0, 0.0); // no specular reflection
        }
        else { // light source on the right side
            specularReflection = attenuation * vec3(lights[index].specular) * vec3(material.specular) * pow(max(0.0, dot(reflect(-lightDirection, normalDirection), viewDirection)), material.shininess);
        }
        totalLighting += diffuseReflection + specularReflection;
    }

    color = vec4(totalLighting, vColor.w);
}
