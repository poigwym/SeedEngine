
matrix MW;
matrix MV;
matrix MP;
matrix MVP;

texture t_texture;

sampler texsampler = sampler_state {
	Texture = t_texture;
};


struct  vsinput{
	float4 pos : POSITION0;
	float3 normal : NORMAL0;
	float2 tex : TEXCOORD0;
};

struct  vsoutput{
	vector pos : POSITION0;
	float2 tex : TEXCOORD0;
};


void vsmain(vector pos : POSITION, float3 normal : NORMAL, float2 tex : TEXCOORD,
	out vector oPos : POSITION, out float2 oTex : TEXCOORD)
{
	vector posW = mul(pos, MW);
	oPos = mul(posW, MVP);
	oTex = tex;
}

void psmain(float2 tex : TEXCOORD, out float4 color : COLOR)
{
	color = tex2D(texsampler, tex);
	color.a = 0.5;
}

technique TransparentLighting
{
	pass Pass1
	{
		AlphaBlendEnable = TRUE;
		DestBlend = INVSRCALPHA;
		SrcBlend = SRCALPHA;
		VertexShader = compile vs_2_0 vsmain();
		PixelShader = compile ps_2_0 psmain();
	}

}


technique UIlighting
{
	pass Pass1
	{
		AlphaBlendEnable = TRUE;
		DestBlend = INVSRCALPHA;
		SrcBlend = SRCALPHA;

		VertexShader = NULL;
		PixelShader = compile ps_2_0 psmain();
	}

}