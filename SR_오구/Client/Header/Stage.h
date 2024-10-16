#pragma once

#include "Scene.h"
#include "BackGround.h"

#include "Player.h"
#include "PlayerInteractionBox.h"
#include "SwingEffect.h"
#include "Monster.h"
#include "DefaultUI.h"
#include "InvenUI.h"
#include "QuickSlot.h"
#include "HeartFruit.h"
#include "ExpressMonkey.h"
#include "SeaLion.h"
#include "Sheep.h"
#include "Pig.h"
#include "Cow.h"


#include "B52Bomber.h"

#include "Terrain.h"
#include "DynamicCamera.h"

#include "SkyBox.h"
#include "TestMap.h"
#include "Map.h"
#include "Object.h"
#include "TestCol.h"
#include "TestCol2.h"
#include "MonsterSlimeFast.h"
#include "ArenaStage.h"
#include "MonsterMothMage.h"

//9월 27일 애니메이션 테스트
#include "TestObject.h"
#include "Grass.h"
#include "Grass2.h"

//민지 임시
#include "AddHP.h"
#include "MapName.h"


class CStage : public Engine::CScene
{
private:
	explicit CStage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStage();


public:
	virtual   HRESULT   Ready_Scene();
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
	static CStage* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual  void Free();

};

