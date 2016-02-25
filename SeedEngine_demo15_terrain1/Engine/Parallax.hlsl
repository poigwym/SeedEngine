matrix MW;
matrix MV;
matrix MP;
matrix MVP;


texture t_texture;
texture normalHeightMap;
vector camPos;


vector lightPos = { -3, 1, -1, 1 };
vector pointLightColor = { 0.1, 0.1, 0.4, 1 };
float3 lightDir = { 0, -1, -1 };
vector dirLightColor = { 1, 1, 1, 1 };

float3 Idiffuse = { 10, 10, 10 }; // intensity multiplier
float3 Ispecular = { 1, 1, 1 }; // intensity multiplier

float3 AmbientLightColor = { 0, 0, 0 };
float3 diffuseColor = { 0.3, 0.1, 0.3 };
float3 specularColor = { 1, 1, 1 };
float3 EmissiveColor = { 1, 1, 1 };

float SpecularPower = 20;
float g_fHeightMapScale = 0.1;


sampler texsampler = sampler_state {
	Texture = t_texture;
	MagFilter = Linear;
	MinFilter = Anisotropic;
	MipFilter = Linear;
	MaxAnisotropy = 16;
};
sampler normalHeightsampler = sampler_state {
	Texture = normalHeightMap;
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
};


vsoutput vsmain(vsinput In)
{
	vsoutput o = (vsoutput)0;
	vector posW = mul(In.pos, MW);
	o.pos = mul(posW, MVP);

	float3 n = normalize(mul(In.normal, (float3x3)MW));
	float3 t = normalize(mul(In.tangent, MW));
	float3 b = normalize(cross(n, t));

	float3x3 tbn = float3x3(t.x, b.x, n.x,
													t.y, b.y, n.y,
													t.z, b.z, n.z);

	o.lightVec = mul(lightPos - posW, tbn);
	o.viewVec = mul(camPos - posW, tbn);
	o.normal = mul(n, tbn);

	o.tex = In.tex;
	return o;
}

// 数据: 5*point(0.5diffuse + specular) / atten + specular(0.8diffuse+specular)
float4 calDirctionLight(float3 n, float3 v, float3 l, vector diffuseColor, vector specularColor)
{
	float4 diffuse = saturate(dot(n, l)) * diffuseColor;
		float3 h = normalize(v + l);
		float4 specular = pow(saturate(dot(n, h)), SpecularPower) * specularColor;
		return diffuse * 0.5 + specular;
}


float4 calPointLight(float3 n, float3 v, float3 l, vector diffuseColor, vector specularColor)
{
	float4 diffuse = saturate(dot(n, l)) * diffuseColor;
		float3 h = normalize(v + l);
		float4 specular = pow(saturate(dot(n, h)), SpecularPower) * specularColor;
		return diffuse * 0.8 + specular;
}



float2 calIntersectPoint(float2 tex, float3 v, float3 normal)
{

	float offsetDir = normalize(v.xy) * length(v.xy) / v.z * g_fHeightMapScale;
	v = normalize(v);
	int nSample = lerp(1, 50, dot(v, normalize(normal)));
	float dl = 1.0f / nSample;

	

	float h = 1.0;		// polygon的原高度
	float2 curOffset = float2(0, 0), preOffset = curOffset;
	float curHeight = 1.0f, preHeight = 1.0f;

	float dx = ddx(tex);
	float dy = ddy(tex);

	int i = 0;
	while (i < nSample)
	{
		curHeight = tex2Dgrad(normalHeightsampler, tex + curOffset, dx, dy).a;
		if (curHeight > h)
			i = nSample + 1;
		else
		{
			h -= dl;
			preHeight = curHeight;
			preOffset = curOffset;
			curOffset -= dl * offsetDir;
			++i;
		}
	}

	float delta1 = curHeight - h;
	float delta2 = (h + dl) - preHeight;
	float ratio = delta1 / (delta1 + delta2);
	float2 totOffset = ratio * preOffset + (1.0 - ratio)*curOffset;

		return tex + totOffset;
}




float4 psmain(vsoutput In) : COLOR
{

	float2 tex = calIntersectPoint(In.tex, In.viewVec, In.normal);

	float d = length(In.lightVec);
	float3 v = normalize(In.viewVec);
	float3 l = normalize(In.lightVec);  // p->l
	float3 h = normalize(v + l);


		// cal normal and texturecolor with the new tex
	float3 n = normalize(2.0 * tex2D(normalHeightsampler, tex) - 1);
	float4 texColor = tex2D(texsampler, tex);

	vector calPoint = calPointLight(n, v, l, pointLightColor, pointLightColor) * 5;
	calPoint /= d;
	vector calDir = calDirctionLight(n, v, -lightDir, dirLightColor, dirLightColor);

	return (calPoint + calDir) * texColor;
}






technique Parallax
{
	pass Pass1
	{
		VertexShader = compile vs_3_0 vsmain();
		PixelShader = compile ps_3_0 psmain();
	}
}
