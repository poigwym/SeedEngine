


matrix MW;		// world
matrix MV;		// view
matrix MP;		// proj
matrix MVP;		// view proj
matrix MRF;		// reflect

texture t_mirror;

float3 camPos;

sampler texsampler = sampler_state {
	Texture = t_mirror;
};



void vsmain(vector pos : POSITION, float3 normal : NORMAL, float2 tex : TEXCOORD,
	out vector oPos : POSITION, 
	out float3 oNormal : TEXCOORD0, 
	out float3 oPosW : TEXCOORD1)
{
	vector posW = mul(pos, MW);
	oPos = mul(posW, MVP);
	oNormal = mul(normal, MW);
	oPosW = posW.xyz;
}

float4 psmain(
						float3 normal : TEXCOORD0,
						float3 posW : TEXCOORD1) : COLOR
{

	float3 v = normalize(camPos - posW);
	float3 n = normalize(normal);
	// in sphere space 
	v = normalize(mul(v, MRF));
	n = normalize(mul(n, MRF));

	float3 r = normalize(2.0*n*dot(n, v) - v);
		float3 h = normalize(r + float3(0, 0, 1.0));
		return
		//0.5*
		tex2D(texsampler, float2(h.x*0.5 + 0.5, -h.y*0.5 + 0.5))
		//+
		//0.5*
		//vector(0, 0, 1.0, 1.0)
		;
}

technique spheremapLighting
{
	pass Pass1
	{
		VertexShader = compile vs_2_0 vsmain();
		PixelShader = compile ps_2_0 psmain();
	}

}