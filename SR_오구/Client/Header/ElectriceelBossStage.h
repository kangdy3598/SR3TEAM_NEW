
#pragma once

#include "Scene.h"
#include "DynamicCamera.h"
#include "Player.h"
#include "ElectriceelBossMap.h"

#include "DefaultUI.h"
#include "InvenUI.h"
#include "StoreUI.h"
#include "QuickSlot.h"
#include "Object.h"
#include "ElectriceelBoss.h"
#include "ArenaStage.h"
#include "WaterFall.h"	
#include "Stone.h"
#include "PlayerInteractionBox.h"


class CElectriceelBossStage : public CScene
{
private:
	explicit CElectriceelBossStage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CElectriceelBossStage();


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
	static CElectriceelBossStage* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual  void Free();


};

