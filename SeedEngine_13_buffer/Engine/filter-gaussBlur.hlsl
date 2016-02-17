

texture2D t_texture;
float3 blurScale;

sampler2D tsampler = sampler_state
{
	Texture = t_texture;
	AddressU = Clamp;
	AddressV = Clamp;
	MinFilter = Point;
	MagFilter = Linear;
	MipFilter = Linear;
};

void psmain(float2 tex : TEXCOORD, out float4 color : COLOR)
{
	 color = float4(0, 0, 0, 0);
	 color += tex2D(tsampler, tex + (float2(-3.0, -3.0)* blurScale.xy)) * (1.0 / 64.0);
	 color += tex2D(tsampler, tex + (float2(-2.0, -2.0) * blurScale.xy)) * (6.0 / 64.0);
	 color += tex2D(tsampler, tex + (float2(-1.0, -1.0) * blurScale.xy)) * (15.0 / 64.0);
	 color += tex2D(tsampler, tex + (float2(0, 0) * blurScale.xy))  * (20.0 / 64.0);
	 color += tex2D(tsampler, tex + (float2(1.0, 1.0) * blurScale.xy))  * (15.0 / 64.0);
	 color += tex2D(tsampler, tex + (float2(2.0, 2.0) * blurScale.xy))  * (6.0 / 64.0);
	 color += tex2D(tsampler, tex + (float2(3.0, 3.0) * blurScale.xy))  * (1.0 / 64.0);


}


technique GaussBlur
{
	pass p0
	{
		VertexShader = null;
		PixelShader = compile ps_2_0 psmain();
		ZEnable = false;
	}
}
