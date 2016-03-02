
//-------------------- 多种光源，多种光照 --------------------
matrix MW;
matrix MV;
matrix MP;
matrix MVP;

texture t_texture;
sampler texsampler = sampler_state {
	Texture = t_texture;
};

vector camPos;

// Direction Light
float3 lightDir = { 0, -1, -1 };
float4 dirLightDiffuse = { 0.1, 0.1, 0.3,1 };
float4 dirLightSpecular = { 1, 1, 1, 1 };

float SpecularPower = 200;


// multi pointLight

vector multiPos[4] = {
	{ 5, 1, 5, 1 }, { -5, 1, 5, 1 }, { -5, 1, -5, 1 }, { 5, 1, -5, 1 } };
// diffuseColor
vector multiDiffuse[4] = {
	{ 0.5, 0.1, 0.3, 1.0 }, { 0.2, 0.4, 0.1, 1.0 }, { 0.2, 0.1, 0.5, 1.0 }, { 0.1, 0.5, 0.3, 1.0 } };
// specularColor
vector multiSpecular[4] = {
	{ 0.5, 0.1, 0.3, 1.0 }, { 0.2, 0.4, 0.1, 1.0 }, { 0.2, 0.1, 0.5, 1.0 }, { 0.1, 0.5, 0.3, 1.0 } };


struct  vsINPUT{
	float4 pos : POSITION0;
	float3 normal : NORMAL0;
	float2 tex : TEXCOORD0;
};

struct  vsOUTPUT {
	vector pos : POSITION0;
	float2 tex : TEXCOORD0;
	float3 normal : TEXCOORD1;
	float4 posW : TEXCOORD2;
};


vsOUTPUT multiPointLight_vs(vsINPUT input)
{
	vsOUTPUT output = (vsOUTPUT)0;
	vector posW = mul(input.pos, MW);
	output.posW = posW;
	output.pos = mul(posW, MVP);
	output.normal = mul(input.normal, MW);
	output.tex = input.tex;
	return output;
}


float4 calPointLight(vector pos, float3 normal, vector lightPos, vector diffuseColor, vector specularColor) : COLOR
{
	float3 lightVec = normalize(pos - lightPos);
	float4 diffuse = saturate(dot(normal, -lightVec)) * diffuseColor * 2;
	float3 viewVec = normalize(camPos - pos);
	float4 specular = pow(saturate(dot(normalize(viewVec - lightVec), normal)), SpecularPower) * specularColor * 3;
	return 0.8*diffuse + 0.6*specular;
}

float4 calDirLight(vector pos, float3 normal, float3 lightDir, vector diffuseColor, vector specularColor) : COLOR
{
	float4 diffuse = saturate(dot(normal, -lightDir)) * diffuseColor * 2;
	float3 viewVec = normalize(camPos - pos);
	float4 specular = pow(saturate(dot(normalize(viewVec - lightDir), normal)), SpecularPower) * specularColor * 3;
	return 0.8*diffuse + 0.6*specular;
}


float4 multiPointLight_ps(vsOUTPUT input) : COLOR
{
	float4 texColor = tex2D(texsampler, input.tex);

	input.normal = normalize(input.normal);

	float4 color = calPointLight(input.posW, input.normal, multiPos[0], multiDiffuse[0], multiSpecular[0]) +
		calPointLight(input.posW, input.normal, multiPos[1], multiDiffuse[1], multiSpecular[1]);

	color += calDirLight(input.posW, input.normal, lightDir, dirLightDiffuse, dirLightSpecular) * 0.2;

	return color * texColor;
}

technique multiPointLight
{
	pass Pass1
	{
		VertexShader = compile vs_2_0 multiPointLight_vs();
		PixelShader = compile ps_2_0 multiPointLight_ps();
	}
}







