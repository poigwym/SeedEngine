uniform matrix MW;
uniform matrix MV;
uniform matrix MP;
uniform matrix MLVP;
uniform matrix MVP;

uniform vector camPos;

float g_SpecularPower = 20;


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

uniform PointLight pointlight =
{
	float3(1.0, 2.0, 1.0), float3(0.5, 0.1, 0.3), 0, 0, 1
};
uniform DirectionLight directionlight =
{
	float3(1.0, 2.0, 1.0), float3(0, -1, -1), float3(1, 1, 1), 1.0
};


// tex_ to samp_
//texture tex_color;	
//texture tex_normal;
//texture tex_shadow;
uniform sampler samp_color;
uniform sampler samp_normal;
uniform sampler samp_shadow;



void vsmain(
		float4 pos : POSITION0,
		float3 normal : NORMAL0,
		float2 tex : TEXCOORD0,
		float4 tangent : TANGENT,

		out vector opos : POSITION0,
		out float2 otex : TEXCOORD0,
		out float3 olightVec : TEXCOORD1,
		out float3 oviewVec : TEXCOORD2,
		out float3 onormal : TEXCOORD3,
		out float4 otangent : TEXCOORD4,
		out float4 oposH : TEXCOORD5
)

{
	vector posW = mul(pos, MW);
	opos = mul(posW, MVP);
	otex = tex;
	onormal = mul(normal, (float3x3)MW);
	otangent = mul(tangent, MW);
	olightVec = vector(pointlight.pos, 1.0) - posW;
	oviewVec = camPos - posW;
	oposH = mul(posW, MLVP);
}

// 数据: 5*point(0.5diffuse + specular) / atten + specular(0.8diffuse+specular)

float4 calPointLight(PointLight light, float3 n, float3 v, float3 l, float dis)
{
	float4 diffuse = saturate(dot(n, l)) * vector(light.color, 1.0);
		float3 h = normalize(v + l);
		float4 specular = pow(saturate(dot(n, h)), g_SpecularPower) * vector(light.color, 1.0);
		return  5 * (diffuse * 0.8 + specular) / (dis);
}

float4 calDirctionLight(DirectionLight light, float3 n, float3 v) : COLOR
{
	float3 l = -light.dir;
	float4 diffuse = saturate(dot(n, l)) * vector(light.color, 1);
	float3 h = normalize(v + l);
	float4 specular = pow(saturate(dot(n, h)), g_SpecularPower) * vector(light.color, 1);
	return light.Intensity * diffuse * 0.5 + specular;
}


//===============================================================================
// ------------------------- cal shadow ---------------------------
int textureSIZE = 512;
int lightBleedReduction = 0.2;

float linstep(float low, float high, float v)
{
	return clamp((v - low) / (high - low), 0.0, 1.0);
}

float shadow_variance(float2 tex, float dep, float lightBleedReduction)
{
	float2 moment = tex2D(samp_shadow, tex);
		float p = step(moment.x + 1.0 / textureSIZE, dep);  // E(x) < z return 1
	float variance = max(0.1, moment.y - moment.x * moment.x);	// E(x^2) - E(x)^2;
	float d = dep - moment.x;		// z - E(x)
	float pmax = linstep(lightBleedReduction, 1.0, variance / (variance + d*d));
	pmax = 1.0 - pmax;
	return min(0.9, max(p, pmax));
}

// input posH.xy  depth
float calShadowAmt(float2 posH, float z)
{
	// 返回阴影值
	// proj坐标 -> 纹理空间坐标
	float2 shadowTex = posH.xy * 0.5 + float2(0.5, 0.5);
		shadowTex.y = 1.0 - shadowTex.y;

	if (shadowTex.x < 0 || shadowTex.y < 0 ||
		shadowTex.x > 1 || shadowTex.y > 1)
		return 0;

	return clamp(0.1, 0.8, shadow_variance(shadowTex, z, lightBleedReduction));
}
//===============================================================================

float4 psmain(
	 vector pos : POSITION0,
	 float2 tex : TEXCOORD0,
	 float3 lightVec : TEXCOORD1,
	 float3 viewVec : TEXCOORD2,
	 float3 normal : TEXCOORD3,
	 float4 tangent : TEXCOORD4,
	 float4 posH : TEXCOORD5
	
	) : COLOR
{
	float shadowAmt = calShadowAmt(posH.xy / posH.w, posH.z / posH.w);

	float4 texColor = tex2D(samp_color, tex);
	float3 n = normalize(2.0 * tex2D(samp_normal, tex) - 1);

	float3 N = normalize(normal);
	float3 T = normalize(tangent);
	T = normalize(T - (N*T)*N);
	float3 B = normalize(cross(N, T));

		// 法线从纹理坐标系转到 世界坐标系
		float3x3 T2W = float3x3(T, B, N);
		n = normalize(mul(n, T2W));

	float d = length(lightVec);
	float3 v = normalize(viewVec);
		float3 l = normalize(lightVec);  // p->l
		vector calPoint = calPointLight(pointlight, n, v, l, d);
	vector calDir = calDirctionLight(directionlight, n, v);

	return ((calPoint + calDir) * texColor)
		*
		(1 - shadowAmt);

	//return (calPoint + calDir) * texColor;
}
