#include "pch.h"
#include "Loading.h"
#include "Export_Utility.h"
#include "ResMgr.h"

	

CLoading::CLoading(LPDIRECT3DDEVICE9 pGraphicDev)
	:m_pGraphicDev(pGraphicDev)
	,m_bFinish(false)
{
	m_pGraphicDev->AddRef();
}

CLoading::~CLoading()
{
}

HRESULT CLoading::Ready_Loading(LOADINGID eLoading)
{
	InitializeCriticalSection(&m_Crt);

	m_eID = eLoading;

	//  스레드를 만드는 함수 _beginthreadx 
	m_hThread = (HANDLE)_beginthreadex(NULL, 0, Thread_Main, this, 0, NULL);
	
	return S_OK;
}

_uint CLoading::Loading_Stage()
{
	
	lstrcpy(m_szLoading, L"Etc Loading................");

	//ETC
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Transform", Engine::CTransform::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Calculator", Engine::CCalculator::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_State", Engine::CStateController::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PipeBoard", Engine::CPipeBoardCom::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Pipe", Engine::CPipeCom::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PlantOrbCom", Engine::CPlantOrbCom::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Collider", Engine::CCollider::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Animator", Engine::CAnimator2::Create(m_pGraphicDev)), E_FAIL);	


	lstrcpy(m_szLoading, L"Buffer Loading................");
	//BUFFER 

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TriCol", Engine::CTriCol::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RcCol", Engine::CRcCol::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTex", Engine::CTerrainTex::Create(m_pGraphicDev, VTXCNTX, VTXCNTZ, VTXITV)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubeTex", Engine::CCubeTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TestMapTex", Engine::CStartMap::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MapTex", Engine::CMapTex::Create(m_pGraphicDev, 1000, 1000, 1.f,1.f)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_WorldHeartMapTex", Engine::CMapTex::Create(m_pGraphicDev, 1000, 1000, 2.0f,2.0f)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MoonForestMapTex", Engine::CMapTex::Create(m_pGraphicDev, 1000, 1000, 1.0f,1.0f)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ElectriceelBossMapTex", Engine::CMapTex::Create(m_pGraphicDev, 1000, 1000, 1.0f,1.0f)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_JungleMonkeyTownMapTex", Engine::CMapTex::Create(m_pGraphicDev, 1000, 1000, 1.5f,2.0f)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MoonTempleMapTex", Engine::CMapTex::Create(m_pGraphicDev, 1000, 1000, 2.0f, 2.0f)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ArenaMapTex", Engine::CMapTex::Create(m_pGraphicDev, 1000, 1000, 1.0f, 1.0f)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SunTempleMapTex", Engine::CMapTex::Create(m_pGraphicDev, 1000, 1000, 2.0f, 2.0f)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TownMapTex", Engine::CMapTex::Create(m_pGraphicDev, 1000, 1000, 2.0f, 1.0f)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SwingEffect1", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ogu_Move_Motion/Ogu_SwingEffect1.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SwingEffect2", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ogu_Move_Motion/Ogu_SwingEffect2.png", TEX_NORMAL)), E_FAIL);



	lstrcpy(m_szLoading, L"Texture Loading................");
	//TEXTURE

	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PlayerTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Player.png", TEX_NORMAL)), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PlayerTex2", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Player0.png", TEX_NORMAL)), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Terrain/Terrain0.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SkyBox", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/SkyBox/burger%d.dds", TEX_CUBE, 4)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_StartMap", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Map/Tutorial_Map.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_WorldHeartMap", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Map/WorldHeart.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MoonForestMap", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Map/MoonForestMap.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ElectriceelBossMap", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Map/ElectriceelBossMap.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_JungleMonkeyTownMap", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Map/JungleMonkeyTownMap.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MoonTempleMap", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Map/MoonTempleMap.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ArenaMap", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Map/ArenaMap.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SunTempleMap", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Map/SunTempleMap.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TownMap", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Map/TownMap.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Electric_Bullet", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/ElectriceelBoss/Plant_Bullet_BlueRed.png", TEX_NORMAL)), E_FAIL);



	//NPC
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_FishShop", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/NPC/FishShop.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CottonCandyCart", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/NPC/Sprite_RandomEncounterMerchantObjects2_CottonCandyCart.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_HatMarket", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/NPC/HatMarket.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ConsumMarket", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/NPC/ConsumMarket.png", TEX_NORMAL)), E_FAIL);
	//UI
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_HPFrame", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/HeartHPFrame_UI2.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_HPBar", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/HPBar.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_HPBarBack", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/HPBarBack.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CoinUI", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/Coin_UI.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_KeyBoardUI", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/KeyBoardUI.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_InvenBack", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/InvenBackGround.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_InvenDefault", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/InvenDefault.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_InvenFrame", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/InvenFrame.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_EquipIcon", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/equipment_Icon.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ConsumIcon", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/Consumption_Icon.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_OtherIcon", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/Other_Icon.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_QuestIcon", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/Quest_Icon.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_IconButton", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/IconButton.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_EquipOnOffButton", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/EquipOnOff.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TextBox", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/TextBox.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_InvenCursor", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/UI_Cursor_White.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CountRC", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/CountRC.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_InteractionButton", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/InteractionButton.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_QuestUI", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/Quest_UI.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_QuestCursor", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/QuestCursor.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_QuestAcceptUI", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/QuestAcceptUI.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_StoreUI", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/StoreUI.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PowerUI", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/PowerUI.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SpeedUI", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/SpeedUI.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PickUpButton", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/PickUpButton.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_AcquiredItemUI", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/AcquiredItemUI.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BuyButton", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/BuyButton.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PriceUI", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/PriceUI.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BossHPFrame", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/BossFrame.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BossHPBar", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/BossBar.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BossHPBack", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/BossBack.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ExamineButton", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/ExamineButton.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_FightUI", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/Fight.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_VictoryUI", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/Victory.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MapName", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/MapName.png", TEX_NORMAL)), E_FAIL);
	//Item
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ExploreHat", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/Explore.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PartyHat", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/PartyHat.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Mohican", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/Mohican.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_AppleHat", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/AppleHat.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_EnkyHat", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/EnkyHat.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Penguin", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/Penguin.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PoopHat", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/PoopHat.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CloseButton", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/CloseButton.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_WaterToken", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/WaterToken.png", TEX_NORMAL)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SmallFruit", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/SmallFruit.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MiddleFruit", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/MiddleFruit.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BigFruit", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/BigFruit.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SmallHerb", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/SmallHerb.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MiddleHerb", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/MiddleHerb.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BigHerb", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/BigHerb.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SmallRoot", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/SmallRoot.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MiddleRoot", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/MiddleRoot.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BigRoot", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/BigRoot.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CookSmallFish", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/CookSmallFish.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CookMiddleFish", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/CookMiddleFish.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CookBigFish", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/CookBigFish.png", TEX_NORMAL)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Branch", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/Branch.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Leaf", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/Leaf.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Net", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/Net.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Ticket", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/ExpressTicket.png", TEX_NORMAL)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_HeartFruit", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/Sprite_ItemHeartApple.png", TEX_NORMAL)), E_FAIL);

	//Arena
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_StoneCircle", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/ArenaMonster/StoneCircle.png", TEX_NORMAL)), E_FAIL);

	//Puzzle
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PipeBoardTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/puzzle/Sprite_MoonForest_MazePuzzleFloor2Line.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PipeTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/puzzle/Sprite_MoonForest_MazePushPuzzleThreeWay.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PipeTex2", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/puzzle/Sprite_MoonForest_MazePushPuzzleConer.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PipeCursorTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/puzzle/Sprite_UI_Cursor_Green.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MonkeyStatue", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/puzzle/Sprite_MonkeyStatue_Cave.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_StoneBlock", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/puzzle/Sprite_StoneBlock.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_NoteStatue", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/puzzle/Sprite_NoteStatue.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MusicStatue", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/puzzle/Sprite_MonkeyStatueMusic.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_StonePlaceFrame", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/puzzle/StonePlaceFrame.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_StonePushable", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/puzzle/Sprite_StonePushable.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_StonePedestal", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/puzzle/Sprite_StonePushableElectricityFrame.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CatapultBase", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/puzzle/Sprite_Catapult_Base.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CatapultBucket", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/puzzle/Sprite_Catapult_Bucket.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_LightFlower", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/puzzle/Sprite_MoonForest_LightFlowerRoot.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Shadow", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/puzzle/Shadow.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Catapult_Indicator", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/puzzle/Sprite_AttackTargetSingle.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BreakableStone", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/puzzle/Sprite_StoneBreakable.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Rock", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/puzzle/Rock.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_FrontDoorLeft", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/puzzle/TownFront_Door_Left.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_FrontDoorRight", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/puzzle/TownFront_Door_Right.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_B52", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/puzzle/B_52_Shadow_00.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_FirePit", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/puzzle/Sprite_FirePlace.png", TEX_NORMAL)), E_FAIL);

	//MONSTER
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MonsterTrashBig",
		Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Sprite_MonsterTrashBig.png", TEX_NORMAL)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MonsterTrashFast",
		Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Sprite_MonsterTrashFast.png", TEX_NORMAL)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MonsterTrashSlime",
		Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Sprite_MonsterTrashSlime.png", TEX_NORMAL)), E_FAIL);

	list<const _tchar*> pMothMagePathList;
	pMothMagePathList.push_back(L"../Bin/Resource/Texture/Monster/Sprite_MothMage_Idle.png");
	pMothMagePathList.push_back(L"../Bin/Resource/Texture/Monster/Sprite_MothMage_Attack.png");
	pMothMagePathList.push_back(L"../Bin/Resource/Texture/Monster/Sprite_MothMage_Orb.png");
	pMothMagePathList.push_back(L"../Bin/Resource/Texture/Monster/Sprite_BugBallLiquid.png");


	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MonsterMothMage",
		Engine::CTexture::Create(m_pGraphicDev, pMothMagePathList)), E_FAIL);

	//EFFECT
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Electriceel", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/ElectriceelBoss/Sprite_ThunderVertical.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Attack_Area", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/Sprite_AttackTargetSingle.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Attack_Line", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/Sprite_MoonBossMothFlyLine1.png", TEX_NORMAL)), E_FAIL);	

	CResMgr::GetInstance()->init();
	//TEST 높이 맵 
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainHeightTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Terrain/Height2.bmp", TEX_NORMAL)), E_FAIL);

	Loading_AnimData();


	lstrcpy(m_szLoading, L"Loading Complete!!");

	m_bFinish = true; 

	return 0; 
}

_uint CLoading::Loading_AnimData()
{
	const TCHAR* pGetPath = L"../Bin/Resource/Texture/Ogu_Move_Motion/Animation.dat";
	HANDLE hFile = CreateFile(pGetPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	DWORD	dwByte(0);
	DWORD	dwStrByte(0);

	TCHAR* pObjectName;
	TCHAR* pImgFilePath;
	TCHAR* pImgFileName;

	TCHAR* pAnimationName;

	int imageSizeX, imageSizeY;

	list<const _tchar*> imgPathList;
	vector<_vec2> animFrameCount;
	vector<vector<int>*> animFramePlayList;

	while (true)
	{
		// Player
		ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
		pObjectName = new TCHAR[dwStrByte / 2];
		ReadFile(hFile, pObjectName, dwStrByte, &dwByte, nullptr);

		int animCount = 0;
		ReadFile(hFile, &animCount, sizeof(int), &dwByte, nullptr);

		for (int i = 0; i < animCount; i++)
		{
			// png 주소
			ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
			pImgFileName = new TCHAR[dwStrByte / 2];
			ReadFile(hFile, pImgFileName, dwStrByte, &dwByte, nullptr);

			ReadFile(hFile, &imageSizeX, sizeof(int), &dwByte, nullptr);
			ReadFile(hFile, &imageSizeY, sizeof(int), &dwByte, nullptr);

			// Idle
			ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
			pAnimationName = new TCHAR[dwStrByte / 2];
			ReadFile(hFile, pAnimationName, dwStrByte, &dwByte, nullptr);

			// 가로세로 이미지 개수
			int frameCountX, frameCountY;
			ReadFile(hFile, &frameCountX, sizeof(int), &dwByte, nullptr);
			ReadFile(hFile, &frameCountY, sizeof(int), &dwByte, nullptr);

			// 5방향
			vector<int>* framePlayList = new vector<int>[5];
			for (int j = 0; j < 5; j++)
			{
				int frameCount = 0;
				ReadFile(hFile, &frameCount, sizeof(int), &dwByte, nullptr);

				int readFrame = 0;
				for (int k = 0; k < frameCount; k++)
				{
					ReadFile(hFile, &readFrame, sizeof(int), &dwByte, nullptr);
					framePlayList[j].push_back(readFrame);
				}
			}

			pImgFilePath = new TCHAR[256];
			lstrcpy(pImgFilePath, L"../Bin/Resource/Texture/Ogu_Move_Motion/");
			lstrcat(pImgFilePath, pImgFileName);
			imgPathList.push_back(pImgFilePath);
			animFrameCount.push_back(_vec2(frameCountX, frameCountY));
			animFramePlayList.push_back(framePlayList);
		}

		TCHAR* pProtoName = new TCHAR[128];
		lstrcpy(pProtoName, L"Proto_");
		lstrcat(pProtoName, pObjectName); // Proto_Player

		///////////////////////////////////////////////////////////////////////

		TCHAR* pProtoTextureName = new TCHAR[128];
		lstrcpy(pProtoTextureName, pProtoName);
		lstrcat(pProtoTextureName, L"Texture"); // Proto_PlayerTexture

		FAILED_CHECK_RETURN(Engine::Ready_Proto(pProtoTextureName,
			Engine::CTexture::Create(m_pGraphicDev, imgPathList)), E_FAIL);

		////////////////////////////////////////////////////////////////////////

		TCHAR* pProtoAnimName = new TCHAR[128];
		lstrcpy(pProtoAnimName, pProtoName);
		lstrcat(pProtoAnimName, L"Anim");      // Proto_PlayerAnim

		FAILED_CHECK_RETURN(Engine::Ready_Proto(pProtoAnimName,
			Engine::CAnimation::Create(m_pGraphicDev, animFrameCount, animFramePlayList)), E_FAIL);
		/*if (0 == dwByte)
		{
			delete[]pName;
			break;
		}*/
		break;
	}

	CloseHandle(hFile);
	return 0;
}

unsigned int __stdcall CLoading::Thread_Main(void* pArg)
{
	CLoading* pLoading = reinterpret_cast<CLoading*>(pArg);

	_uint iFlag(0);

	EnterCriticalSection(pLoading->Get_Crt());
	
	switch(pLoading->Get_LoadingID())
	{
	case LOADING_STAGE:
		iFlag = pLoading->Loading_Stage();

		break;

	case LOADING_BOSS:
		break;

	}

	LeaveCriticalSection(pLoading->Get_Crt());

	return iFlag; 

}

CLoading* CLoading::Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eID)
{
	CLoading* pLoading = new CLoading(pGraphicDev);

	if (FAILED(pLoading->Ready_Loading(eID)))
	{
		Safe_Release(pLoading);
		return nullptr;
	}

	return pLoading;
}

void CLoading::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_Crt);

	Safe_Release(m_pGraphicDev);
}
