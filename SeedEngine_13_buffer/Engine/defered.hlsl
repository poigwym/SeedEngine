
matrix MW;
matrix MV;
matrix MP;
matrix MVP;

texture t_texture;
float3 camPos;
float SpecularPower = 20;

// MRT
texture t_diffuse, t_normal, t_depth;


sampler samplerDiffuse = sampler_state {
	Texture = t_diffuse;
	MagFilter = Point;
	MinFilter = Point;
};
sampler samplerNormal = sampler_state {
	Texture = t_normal;
	MagFilter = Point;
	MinFilter = Point;
};

sampler samplerDepth = sampler_state {
	Texture = t_depth;
	MagFilter = Point;
	MinFilter = Point;
};

sampler samplerTexture = sampler_state {
	Texture = t_texture;
	MagFilter = Linear;
	MinFilter = Anisotropic;
	MipFilter = Linear;
	MaxAnisotropy = 16;
};


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


// first pass
// 返回normalV, homo下的深度


void vsGeo(
	vector pos : POSITION, 
	float3 normal : NORMAL, 
	float2 tex : TEXCOORD,

	out vector oposH : POSITION, 
	out float2 otex: TEXCOORD0,
	out float3 onormalV: TEXCOORD1,
	out float2 odepth: TEXCOORD2 
	)
{
	vector posW = mul(pos, MW);
	oposH = mul(posW, MVP);
	odepth = oposH.zw;											// z in homogenous
	otex = tex;
	onormalV = mul(normal, mul(MW, MV));
	//onormalV = mul(normal, MW);
}

//输出：diffuse(纹理)， normalV 和 homo下的depth到RT
void  psGeo(
	float2 tex : TEXCOORD0,
	float3 normal : TEXCOORD1,
	float2 depth : TEXCOORD2,
	out float4 odiffuse: COLOR0,
	out float4 onormalV: COLOR1,
	out float4 odepth: COLOR2
	)
{

	odiffuse = tex2D(samplerTexture, tex);
	onormalV = vector(normalize(normal)*0.5 + 0.5, 1.0f);	// [-1, 1]->[0,1]
	odepth = vector(depth.x / depth.y *0.5 + 0.5, 0, 0, 0); // [-1, 1] -> [0,1]
}


float4 calPointLight(PointLight light, float3 n, float3 v, float3 l, float dis)
{
	float4 diffuse = saturate(dot(n, l)) * vector(light.color, 1.0);
		float3 h = normalize(v + l);
		float4 specular = pow(saturate(dot(n, h)), SpecularPower) * vector(light.color, 1.0);
		return   (diffuse + specular) / (dis);
}

float4 calDirctionLight(DirectionLight light, float3 n, float3 v) : COLOR
{
	float3 l = -light.dir;
	l = mul(l, (float3x3) MV);
	float4 diffuse = saturate(dot(n, l)) * vector(light.color, 1.0f);
	float3 h = normalize(v + l);
	float4 specular = pow(saturate(dot(n, h)), SpecularPower) * vector(light.color, 1);
	return light.Intensity * diffuse * 0.5 + specular;
}


// input screen texcoord
float4 psLighting(float2 screen : TEXCOORD) : COLOR 
{
	float zHomo = tex2D(samplerDepth, screen).x * 2 - 1.0f;
	// cal xyz in View space
	float zView = MP[3].z / (zHomo - MP[2].z);		// zHomor -> zView
	float xView = (2 * screen.x - 1)*zView / MP[0].x;
	float yView = (1 - 2 * screen.y)*zView / MP[1].y;
	
	// in view space
	float3 pos = float3(xView, yView, zView);
	float3 n = tex2D(samplerNormal, screen).xyz * 2 - 1.0f;
	float3 l = mul(vector(pointlight.pos, 1.0), MV).xyz - pos;
	float d = length(l);
	l = normalize(l);
	float3 v = normalize(-pos);

	
	float4 shade =
									0.8*calPointLight(pointlight, n, v, l, d) * vector(0.5, 0.1, 0.3,1.0)
									+
								 0.6*calDirctionLight(directionlight, n, v)
								 ;

	float4 color = tex2D(samplerDiffuse, screen);
	return color*shade;

}


technique DeferedShading
{
	pass Pass1
	{
		VertexShader = compile vs_3_0 vsGeo();
		PixelShader = compile ps_3_0 psGeo();
	}
	pass Pass2
	{
		VertexShader = NULL;
		PixelShader = compile ps_3_0 psLighting();
	}
}

