// JOPNAL GAUSSIAN BLUR FRAGMENT SHADER
//
// Jopnal license applies

//////////////////////////////////////////////

in vec2 vf_TexCoords;

uniform sampler2D u_Buffer;

uniform bool u_Horizontal;

const float offset[3] = float[](0.0, 1.3846153846, 3.2307692308);
const float weight[3] = float[](0.3162162162, 0.2270270270, 0.0702702703);

layout(location = 0) out vec4 out_FinalColor;

void main()
{
    ivec2 size = textureSize(u_Buffer, 0);

    vec3 tempColor = texture(u_Buffer, vf_TexCoords).rgb * weight[0];

    if (u_Horizontal)
    {
        for (int i = 1; i < 3; ++i)
        {
            tempColor += texture(u_Buffer, vf_TexCoords + vec2(offset[i] / float(size.x), 0.0)).rgb * weight[i];
            tempColor += texture(u_Buffer, vf_TexCoords - vec2(offset[i] / float(size.x), 0.0)).rgb * weight[i];
        }
    }
    else
    {
        for (int i = 1; i < 3; ++i)
        {
            tempColor += texture(u_Buffer, vf_TexCoords + vec2(0.0, offset[i] / float(size.y))).rgb * weight[i];
            tempColor += texture(u_Buffer, vf_TexCoords - vec2(0.0, offset[i] / float(size.y))).rgb * weight[i];
        }
    }

	out_FinalColor = vec4(tempColor, 1.0);
}