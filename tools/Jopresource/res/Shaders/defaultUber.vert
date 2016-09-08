// JOPNAL DEFAULT VERTEX UBERSHADER
//
// Jopnal license applies

//////////////////////////////////////////////

#include <Jopnal/Compat/Attributes>
#include <Jopnal/Compat/Varyings>

// Vertex attributes
JOP_ATTRIBUTE(0) vec4 a_Position;
JOP_ATTRIBUTE(1) vec2 a_TexCoords;
JOP_ATTRIBUTE(2) vec3 a_Normal;
JOP_ATTRIBUTE(3) vec4 a_Color;

// Matrices
uniform mat4 u_VMMatrix;
uniform mat4 u_PVMMatrix;
uniform mat3 u_NMatrix;

// Vertex attributes to fragment shader
JOP_VARYING_OUT vec3 vf_Position;
JOP_VARYING_OUT vec2 vf_TexCoords;
JOP_VARYING_OUT vec3 vf_Normal;
JOP_VARYING_OUT vec4 vf_Color;

void main()
{
    // Assign attributes
    vf_Position     = (
    #if !defined(JDRW_SKYBOX) && !defined(JDRW_SKYSPHERE)
        u_VMMatrix * 
    #endif
                      a_Position).xyz;
    vf_TexCoords    = a_TexCoords;
    vf_Normal       = normalize(u_NMatrix * a_Normal);
    vf_Color        = a_Color;

    // Calculate and assign position
    gl_Position = (u_PVMMatrix * a_Position)
    
    #if (defined(JDRW_SKYBOX) || defined(JDRW_SKYSPHERE))
        .xyww
    #endif
    ;
}