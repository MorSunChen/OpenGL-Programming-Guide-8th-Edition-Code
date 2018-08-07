#version 430 core 

uniform vec4 Ambient;

uniform vec3 LightColor;
uniform vec3 LightPosition;
uniform float Shininess;
uniform float Strength;

uniform vec3 EyeDirection;
uniform float Attionuation;

in vec4 color;
in vec3 Normal;
in vec4 Pos;

out vec4 FragColor;

void main()
{
    vec3 lightDir = vec3(Pos.xyz) - LightPosition;
    float len = length(lightDir);
    lightDir = lightDir/len;
    vec3 HalfVector = normalize(EyeDirection - lightDir);
    float Attenua = 1 / (Attionuation*len*len*len);
    
    
    
    float diffuse = max(0.0f, -1*dot(Normal, lightDir)) *0.5;
    float specular = max(0.0f, 1*dot(Normal, HalfVector));
    if(diffuse <= 0.00001)
        specular = 0.0f;
    else
        specular = pow(specular, Shininess);

    vec4 scatteredLight = Ambient + vec4(LightColor * diffuse*Attenua, 0.0); 
    vec4 ReflectLight = vec4(LightColor*specular*Strength*Attenua, 0.0);

    FragColor = min(color*scatteredLight+ReflectLight, vec4(1.0));
    
}
