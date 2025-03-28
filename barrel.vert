#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormals;
layout (location = 2) in vec2 aTex;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out vec2 uvs;
out vec3 normals;
out mat3 TBN;

uniform float scale = 1.0;
uniform float t = 0.0f; 

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    float yOffset = sin(t*2);

    /*vec3 newPosition = vec3(aPos.x, aPos.y + yOffset, aPos.z);

    gl_Position = proj * view * model * newPosition;*/

    gl_Position = (proj * view * model * vec4(aPos, 1.0)) + vec4(0.0f, yOffset, 0.0f, 0.0f);

    vec3 T = normalize(vec3(model * vec4(aTangent,   0.0)));
    vec3 B = normalize(vec3(model * vec4(aBitangent, 0.0)));
    vec3 N = normalize(vec3(model * vec4(aNormals,    0.0)));
    TBN = mat3(T, B, N);

    uvs = aTex;
    normals = aNormals;

}