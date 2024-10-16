#pragma once

#include "Scene.h"
#include "StartBackGround.h"

class CLogo;
class CMapEditor;

class CStartScene : public Engine::CScene
{

private:
	explicit CStartScene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStartScene();

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
	static CStartScene* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();

	CLogo* m_pLogo;
	CMapEditor* m_pMapEditor;
	

};

