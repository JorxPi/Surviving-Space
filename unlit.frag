#version 330 core
out vec4 FragColor;

in vec3 color;

in vec2 TexCoords;

uniform sampler2D tex0;

uniform vec3 tint = vec3(1.0, 1.0, 1.0);

void main()
{
    vec4 texColor = texture(tex0, TexCoords);
    FragColor = vec4(texColor.r, texColor.g, texColor.b, texColor.a);

   //FragColor = vec4(TexCoords, 0.0f, 1.0f);
   //FragColor = vec4(color, 1.0f);
}