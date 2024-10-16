#pragma once

#include "GraphicDev.h"
#include "Engine_Define.h"
#include "Export_System.h"
#include "Export_Utility.h"


#include  "Object.h"
#include  "Layer.h"

#include "Imgui_ObjectTool.h"

class CImGuiManger
{
	DECLARE_SINGLETON(CImGuiManger)

private:
	explicit CImGuiManger();
	virtual ~CImGuiManger();

public:
	void init();
	void update();
	void Render(); 
	void Release();


public:
	void ShowTileMenuWindow();
	void CreateTerrain(const _ulong& dwCntX,
					   const _ulong& dwCntZ,
					   const _ulong& dwVtxItv) {};
	
public:

	bool				GetTerrainOnOFF() { return m_bTerrainOnOff; }
	bool				GetWireFrameOnOFF() { return m_bWireFrameModeOnOff; }
	
	bool				GetImageButtonClick() { return m_bImageButtonClick; }
	IDirect3DTexture9*  GetCurrentTerrainTexture() { return m_pCurTerrainTexture;}
	IDirect3DTexture9*  GetCurrentObjectTexture() { return m_pCurTerrainTexture; }



	//신가져오고 레이어는 안가져와도됨 
	

private:
	
	_vec3				PickingOnTerrain();
	void				OnTileImageButtonClick(int tileIndex);

private:
	// Our state
	bool		testbool;
	int			inttest;
	float		floattest;

	int         iTileX;
	int			iTileY;
	float       fVtxItv; // 점과 점사이의 간격 

	CMapTex* m_pMapTex;

private:
	bool LoadTextureFromFile(LPDIRECT3DDEVICE9 d3dDevice, const char* filePath, IDirect3DTexture9** outTexture)
	{
		HRESULT hr = D3DXCreateTextureFromFileA(d3dDevice, filePath, outTexture);
		return SUCCEEDED(hr);
	}





	LPDIRECT3DDEVICE9 m_pGraphicDev;	

	IDirect3DTexture9* m_pTerrainTexture;
	IDirect3DTexture9* m_pCurTerrainTexture;
	IDirect3DTexture9* m_pCurObjectTexture;

	vector<IDirect3DTexture9*> m_vecTerrainTexture;
	

	bool m_bshowTileTextureWindow;
	bool m_bshowObjectTextrueWindow;
	bool m_bshowInstalledObjectList;
	bool m_bTerrainOnOff; 
	bool m_bWireFrameModeOnOff;
	bool m_bImageButtonClick;
	

	_vec3       m_vecPickPos;




	
};

