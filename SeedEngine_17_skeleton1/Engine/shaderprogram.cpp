#include"shaderprogram.h"
#include"d3d9Texture.h"
#include"math3d.h"

namespace Seed
{

	void parseShaderProgram(const string &file, const string &entry, map<string, D3DXHANDLE> &uniforms,
		ID3DXConstantTable *ConstTable)
	{
		ifstream in(file.c_str(), ios::in);

		Log(file);
		assert(in.is_open() != 0);
		char buf[256];

		while (in >> buf)
		{
			if (!strcmp(buf, "uniform"))
			{
				in >> buf;
				in >> buf;
				char tmp[256];
				int len = 0;


				// 数组变量只要数组名不用[xxx]
				while (islower(buf[len]) || isupper(buf[len]) || buf[len] == '_'
					)
					tmp[len] = buf[len], len++;

				tmp[len] = 0;

				Log(tmp);
				char *samp;
				if ((samp=strstr(tmp, "samp")) != NULL)
				{
					// samp_  -> tex_
					uniforms[string("tex_") + (char*)(tmp+5)] = ConstTable->GetConstantByName(0, tmp);
				}
				else 
					uniforms[tmp] = ConstTable->GetConstantByName(0, tmp);
			}
		}

		in.close();
	}


	void ShaderProgram::setValue(D3DXHANDLE h, const matrix4f &m)
	{
		ConstTable->SetMatrix(device, h, (D3DXMATRIX*)&m);
	}

	void ShaderProgram::setValue(D3DXHANDLE h, const matrix4f *m, int n)
	{
		//static D3DXMATRIX d3dmat[36];
		//for (int i = 0; i < n; ++i)
			//memcpy(d3dmat + i, m + i, sizeof(matrix4f));

		//ConstTable->SetMatrixArray(device, h, d3dmat, n);
		ConstTable->SetMatrixArray(device, h, (D3DXMATRIX*)m, n);
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