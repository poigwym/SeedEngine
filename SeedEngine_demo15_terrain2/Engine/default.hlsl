uniform matrix MW;
uniform matrix MV;
uniform matrix MP;
uniform matrix MVP;

uniform sampler samp_color;



void vsmain(vector pos : POSITION, float3 normal : NORMAL, float2 tex : TEXCOORD,
	out vector oPos : POSITION, out float2 oTex : TEXCOORD0)
{
	vector posW = mul(pos, MW);
	oPos = mul(posW, MVP);
	oTex = tex;
	//oNormal = mul(normal, MW);
}

float4 psmain(float2 tex : TEXCOORD) : COLOR
{
	//return vector(tex.x, tex.y, 0, 0);
	return tex2D(samp_color, tex);
}
