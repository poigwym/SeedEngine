
matrix MW;
matrix MV;
matrix MP;
matrix MVP;

texture t_texture;

sampler texsampler = sampler_state {
	Texture = t_texture;
};



void vsmain(vector pos : POSITION, float3 normal : NORMAL, float2 tex : TEXCOORD,
				out vector oPos : POSITION, out float2 oTex : TEXCOORD0, out float3 oNormal : TEXCOORD1)
{
	vector posW = mul(pos, MW);
	oPos = mul(posW, MVP);
	oTex = tex;
	oNormal = mul(normal, MW);
}

void psmain(float2 tex : TEXCOORD, out float4 color : COLOR)
{
	color = tex2D(texsampler, tex);
}

technique defaultLighting
{
	pass Pass1
	{
		//CullMode = None;
		VertexShader = compile vs_2_0 vsmain();
		PixelShader = compile ps_2_0 psmain();
	}

}


technique UIlighting
{
	pass Pass1
	{
		VertexShader = NULL;
		PixelShader = compile ps_2_0 psmain();
	}

}