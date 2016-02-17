
texture2D t_texture;
float3 R_inverseFilterTextureSize = 1.0 / 512;
float R_fxaaSpanMax = 0.5;
float R_fxaaReduceMin = 0.2;
float R_fxaaReduceMul = 0.1;

 sampler2D samp = sampler_state
 {
	 Texture = t_texture;
	 AddressU = Clamp;
	 AddressV = Clamp;
	 MinFilter = Point;
	 MagFilter = Linear;
	 MipFilter = Linear;
 };

 float4 psmain(float2 tex : TEXCOORD ) : COLOR
{
	float2 texCoordOffset = R_inverseFilterTextureSize.xy;

	float3 luma = float3(0.299, 0.587, 0.114);
	float lumaTL = dot(luma, tex2D(samp, tex.xy + (float2(-1.0, -1.0) * texCoordOffset)).xyz);
	float lumaTR = dot(luma, tex2D(samp, tex.xy + (float2(1.0, -1.0) * texCoordOffset)).xyz);
	float lumaBL = dot(luma, tex2D(samp, tex.xy + (float2(-1.0, 1.0) * texCoordOffset)).xyz);
	float lumaBR = dot(luma, tex2D(samp, tex.xy + (float2(1.0, 1.0) * texCoordOffset)).xyz);
	float lumaM = dot(luma, tex2D(samp, tex.xy).xyz);

	float2 dir;
	dir.x = -((lumaTL + lumaTR) - (lumaBL + lumaBR));
	dir.y = ((lumaTL + lumaBL) - (lumaTR + lumaBR));

	float dirReduce = max((lumaTL + lumaTR + lumaBL + lumaBR) * (R_fxaaReduceMul * 0.25), R_fxaaReduceMin);
	float inverseDirAdjustment = 1.0 / (min(abs(dir.x), abs(dir.y)) + dirReduce);

	dir = min(float2(R_fxaaSpanMax, R_fxaaSpanMax),
		max(float2(-R_fxaaSpanMax, -R_fxaaSpanMax), dir * inverseDirAdjustment));

	dir.x = dir.x * step(1.0, abs(dir.x));
	dir.y = dir.y * step(1.0, abs(dir.y));

	//float dirStep = max(step(1.0, abs(dir.x)), step(1.0, abs(dir.y)));
	//dir.x = dir.x * dirStep;
	//dir.y = dir.y * dirStep;

	dir = dir * texCoordOffset;

	float3 res1 = (1.0 / 2.0) * (
		tex2D(samp, tex.xy + (dir * float2(1.0 / 3.0 - 0.5, 1.0 / 3.0 - 0.5))).xyz +
		tex2D(samp, tex.xy + (dir * float2(2.0 / 3.0 - 0.5, 2.0 / 3.0 - 0.5))).xyz);

	float3 res2 = res1 * (1.0 / 2.0) + (1.0 / 4.0) * (
		tex2D(samp, tex.xy + (dir * float2(0.0 / 3.0 - 0.5, 0.0 / 3.0 - 0.5))).xyz +
		tex2D(samp, tex.xy + (dir * float2(3.0 / 3.0 - 0.5, 3.0 / 3.0 - 0.5))).xyz);

	float lumaMin = min(lumaM, min(min(lumaTL, lumaTR), min(lumaBL, lumaBR)));
	float lumaMax = max(lumaM, max(max(lumaTL, lumaTR), max(lumaBL, lumaBR)));
	float lumaRes2 = dot(luma, res2);


	if (lumaRes2 < lumaMin || lumaRes2 > lumaMax)
		return vector(res1, 1.0);
	else
		return vector(res2, 1.0);
}


technique  FXAA
{
	pass p1
	{
		VertexShader = null;
		PixelShader = compile ps_2_0 psmain();
		ZEnable = false;
	}
}
