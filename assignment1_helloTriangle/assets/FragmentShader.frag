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
    vec3 result;

    float waveY = step(TexCoord.y, _FillAmount + sin(_Time));
    
    //Create sin wave
    float sinWave = sin(_Time * _FillAmount);

    //Cache TexCoord as UV
    vec2 UV = TexCoord;

    //If coord at fill line, add sin wave to coord
   // if (UV.y < _FillAmount + 0.01 && UV.y > _FillAmount - 0.01)
   // {
    //    UV.y += sinWave;
    //}

    //Sample textures
    if (UV.y < _FillAmount)
    {
        result = texture(_TextureOne, UV).rgb;
    }
    else
    {
        result = texture(_TextureTwo, UV).rgb;
    }
    
    FragColor = vec4(result, 1.0);
} 