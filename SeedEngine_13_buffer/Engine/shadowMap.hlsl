matrix MW;
matrix MV;
matrix MP;
matrix MVP;
matrix MLVP;

texture t_texture;
texture t_shadowMap;
vector camPos;


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
	MinFilter = Point;
	MagFilter = Linear;
	MipFilter = Linear;
};


sampler shadowSampler = sampler_state {
	Texture = t_shadowMap;
	MinFilter = Point;
	MagFilter = Linear;
	MipFilter = Linear;
	AddressU = Clamp;
	AddressV = Clamp;
	
};

void shadowVS(float4 pos : POSITION, float3 normal : NORMAL,
	out float4 oPos : POSITION, out float2 depth : TEXCOORD)
{
	oPos = mul(pos, MW);
	oPos = mul(oPos, MLVP);
	depth.xy = oPos.zw;
}

//void shadowPS(float2 depth : TEXCOORD, out float4 color : COLOR)
//{
//	color = float4(0, 0, 0, 0);
//	color.r = depth.x / depth.y;
//}
void shadowPS(float2 depth : TEXCOORD, out float4 color : COLOR)
{
	color = float4(0, 0, 0, 0);
	float d = depth.x / depth.y;
	float x = ddx(d);
	float y = ddy(d);
	color.x = d;
	color.y = d*d + 0.25*(x*x + y*y);
}


technique RenderShadow
{
	pass Pass1
	{
		VertexShader = compile vs_3_0 shadowVS();
		PixelShader = compile ps_3_0 shadowPS();
	}
}



void SceneVS(float4 pos : POSITION, float3 normal : NORMAL, float2 tex : TEXCOORD,
	out float4 oPos : POSITION, out float3 oNormal : TEXCOORD0, out float2 oTex : TEXCOORD1,
	out float4 vlPos : TEXCOORD2, 
	out float4 oPosW : TEXCOORD3)
{
	vector posW = mul(pos, MW);
	oPos = mul(posW, MVP);
	oNormal = mul(normal, MW);
	oTex = tex;
	vlPos = mul(posW, MLVP);
	oPosW = posW;
}





// 数据: 5*point(0.5diffuse + specular) / atten + specular(0.8diffuse+specular)

float4 calPointLight(PointLight light, float3 n, float3 v, float3 l, float atten)
{
	float4 diffuse = saturate(dot(n, l)) * vector(light.color, 1.0);
		float3 h = normalize(v + l);
		float4 specular = pow(saturate(dot(n, h)), SpecularPower) * vector(light.color, 1.0);
		return  5 * (diffuse * 0.8 + specular) *atten;
}

float4 calDirctionLight(DirectionLight light, float3 n, float3 v) : COLOR
{
	float3 l = -light.dir;
	float4 diffuse = saturate(dot(n, l)) * vector(light.color, 1);
	float3 h = normalize(v + l);
	float4 specular = pow(saturate(dot(n, h)), SpecularPower) * vector(light.color, 1);
	return light.Intensity * diffuse * 0.5 + specular;
}




float texelSIZE = 512;

float linearSample(float2 tex, float dep)
{
	// 手写线性采样
	//* *  以这个像素为左上角， 这个点的深度和相邻3个点的深度插值 
	//* *

	tex = tex * texelSIZE + float2(0.5, 0.5);	// [0,1]->[0,texelSIZE];
	float2 lerps = frac(tex);	// decimal
	tex = (tex - lerps) / texelSIZE; //[0,texelSIZE]->[0,1]
	float offset = 1.0 / texelSIZE;
	float a = (tex2D(shadowSampler, tex).r + offset < dep ? 1.0 : 0);
	float b = (tex2D(shadowSampler, tex + float2(offset,0)).r + offset < dep ? 1.0 : 0);
	float c = (tex2D(shadowSampler, tex + float2(0, offset)).r + offset < dep ? 1.0 : 0);
	float d = (tex2D(shadowSampler, tex + float2(offset, offset)).r + offset < dep ? 1.0 : 0);
	return lerp(lerp(a, b, lerps.x),
		lerp(c, d, lerps.x),
		lerps.y);
}

float linstep(float low, float high, float v)
{
	return clamp((v - low) / (high - low), 0.0, 1.0);
}

float varianceSample(float2 tex, float dep, float lightBleedReduction)
{
	float2 moment = tex2D(shadowSampler, tex);
	float p = step(moment.x + 1.0 / texelSIZE, dep);  // E(x) < z return 1
	float variance = max(0.1, moment.y - moment.x * moment.x);	// E(x^2) - E(x)^2;
	float d = dep - moment.x;		// z - E(x)
	float pmax = linstep(lightBleedReduction, 1.0, variance / (variance + d*d));
	pmax = 1.0 - pmax;
	return min(0.9, max(p, pmax));
}

float calShadow(float2 lPos, float z)
{
	// 返回阴影值
	// proj坐标 -> 纹理空间坐标
	float2 shadowTex = lPos.xy * 0.5 + float2(0.5, 0.5);
	shadowTex.y = 1.0 - shadowTex.y;

	if (shadowTex.x < 0 || shadowTex.y < 0 ||
		shadowTex.x > 1 || shadowTex.y > 1)
		return 0;

	return clamp(0.1, 0.8, varianceSample(shadowTex, z, 0.2));
}


void ScenePS(float3 normal : TEXCOORD0, float2 tex : TEXCOORD1, float4 vlPos : TEXCOORD2,
	float4 posW : TEXCOORD3,
	out float4 color : COLOR)
{

	float shadowAmt = calShadow(vlPos.xy / vlPos.w, vlPos.z / vlPos.w);
	float3 l = pointlight.pos - posW.xyz;
	float dis = length(l);
	float3 n = normalize(normal);
	float3 v = normalize(camPos - posW);
	l = normalize(l);
	vector 	calColor = 
		calPointLight(pointlight, n, v, l, 1.0/dis) *0.6
		+
		calDirctionLight(directionlight, n, v)*0.8
		;
	color = calColor * tex2D(texsampler, tex) * (1 - shadowAmt);

}


technique RenderScene
{
	pass Pass1
	{
		VertexShader = compile vs_3_0 SceneVS();
		PixelShader = compile ps_3_0 ScenePS();
	}
}


