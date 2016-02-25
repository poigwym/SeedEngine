#include"shaderprogram.h"
#include"d3d9Texture.h"
#include"math3d.h"

namespace Seed
{
	void ShaderProgram::setValue(D3DXHANDLE h, const matrix4f &m)
	{
		ConstTable->SetMatrix(device, h, (D3DXMATRIX*)&m);
	}

	void ShaderProgram::setValue(D3DXHANDLE h, const vector3f& v)
	{
		ConstTable->SetVector(device, h, &D3DXVECTOR4(v.getX(), v.getY(), v.getZ(), 1.0f));
	}

	void ShaderProgram::setValue(D3DXHANDLE h, Texture* tex)
	{
		D3DXCONSTANT_DESC desc;
		UINT c;
		ConstTable->GetConstantDesc(h, &desc, &c);
		LPDIRECT3DTEXTURE9 d3d9tex = dynamic_cast<d3d9Texture*>(tex)->getInstance();
		device->SetTexture(desc.RegisterIndex, d3d9tex);
		device->SetSamplerState(desc.RegisterIndex, D3DSAMP_MAGFILTER, D3DX_FILTER_LINEAR);
		device->SetSamplerState(desc.RegisterIndex, D3DSAMP_MINFILTER, D3DX_FILTER_LINEAR);
		device->SetSamplerState(desc.RegisterIndex, D3DSAMP_MIPFILTER, D3DX_FILTER_LINEAR);

	}



}