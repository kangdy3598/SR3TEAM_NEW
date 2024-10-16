#pragma once

#include "Scene.h"
#include "Map.h"
#include "MapCamera.h"
#include "WorldHeartMap.h"	
#include "MoonForestMap.h"
#include "JungleForestMap.h"
#include "MoonTempleMap.h"
#include "ElectriceelBossMap.h"
#include "ArenaMap.h"
#include "SunTempleMap.h"
#include "Town.h"

class CMapEditor : public Engine::CScene
{

private:
	explicit CMapEditor(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMapEditor();

public:
	virtual   HRESULT   Ready_Scene();
	virtual   _int      Update_Scene(const _float& fTimeDelta);
	virtual   void      LateUpdate_Scene(const _float& fTimeDelta);
	virtual   void      Render_Scene();

private:
	HRESULT Ready_Prototype();
	
	HRESULT Ready_Layer_Environment(const _tchar* pLayerTag);
	HRESULT Ready_Layer_GameLogic(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UI(const _tchar* pLayerTag);


public:
	static CMapEditor* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	
private:
	virtual void Free();

};

