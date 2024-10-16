#pragma once
#include "Scene.h"
#include "DynamicCamera.h"
#include "Player.h"

#include "DefaultUI.h"
#include "InvenUI.h"
#include "QuestUI.h"
#include "MapName.h"
#include "QuickSlot.h"
#include "Object.h"
#include "Nue.h"
#include "HeartFruit.h"

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
#include "B52Bomber.h"

#include "MoonTempleMap.h"
#include "TownStage.h"

class CMoonTempleStage : public CScene
{
private:
	explicit CMoonTempleStage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMoonTempleStage();


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
	static CMoonTempleStage* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual  void Free();


};

