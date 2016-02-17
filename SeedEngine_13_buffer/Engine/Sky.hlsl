
float4x4 MW;
float4x4 MV;
float4x4 MP;
float4x4 MVP;

float3 camPos;

Texture t_sky;

samplerCUBE SkyBoxSampler = sampler_state
{
	texture = <t_sky>;
	magfilter = LINEAR;
	minfilter = LINEAR;
	mipfilter = LINEAR;
	AddressU = Mirror;
	AddressV = Mirror;
};


samplerCUBE SkySphereSampler = sampler_state
{
	texture = <t_sky>;
	magfilter = LINEAR;
	minfilter = LINEAR;
	mipfilter = LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

void skysphereVS(vector pos : POSITION,
	out vector oposH : POSITION,
	out float3 oview : TEXCOORD
	)
{
	oposH = mul(pos, mul(MW, MVP));			// local pos * MVP
	oposH.w = oposH.z;					// make z/w=1.0
	oview = pos.xyz;
}

vector skyspherePS(float3 tex : TEXCOORD) : COLOR
{
	tex = normalize(tex);
	return texCUBE(SkySphereSampler, tex);

}



technique SkySphere
{
	pass Pass1
	{
		CullMode = None;
		VertexShader = compile vs_2_0 skysphereVS();
		PixelShader = compile ps_2_0 skyspherePS();
	}
}