
uniform matrix MW;
uniform matrix MV;
uniform matrix MP;
uniform matrix MVP;
uniform matrix MBone[35];

uniform sampler samp_diffuse;

float3 lightpos = { 0, 10, 0 };


int BlendMatrixNum = 4;

void vsmain(
vector pos : POSITION,
float3 normal : NORMAL,
float2 tex : TEXCOORD,
float4 w : BLENDWEIGHT,
int4 index : BLENDINDICES,


out vector oPos : POSITION,
out float2 oTex : TEXCOORD0,
out float3 oNormal : NORMAL,
out float4 oPosW : TEXCOORD1
)

{
		//vector

		matrix mw
		=
		MBone[index[0]] * w[0] 
		+
		MBone[index[1]] * w[1]
		+
		MBone[index[2]] * w[2]
		+
		MBone[index[3]] * w[3]

		;

		vector
	posW = mul(pos, 
		mw
		//MBone[index[0]] * w[0]
		);
	//posW = mul(pos, MBone);


	oPosW = posW;
	oPos = mul(posW, MVP);
	oTex = tex;
	oNormal = mul(normal, MW);
}

float4 psmain(
	float3 n : NORMAL,
	float2 tex : TEXCOORD0,
	float4 posW : TEXCOORD1
	) : COLOR
{
	float diffuseColor = dot(normalize(n), normalize(lightpos - posW.xyz));
	return tex2D(samp_diffuse, tex);
	return tex2D(samp_diffuse, tex) * diffuseColor;
}



