
matrix MW;
matrix MV;
matrix MP;
matrix MVP;

texture t_texture;

float3 gLightPos = float3(0, 0, 0);

sampler texsampler = sampler_state {
	Texture = t_texture;
};



void vsBloom(vector pos : POSITION, float3 normal : NORMAL, float2 tex : TEXCOORD,
	out vector oPosH : POSITION,
	out float2 oTex : TEXCOORD0, 
	out float3 oNormal : TEXCOORD1,
	out vector oPosW  : TEXCOORD2
	)
{
	vector posW = mul(pos, MW);
	oPosW = posW;
	oPosH = mul(posW, MVP);
	oTex = tex;
	oNormal = mul(normal, MW);
}

float4 psBloom(float2 tex : TEXCOORD0, float3 normalW : TEXCOORD1, vector posW: TEXCOORD2) : COLOR
{
	//color = tex2D(texsampler, tex);
	float3 lpos = mul(vector(gLightPos + float3(0, 0, -1), 1.0), MW).xyz;
	float3 l = lpos - posW.xyz;
	//if (length(l) > 1.1) return vector(0, 0, 0, 1);
	float atten = 1.0 / length(l);
	return dot(normalize(l), normalW)*vector(1,1,1,1)*atten;
}

technique BillboardBloom
{
	pass Pass1
	{
		CullMode = None;
		AlphaBlendEnable = TRUE;
		SrcBlend = SRCALPHA;
		DestBlend = INVSRCALPHA;
		BlendOp = Max;
		VertexShader = compile vs_2_0 vsBloom();
		PixelShader = compile ps_2_0 psBloom();
	}

}
