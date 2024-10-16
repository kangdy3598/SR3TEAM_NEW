#pragma once
#include "Scene.h"
#include "DynamicCamera.h"
#include "Player.h"
#include "MoonForestMap.h"

#include "DefaultUI.h"
#include "StoreUI.h"
#include "InvenUI.h"
#include "QuickSlot.h"
#include "Object.h"
#include "DefaultUI.h"
#include "MapName.h"
#include "InvenUI.h"
#include "QuickSlot.h"
#include "Object.h"

#include "Pipe.h"
#include "PipeBoard.h"
#include "Crystal.h"
#include "PressBlock.h"
#include "CrystalPuzzle.h"
#include "BoardCursor.h"
#include "MonkeyStatue.h"
#include "StoneBlock.h"
#include "BugStatue.h"
#include "BugStatuePuzzle.h"
#include "CombinationPuzzle.h"
#include "CombinationStatue.h"
#include "MusicStatue.h"
#include "NoteStatue.h"
#include "FirePit.h"
#include "Scale.h"
#include "WeightComparator.h"
#include "Stone.h"
#include "StonePedestal.h"
#include "Catapult.h"
#include "LightFlower.h"
#include "FlowerGlow.h"
#include "ColorStone.h"
#include "ShootingPlant.h"
#include "StoneHole.h"
#include "BreakableStone.h"
#include "Rock.h"
#include "FenceThorn.h"
#include "FrontDoor.h"
#include "Trigger.h"
#include "ColorBugStatue.h"
#include "MatchPuzzle.h"
#include "B52Bomber.h"

#include "ElectriceelBossStage.h"
#include "MoonTempleStage.h"

class CMoonForestStage : public CScene
{

private:
	explicit CMoonForestStage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMoonForestStage();


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
	static CMoonForestStage* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual  void Free();


};

