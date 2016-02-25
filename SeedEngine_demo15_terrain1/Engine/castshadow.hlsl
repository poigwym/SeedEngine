uniform matrix MW;
uniform matrix MLVP : matrix_light_view_projection;


void vsmain(
	float4 pos : POSITION, 
	float3 normal : NORMAL,
	out float4 oPos : POSITION, 
	out float2 depth : TEXCOORD
	)
{
	oPos = mul(pos, MW);
	oPos = mul(oPos, MLVP);
	depth.xy = oPos.zw;
}


float4 psmain(
	float2 depth : TEXCOORD ):COLOR
{
	float d = depth.x / depth.y;
	float x = ddx(d);
	float y = ddy(d);
	return float4(d, d*d + 0.25*(x*x + y*y), 0, 0);
}