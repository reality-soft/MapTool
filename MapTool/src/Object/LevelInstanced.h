#pragma once
#include "Engine_include.h"

using namespace KGCA41B;

struct CbInstance
{
	struct Data
	{
		XMMATRIX instance_transform[128];
	}data;

	ComPtr<ID3D11Buffer> buffer;
};

class LevelInstanced
{
public:
	LevelInstanced();
	~LevelInstanced();

public:
	void Init(string mesh_id, string vs_id);
	void Frame();
	void Render();
	void Release();

public:
	void SetTRS(XMVECTOR& t, XMVECTOR r, XMVECTOR s);
	void CreateSurfaceMaterial(const vector<string>& texture_id);
	void CreateInstanceBuffer();
	void AddNewInstance(XMMATRIX& transform);
	void TransformationInstance(UINT index, XMVECTOR& transform);
public:
	shared_ptr<StaticMesh> static_mesh;
	shared_ptr<VertexShader> vs;
	UINT instance_count = 0;
	map<UINT, XMMATRIX> instancing_map;

private:
	XMMATRIX world_transform;
	CbInstance instancing;
};

