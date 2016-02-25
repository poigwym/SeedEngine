#pragma once

#include"prequest.h"
#include"Bug.h"

namespace Seed
{
	typedef map<string, D3DXHANDLE> UniformMap;

	inline void parseShaderProgram(const string &file, const string &entry, map<string, D3DXHANDLE> &uniforms,
		ID3DXConstantTable *ConstTable)
	{
		ifstream in(file.c_str(), ios::in);

		Log << file << endl;
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

				while (islower(buf[len]) || isupper(buf[len]) || buf[len] == '_')
					tmp[len] = buf[len], len++;
				tmp[len] = 0;

				Log << tmp << endl;
				uniforms[tmp] = ConstTable->GetConstantByName(0, tmp);
			}
		}

		in.close();
	}

	class ShaderProgram
	{
	protected:
		ID3DXConstantTable *ConstTable;
		map<string, D3DXHANDLE> uniforms;
		LPDIRECT3DDEVICE9 device;

	public:
		ShaderProgram(){}
		ShaderProgram(LPDIRECT3DDEVICE9 device) :device(device) {}
		inline UniformMap& getUniformMap() { return uniforms; }

		void setValue(D3DXHANDLE h, const matrix4f &m);
		void setValue(D3DXHANDLE h, Texture* tex);
		void setValue(D3DXHANDLE h, const vector3f& v);
		virtual void fireProgram(){}
	};


	class VertexShader : public ShaderProgram
	{
		IDirect3DVertexShader9 *vs;
		
	public:

		VertexShader() { }
		VertexShader(LPDIRECT3DDEVICE9 device, const string &file, const string &entry, const string &version) :ShaderProgram(device) {
			Log << file << ' ' << entry << endl;
			init(file, entry, version);
		}
		virtual ~VertexShader() {
			RELEASE(vs);
		}
		virtual void init(const string &file, const string &entry, const string &version) {
			HRESULT hr;
			ID3DXBuffer *shaderprogram = 0;
			ID3DXBuffer *errorbuffer = 0;

			hr = D3DXCompileShaderFromFile(file.c_str(),
				0, 0, entry.c_str(), version.c_str(), D3DXSHADER_DEBUG,
				&shaderprogram, &errorbuffer, &ConstTable);
			if (FAILED(hr)) {
				throw((char*)errorbuffer->GetBufferPointer());
			}

			// 创建 vertex shader
			hr = device->CreateVertexShader((DWORD*)shaderprogram->GetBufferPointer(), &vs);
			if (FAILED(hr)) {
				MessageBox(0, "create vs fail", 0, 0);
				return;
			}
			RELEASE(shaderprogram);

			ConstTable->SetDefaults(device);


			parseShaderProgram(file, entry, uniforms, ConstTable);
		}

		void  bindVertexProgram()
		{
			device->SetVertexShader(vs);
		}

		virtual void fireProgram() {
			device->SetVertexShader(0);
		}

	};


	// -------------------------- pixel shader---------------------
	class PixelShader : public ShaderProgram
	{
		IDirect3DPixelShader9 *ps;
	public:
		PixelShader() { }
		PixelShader(LPDIRECT3DDEVICE9 device, const string &file, const string &entry, const string &version) :ShaderProgram(device) {
			Log << file << ' ' << entry << endl;
			init(file, entry, version);
		}
		virtual ~PixelShader() {
			RELEASE(ps);
		}
		virtual void init(const string &file, const string &entry, const string &version) {
			HRESULT hr;
			ID3DXBuffer *shader = 0;
			ID3DXBuffer *errorbuffer = 0;

			hr = D3DXCompileShaderFromFile(file.c_str(),
				0, 0, entry.c_str(), version.c_str(), D3DXSHADER_DEBUG,
				&shader, &errorbuffer, &ConstTable);
			if (FAILED(hr)) {
				throw((char*)errorbuffer->GetBufferPointer());
			}
			if (!device) return;
			// 创建 vertex shader
			hr = device->CreatePixelShader((DWORD*)shader->GetBufferPointer(), &ps);
			if (FAILED(hr))
				throw("create ps fail");
			RELEASE(shader);
			ConstTable->SetDefaults(device);


			parseShaderProgram(file, entry, uniforms, ConstTable);
		}
		
		virtual void bindPixelProgram() {
			device->SetPixelShader(ps);
		}

		virtual void fireProgram() {
			device->SetPixelShader(0);
		}

	};



}