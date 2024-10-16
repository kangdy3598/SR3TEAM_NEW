#pragma once
#include "Scene.h"
#include "DynamicCamera.h"
#include "Player.h"
#include "WorldHeartMap.h"
#include "JungleForestStage.h"

#include "DefaultUI.h"
#include "InvenUI.h"
#include "MapName.h"
#include "StoreUI.h"
#include "QuickSlot.h"
#include "Object.h"

#include "B52Bomber.h"

#include "WhiteBird.h"
#include "God.h"
#include "CGodEffect.h"
#include "CMouse.h"


//여기다 넣을 오브젝트들 다 include하기

class CWorldHearStage : public CScene
{

private:
	explicit CWorldHearStage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CWorldHearStage();


public:
	virtual   HRESULT   Ready_Scene();
	virtual   void		LateReady_Scene();
	virtual   _int		Update_Scene(const _float& fTimeDelta);
	virtual   void		LateUpdate_Scene(const _float& fTimeDelta);
	virtual   void      Render_Scene();
	virtual   void		init();

private:
	HRESULT   Ready_LightInfo();
	HRESULT   Ready_Layer_Environmnet(const _tchar* pLayerTag);
	HRESULT   Ready_Layer_GameLogic(const _tchar* pLayerTag);
	HRESULT   Ready_Layer_UI(const _tchar* pLayerTag);


public:
	static CWorldHearStage* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual  void Free();
};

