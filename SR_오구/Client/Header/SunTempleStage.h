#pragma once
#include "Scene.h"
#include "DynamicCamera.h"
#include "Player.h"
#include "MoonForestStage.h"
#include "PlayerInteractionBox.h"
#include "SunTempleMap.h"

#include "Crystal.h"
#include "PressBlock.h"
#include "CrystalPuzzle.h"
#include "MonkeyStatue.h"
#include "StoneBlock.h"
#include "BugStatue.h"
#include "BugStatuePuzzle.h"
#include "CombinationPuzzle.h"
#include "CombinationStatue.h"
#include "Scale.h"
#include "WeightComparator.h"
#include "Stone.h"
#include "StonePedestal.h"
#include "Catapult.h"
#include "StoneHole.h"
#include "BreakableStone.h"
#include "Rock.h"
#include "B52Bomber.h"

#include "DefaultUI.h"
#include "StoreUI.h"
#include "InvenUI.h"
#include "MapName.h"
#include "QuickSlot.h"
#include "Object.h"
#include "ExpressMonkey.h"

class CSunTempleStage : public CScene
{
private:
	explicit CSunTempleStage(LPDIRECT3DDEVICE9 pGraphicDev);	
	virtual ~CSunTempleStage();	


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
	static CSunTempleStage* Create(LPDIRECT3DDEVICE9 pGraphicDev);	

private:
	virtual  void Free();	

};

