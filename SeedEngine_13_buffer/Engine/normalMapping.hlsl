matrix MW;
matrix MV;
matrix MP;
matrix MVP;


texture t_texture;
texture normalMap;
vector camPos;

vector lightPos = { -3, 1, -1, 1 };
vector pointLightColor = { 0.1, 0.1, 0.4, 1 };
float3 lightDir = { 0, -1, -1 };
vector dirLightColor = { 1, 1, 1, 1 };

float SpecularPower = 20;


struct PointLight
{
	float3 pos, color;
	float Constant;
	float Linear;
	float Exponent;
};

struct DirectionLight
{
	float3 pos, dir, color;
	float Intensity;
};

PointLight pointlight;
DirectionLight directionlight;

sampler texsampler = sampler_state {
	Texture = t_texture;
	MagFilter = Linear;
	MinFilter = Anisotropic;
	MipFilter = Linear;
	MaxAnisotropy = 16;
};
sampler normalsampler = sampler_state {
	Texture = normalMap;
	MagFilter = Linear;
	MinFilter = Anisotropic;
	MipFilter = Linear;
	MaxAnisotropy = 16;
};


struct  vsinput{
	float4 pos : POSITION0;
	float3 normal : NORMAL0;
	float2 tex : TEXCOORD0;
	float4 tangent : TANGENT;
};

struct  vsoutput{
	vector pos : POSITION0;
	float2 tex : TEXCOORD0;
	float3 lightVec : TEXCOORD1;
	float3 viewVec : TEXCOORD2;
	float3 normal : TEXCOORD3;
	float4 tangent : TEXCOORD4;
};

vsoutput vsmain(vsinput input)
{
	vsoutput output = (vsoutput)0;
	vector posW = mul(input.pos, MW);
	output.pos = mul(posW, MVP);
	output.tex = input.tex;
	output.normal = mul(input.normal, (float3x3)MW);
	output.tangent = mul(input.tangent, MW);
	output.lightVec = vector(pointlight.pos, 1.0) - posW;
	output.viewVec = camPos - posW;

	return output;
}

// 数据: 5*point(0.5diffuse + specular) / atten + specular(0.8diffuse+specular)

float4 calPointLight(PointLight light, float3 n, float3 v, float3 l, float dis)
{
	float4 diffuse = saturate(dot(n, l)) * vector(light.color, 1.0);
		float3 h = normalize(v + l);
		float4 specular = pow(saturate(dot(n, h)), SpecularPower) * vector(light.color, 1.0);
		return  5 * (diffuse * 0.8 + specular) / (dis);
}

float4 calDirctionLight(DirectionLight light, float3 n, float3 v) : COLOR
{
	float3 l = -light.dir;
	float4 diffuse = saturate(dot(n, l)) * vector(light.color, 1);
	float3 h = normalize(v + l);
	float4 specular = pow(saturate(dot(n, h)), SpecularPower) * vector(light.color, 1);
	return light.Intensity * diffuse * 0.5 + specular;
}


float4 psmain(vsoutput input) : COLOR
{
	float4 texColor = tex2D(texsampler, input.tex);
	float3 n = normalize(2.0 * tex2D(normalsampler, input.tex) - 1);

	float3 N = normalize(input.normal);
	float3 T = normalize(input.tangent);
	T = normalize(T - (N*T)*N);
	float3 B = normalize(cross(N, T));

		// 法线从纹理坐标系转到 世界坐标系
	float3x3 T2W = float3x3(T, B, N);
	n = normalize(mul(n, T2W));

	float d = length(input.lightVec);
	float3 v = normalize(input.viewVec);
	float3 l = normalize(input.lightVec);  // p->l
	vector calPoint = calPointLight(pointlight, n, v, l, d);
	vector calDir = calDirctionLight(directionlight, n, v);


	return (calPoint + calDir) * texColor;
}

technique normalMapPointLight
{
	pass Pass1
	{
		VertexShader = compile vs_2_0 vsmain();
		PixelShader = compile ps_2_0 psmain();
	}
}
