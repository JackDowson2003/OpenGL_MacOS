#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_MVP; //Model view projection matrix

void main()
{
  gl_Position = u_MVP * position; //列向量放在右侧进行变换 保证我们的列存储的(OPENGL)
  v_TexCoord = texCoord;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
//  vec4 texColor = texture(u_Texture,v_TexCoord);
  vec4 texColor = texture(u_Texture,v_TexCoord);
  color = texColor /** u_Color*/; //rgba 分量进行点乘
//  color = vec4(1.0);
}