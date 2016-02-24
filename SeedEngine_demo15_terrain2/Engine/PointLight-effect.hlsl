matrix MW;
matrix MV;
matrix MP;
matrix MVP;

texture t_texture;
vector camPos;
//vector lightPos = { 0, 1, 0, 1 };

float3 Idiffuse = { 10, 10, 10 }; // intensity multiplier
float3 Ispecular = { 1, 1, 1 }; // intensity multiplier

float3 AmbientLightColor = { 0, 0, 0 };
float3 diffuseColor = { 0.3, 0.1, 0.3 };
float3 specularColor = { 1, 1, 1 };
float3 EmissiveColor = { 1, 1, 1 };

float SpecularPower = 200;

sampler texsampler = sampler_state {
	Texture = t_texture;
};

//- -------------------- single pointlight ---------------------


vector lightPos = { 5, 1, 5, 1 };
// vector camPos;

struct  vsinput{
	float4 pos : POSITION0;
	float3 normal : NORMAL0;
	float2 tex : TEXCOORD0;
};

struct  vsoutput{
	vector pos : POSITION0;
	float2 tex : TEXCOORD0;
	float3 normal : TEXCOORD1;
	float3 lightVec : TEXCOORD2;
	float3 viewVec : TEXCOORD3;
};


vsoutput vsmain(vsinput input)
{
	vsoutput output = (vsoutput)0;
	vector posW = mul(input.pos, MW);
	output.pos = mul(posW, MVP);
	output.normal = mul(input.normal, MW);
	output.tex = input.tex;
	output.lightVec = lightPos - posW;
	output.viewVec = camPos - posW;
	return output;
}


float4 psmain(vsoutput input) : COLOR
{
	float4 texColor = tex2D(texsampler, input.tex);

	float3 n = normalize(input.normal);
	float3 l = normalize(input.lightVec);
	float3 h = normalize(normalize(input.viewVec) + l);
	float4 lightcolor = { 1, 1, 1, 1 };
	float4 diffuse = saturate(dot(n, l)) * lightcolor;
	float4 specular = pow(saturate(dot(n, h)), 20) * lightcolor;
	return (diffuse*0.8 + specular*0.3) * texColor;
}


technique pointLight
{
	pass Pass1
	{
		VertexShader = compile vs_2_0 vsmain();
		PixelShader = compile ps_2_0 psmain();
	}
}







// ------------------------ multi-pointlight -----------------------

vector multiPos[4] = {
	{ 5, 1, 5, 1 }, { -5, 1, 5, 1 }, { -5, 1, -5, 1 }, { 5, 1, -5, 1 } };
vector multiDiffuse[4] = {
	{ 0.5, 0.1, 0.3, 1.0 }, { 0.1, 0.5, 0.3, 1.0 }, { 0.2, 0.1, 0.5, 1.0 }, { 0.2, 0.4, 0.1, 1.0 } };
vector multiSpecular[4] = {
	{ 0.5, 0.1, 0.3, 1.0 }, { 0.1, 0.5, 0.3, 1.0 }, { 0.2, 0.1, 0.5, 1.0 }, { 0.2, 0.4, 0.1, 1.0 } };


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
		float4 diffuse = saturate(dot(normalize(normal), -lightVec)) * diffuseColor * 2;
		float3 viewVec = normalize(camPos - pos);
		float4 specular = pow(saturate(dot(normalize(viewVec - lightVec), normal)), SpecularPower) * specularColor * 3;
		return 0.8*diffuse + 0.6*specular;
}

float4 multiPointLight_ps(vsOUTPUT input) : COLOR
{
	float4 texColor = tex2D(texsampler, input.tex);

	input.normal = normalize(input.normal);
	float4 color = calPointLight(input.posW, input.normal, multiPos[0], multiDiffuse[0], multiSpecular[0]) +
		calPointLight(input.posW, input.normal, multiPos[1], multiDiffuse[1], multiSpecular[1]) +
		calPointLight(input.posW, input.normal, multiPos[2], multiDiffuse[2], multiSpecular[2]);
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