#version 330 core

// Input vertex attributes
layout (location = 0) in vec3 aPos;        // Vertex position
layout (location = 1) in vec3 aNormal;     // Vertex normal
layout (location = 2) in vec2 aTexCoords;  // Texture coordinates (NEW for texture mapping)

// Output to fragment shader
out vec3 FragPos;     // Fragment position in world space
out vec3 Normal;      // Normal vector
out vec2 TexCoords;   // Texture coordinates (pass to fragment shader)

// Transformation matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // Calculate fragment position in world space
    FragPos = vec3(model * vec4(aPos, 1.0));
    
    // Calculate normal in world space (with normal matrix)
    Normal = mat3(transpose(inverse(model))) * aNormal;  
    
    // Pass texture coordinates to fragment shader (IMPORTANT!)
    TexCoords = aTexCoords;
    
    // Calculate final vertex position
    gl_Position = projection * view * vec4(FragPos, 1.0);
}