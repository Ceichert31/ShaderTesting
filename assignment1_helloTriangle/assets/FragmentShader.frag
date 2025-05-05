#version 330 core
out vec4 FragColor;
  
in vec3 vertColor; // the input variable from the vertex shader (same name and same type)  
in vec2 TexCoord;

uniform sampler2D _TextureOne;
uniform sampler2D _TextureTwo;
uniform float _Time;
uniform float _FillAmount;

void main()
{
    vec3 result = texture(_TextureOne, TexCoord).rgb;;

    //Sample textures
    if (TexCoord.y < _FillAmount)
    {
        result = texture(_TextureOne, TexCoord).rgb;
    }
    else
    {
        result = texture(_TextureTwo, TexCoord).rgb;
    }
    
    FragColor = vec4(result, 1.0);
} 