matrix MW;
matrix MV;
matrix MP;
matrix MVP;

texture t_texture;
float4 camPos;
float3 lightDir;

float3 Idiffuse = { 1, 1, 1 }; // intensity multiplier
float3 Ispecular = { 1, 1, 1 }; // intensity multiplier

float3 AmbientLightColor = { 0, 0, 0 };
//float3 diffuseColor = { 0.3, 0.1, 0.3 };
float3 diffuseColor = { 1, 1, 1 };
float3 specularColor = { 1, 1, 1 };
float3 EmissiveColor = { 1, 1, 1 };

float SpecularPower = 200;

sampler texsampler = sampler_state {
	Texture = t_texture;
};

struct VertexShaderInput {
	float4 pos : POSITION0;
	float3 normal : NORMAL0;
	float2 tex : TEXCOORD0;
};

struct VertexShaderOutput
{
	float4 pos : POSITION0;
	float2 tex : TEXCOORD0;
	float3 normal : TEXCOORD1;
	float4 WorldPos : TEXCOORD2;
};

VertexShaderOutput DirectionLight_vs(VertexShaderInput input)
{
	VertexShaderOutput output;
	float4 posWorld = mul(input.pos, MW);
	output.WorldPos = posWorld;
	output.pos = mul(posWorld, MVP);
	output.tex = input.tex;
	output.normal = mul(input.normal, MW);
	return output;
}

float4 DirectionLight_ps(VertexShaderOutput input) : COLOR0
{
	// I = Ia*Ca*ka + tex * (Id*Cd*kd + Is*Cs*ks) / atten;

	// diffuse amount
	float diffuseLighting = saturate(dot(input.normal, -lightDir));

	// half vector 
	float3 h = normalize(normalize(camPos - input.WorldPos) - lightDir);
		float specLighting = pow(saturate(dot(h, input.normal)), SpecularPower);
	float4 texColor = tex2D(texsampler, input.tex);

		return float4(saturate(
		AmbientLightColor +
		(texColor.xyz * diffuseColor * Idiffuse * diffuseLighting * 0.6) + // Use light diffuse vector as intensity multiplier
		(specularColor * Ispecular * specLighting * 0.5) // Use light specular vector as intensity multiplier
		), texColor.w);
}


technique  directionlight
{
	pass Pass1
	{
		VertexShader = compile vs_2_0 DirectionLight_vs();
		PixelShader = compile ps_2_0 DirectionLight_ps();
	}
}

