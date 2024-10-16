#pragma once
#include "Scene.h"
#include "DynamicCamera.h"
#include "Player.h"

#include "SunTempleStage.h"
#include "PlayerInteractionBox.h"

#include "DefaultUI.h"
#include "StoreUI.h"
#include "MapName.h"
#include "InvenUI.h"
#include "QuickSlot.h"
#include "Object.h"
#include "JungleForestMap.h"

#include "Stone.h"
#include "StoneHole.h"
#include "Scale.h"
#include "Rock.h"
#include "FrontDoor.h"
#include "Trigger.h"

#include "Dochi.h"
#include "Hochi.h"
#include "CCheerRabbit.h"
#include "BardCat.h"



class CJungleForestStage :public CScene
{
private:
	explicit CJungleForestStage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CJungleForestStage();


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
	static CJungleForestStage* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual  void Free();


};

