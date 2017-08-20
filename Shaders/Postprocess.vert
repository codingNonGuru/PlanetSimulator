#version 450

layout (location = 0) uniform vec2 scale;

layout (location = 0) in vec3 position;
//layout (location = 1) in vec2 textureCoords;

out vec2 coords;

void main() {
    coords.x = position.x + 0.5f;
    coords.x *= scale.x;
    coords.y = position.y + 0.5f;
    coords.y *= scale.y;
    gl_Position = vec4(position.x * 1.0f, position.y * 1.0f, 0.1f, 1.0f);
}
