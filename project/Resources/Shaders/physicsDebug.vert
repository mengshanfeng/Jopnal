// JOPNAL PHYSICS DEBUG VERTEX SHADER
//
// Jopnal license applies

//////////////////////////////////////////////


// Matrices
uniform mat4 u_PVMatrix; // Projection-view

// Vertex attributes
layout(location = 0) in vec3 a_Position;
layout(location = 3) in vec3 a_Color;

// Color to fragment shader
out vec3 vf_Color;

void main()
{
    // Calculate vertex position, the vertices are pre-transformed
    gl_Position = u_PVMatrix * vec4(a_Position, 1.0);

    // Assign color
    vf_Color = a_Color;
}