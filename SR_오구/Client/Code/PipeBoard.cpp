#include "pch.h"
#include "PipeBoard.h"
#include "Pipe.h"
#include "BoardCursor.h"
#include "StoneBlock.h"
#include "Nue.h"
#include "Player.h"

#include "Export_Utility.h"

CPipeBoard::CPipeBoard(LPDIRECT3DDEVICE9 pGraphicDev)
    : Engine::CPuzzleObject(pGraphicDev), m_bIsInteracting(false), m_pIntButton(nullptr), m_pExamineButton(nullptr), m_pCloseButton(nullptr)
{
}

CPipeBoard::~CPipeBoard()
{
}

HRESULT CPipeBoard::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->Set_Pos(1000.f, 20.1f, 875.f);
	m_pTransformCom->m_vScale = { 120.f, 20.f, 120.f };
	SetObjectType(NOTPASS_ABLE);

	m_pTexTransformCom->m_vScale = { 98.f, 98.f, 0.f };
	m_pTexTransformCom->Set_Pos(1000.f, 0.2f, 875.f);
	m_pTexTransformCom->Rotation(ROT_X, 90.f * 3.14f / 180.f);

	m_pPipeBoardCom->m_iCurPipeCount = 0;
	m_pPipeBoardCom->m_bIsPipeSpawned = false;
	m_pPipeBoardCom->m_iGridSizeX = 7;
	m_pPipeBoardCom->m_iGridSizeZ = 7;
	m_pPipeBoardCom->m_vecBoardGrid.resize(49);
	
    return S_OK;
}

void CPipeBoard::LateReady_GameObject()
{
	if (m_pExamineButton == nullptr)
	{
		m_pExamineButton = CExamineButton::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(m_pExamineButton);
	}
	if (m_pCloseButton == nullptr)
	{
		m_pCloseButton = CCloseButton::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(m_pCloseButton);
	}
	if (m_pIntButton == nullptr)
	{
		m_pIntButton = CInteractionButton::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(m_pIntButton);
	}

}

_int CPipeBoard::Update_GameObject(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);
	Key_Input(fTimeDelta);

	if (m_pPipeBoardCom->m_bIsPipeSpawned == false && m_pPipeBoardCom->m_iCurPipeCount < 9) {
		map<const _tchar*, CLayer*> pMapLayer = Engine::Get_CurScenePtr()->GetLayerMapPtr();
		NULL_CHECK_RETURN(pMapLayer[L"Layer_GameLogic"], E_FAIL);
		Engine::CGameObject* pGameObject = nullptr;
		_vec3 vBasePos, vPos, vRot;
		m_pTransformCom->Get_Info(INFO_POS, &vBasePos);
		vBasePos = { 1000.f, 0.2f, 875.f };

		pGameObject = CBoardCursor::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pMapLayer[L"Layer_GameLogic"]->Add_GameObject(L"PipeCursor", pGameObject);
		m_pPipeBoardCom->m_pCursor = pGameObject;
		m_pPipeBoardCom->m_iCursorID = 25;
		
		pGameObject = CPipe::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pMapLayer[L"Layer_GameLogic"]->Add_GameObject(L"Pipe0", pGameObject);
		vPos = { vBasePos.x - 49.f, vBasePos.y + 0.2f, vBasePos.z + 49.f };
		vRot = { 0.f, 0.f, -180.f };
		static_cast<CPipe*>(pGameObject)->Initialize_Pipe_Option(FLOW_DOWN, FLOW_END, FLOW_END, FLOW_END, true, 2, vPos, vRot);
		static_cast<CPipe*>(pGameObject)->Set_PipeID(0);
		m_pPipeBoardCom->m_vecBoardGrid[8] = pGameObject;

		pGameObject = CPipe::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pMapLayer[L"Layer_GameLogic"]->Add_GameObject(L"Pipe1", pGameObject);
		vPos = { vBasePos.x, vBasePos.y + 0.2f, vBasePos.z + 49.f };
		vRot = { 0.f, 90.f, 0.f };
		dynamic_cast<CPipe*>(pGameObject)->Initialize_Pipe_Option(FLOW_DOWN, FLOW_END, FLOW_END, FLOW_END, true, 1, vPos, vRot);
		static_cast<CPipe*>(pGameObject)->Set_PipeID(1);
		m_pPipeBoardCom->m_vecBoardGrid[10] = pGameObject;

		pGameObject = CPipe::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pMapLayer[L"Layer_GameLogic"]->Add_GameObject(L"Pipe2", pGameObject);
		vPos = { vBasePos.x + 49.f, vBasePos.y + 0.2f, vBasePos.z + 49.f };
		vRot = { 0.f, 0.f, 0.f };
		dynamic_cast<CPipe*>(pGameObject)->Initialize_Pipe_Option(FLOW_DOWN, FLOW_END, FLOW_END, FLOW_END, true, 2, vPos, vRot);
		static_cast<CPipe*>(pGameObject)->Set_PipeID(2);
		m_pPipeBoardCom->m_vecBoardGrid[12] = pGameObject;

		pGameObject = CPipe::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pMapLayer[L"Layer_GameLogic"]->Add_GameObject(L"Pipe3", pGameObject);
		vPos = { vBasePos.x, vBasePos.y + 0.2f, vBasePos.z - 24.5f};
		vRot = { 0.f, 90.f, 0.f };
		dynamic_cast<CPipe*>(pGameObject)->Initialize_Pipe_Option(FLOW_LEFT, FLOW_UP, FLOW_RIGHT, FLOW_END, true, 0, vPos, vRot);		
		dynamic_cast<CPipeCom*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Pipe"))->m_bIsConnected = true;
		m_pPipeBoardCom->m_pStatingPipe = pGameObject;
		m_pPipeBoardCom->m_vecBoardGrid[31] = pGameObject;

		pGameObject = CPipe::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pMapLayer[L"Layer_GameLogic"]->Add_GameObject(L"Pipe4", pGameObject);
		vPos = { vBasePos.x - 49.f, vBasePos.y + 0.2f, vBasePos.z - 49.f };
		vRot = { 0.f, 180.f, 0.f };
		dynamic_cast<CPipe*>(pGameObject)->Initialize_Pipe_Option(FLOW_UP, FLOW_END, FLOW_END, FLOW_END, true, 2, vPos, vRot);
		static_cast<CPipe*>(pGameObject)->Set_PipeID(3);
		m_pPipeBoardCom->m_vecBoardGrid[36] = pGameObject;

		pGameObject = CPipe::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pMapLayer[L"Layer_GameLogic"]->Add_GameObject(L"Pipe5", pGameObject);
		vPos = { vBasePos.x + 49.f, vBasePos.y + 0.2f, vBasePos.z - 49.f };
		vRot = { 0.f, 180.f,  -180.f };
		dynamic_cast<CPipe*>(pGameObject)->Initialize_Pipe_Option(FLOW_UP, FLOW_END, FLOW_END, FLOW_END, true, 2, vPos, vRot);
		static_cast<CPipe*>(pGameObject)->Set_PipeID(4);
		m_pPipeBoardCom->m_vecBoardGrid[40] = pGameObject;

		// Movable Blocks
		pGameObject = CPipe::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pMapLayer[L"Layer_GameLogic"]->Add_GameObject(L"Pipe6", pGameObject);
		vPos = { vBasePos.x - 49.f, vBasePos.y + 0.2f, vBasePos.z - 24.5f };
		vRot = { 0.f, 90.f, 0.f };
		dynamic_cast<CPipe*>(pGameObject)->Initialize_Pipe_Option(FLOW_LEFT, FLOW_RIGHT, FLOW_UP, FLOW_END, false, 3, vPos, vRot);
		m_pPipeBoardCom->m_vecBoardGrid[29] = pGameObject;

		pGameObject = CPipe::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pMapLayer[L"Layer_GameLogic"]->Add_GameObject(L"Pipe7", pGameObject);
		vPos = { vBasePos.x + 49.5f, vBasePos.y + 0.2f, vBasePos.z - 24.5f };
		vRot = { 0.f, 0.f, 0.f };
		dynamic_cast<CPipe*>(pGameObject)->Initialize_Pipe_Option(FLOW_LEFT, FLOW_DOWN, FLOW_END, FLOW_END, false, 5, vPos, vRot);
		m_pPipeBoardCom->m_vecBoardGrid[33] = pGameObject;

		pGameObject = CPipe::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pMapLayer[L"Layer_GameLogic"]->Add_GameObject(L"Pipe8", pGameObject);
		vPos = { vBasePos.x - 24.5f, vBasePos.y + 0.2f, vBasePos.z };
		vRot = { 0.f, -90.f, 0.f };
		dynamic_cast<CPipe*>(pGameObject)->Initialize_Pipe_Option(FLOW_RIGHT, FLOW_DOWN, FLOW_END, FLOW_END, false, 5, vPos, vRot);
		m_pPipeBoardCom->m_vecBoardGrid[23] = pGameObject;

		m_pPipeBoardCom->m_iCurPipeCount = 8;
		m_pPipeBoardCom->m_bIsPipeSpawned = true;
	}
	else if (m_pPipeBoardCom->m_bIsPipeSpawned == false && m_pPipeBoardCom->m_iCurPipeCount < 10) {
		map<const _tchar*, CLayer*> pMapLayer = Engine::Get_CurScenePtr()->GetLayerMapPtr();
		NULL_CHECK_RETURN(pMapLayer[L"Layer_GameLogic"], E_FAIL);
		Engine::CGameObject* pGameObject = nullptr;
		_vec3 vBasePos, vPos, vRot;
		vBasePos = { 1000.f, 0.2f, 875.f };		

		pGameObject = CPipe::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pMapLayer[L"Layer_GameLogic"]->Add_GameObject(L"Pipe9", pGameObject);
		vPos = { vBasePos.x - 73.5f, 0.4f, vBasePos.z - 73.5f };
		vRot = { 0.f, 0.f, 0.f };
		dynamic_cast<CPipe*>(pGameObject)->Initialize_Pipe_Option(FLOW_LEFT, FLOW_RIGHT, FLOW_END, FLOW_END, false, 4, vPos, vRot);
		m_pPipeBoardCom->m_vecBoardGrid[42] = pGameObject;

		m_pPipeBoardCom->m_bIsPipeSpawned = true;
	}
	else if (m_pPipeBoardCom->m_bIsPipeSpawned == false && m_pPipeBoardCom->m_iCurPipeCount < 11) {
		map<const _tchar*, CLayer*> pMapLayer = Engine::Get_CurScenePtr()->GetLayerMapPtr();
		NULL_CHECK_RETURN(pMapLayer[L"Layer_GameLogic"], E_FAIL);
		Engine::CGameObject* pGameObject = nullptr;
		_vec3 vBasePos, vPos, vRot;
		vBasePos = { 1000.f, 0.2f, 875.f };

		pGameObject = CPipe::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pMapLayer[L"Layer_GameLogic"]->Add_GameObject(L"Pipe10", pGameObject);
		vPos = { vBasePos.x + 73.5f, 0.4f, vBasePos.z };
		vRot = { 0.f, 90.f, 0.f };
		dynamic_cast<CPipe*>(pGameObject)->Initialize_Pipe_Option(FLOW_UP, FLOW_DOWN, FLOW_END, FLOW_END, false, 4, vPos, vRot);
		m_pPipeBoardCom->m_vecBoardGrid[27] = pGameObject;
		m_pPipeBoardCom->m_bIsPipeSpawned = true;
	}
	else if (m_pPipeBoardCom->m_bIsPipeSpawned == false && m_pPipeBoardCom->m_iCurPipeCount < 12) {
		map<const _tchar*, CLayer*> pMapLayer = Engine::Get_CurScenePtr()->GetLayerMapPtr();
		NULL_CHECK_RETURN(pMapLayer[L"Layer_GameLogic"], E_FAIL);
		Engine::CGameObject* pGameObject = nullptr;
		_vec3 vBasePos, vPos, vRot;
		vBasePos = { 1000.f, 0.2f, 875.f };

		pGameObject = CPipe::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pMapLayer[L"Layer_GameLogic"]->Add_GameObject(L"Pipe11", pGameObject);
		vPos = { vBasePos.x - 73.5f, 0.4f, vBasePos.z + 49.f };
		vRot = { 0.f, 90.f, 0.f };
		dynamic_cast<CPipe*>(pGameObject)->Initialize_Pipe_Option(FLOW_LEFT, FLOW_UP, FLOW_END, FLOW_END, false, 5, vPos, vRot);
		m_pPipeBoardCom->m_vecBoardGrid[7] = pGameObject;
		m_pPipeBoardCom->m_bIsPipeSpawned = true;
	}
	
	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	m_pCloseButton->Update_GameObject(fTimeDelta);
	m_pExamineButton->Update_GameObject(fTimeDelta);
	m_pIntButton->Update_GameObject(fTimeDelta);
	return iExit;
}

void CPipeBoard::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CPipeBoard::Render_GameObject()	
{	 
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTexTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pTextureCom->Set_Texture();	
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pBoundBox->Ready_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

void CPipeBoard::OnCollision(CGameObject* _pOther)
{
	if (_pOther->Get_Tag() != TAG_PLAYER)
		return;	

	if (!m_bIsInteracting) {
		//m_pIntButton->CallButton(true);
		m_pExamineButton->CallButton(true);
		m_pCloseButton->CallButton(false);
	}
	else {
		//m_pIntButton->CallButton(true);
		m_pExamineButton->CallButton(false);
		m_pCloseButton->CallButton(true);

	}
	if (Engine::GetKeyDown(DIK_S))
	{
		if (!m_bIsInteracting) {			
			On_Interacted();
			static_cast<CPlayer*>(_pOther)->Set_Interacting(true);
		}
	}

	if (Engine::GetKeyDown(DIK_D))
	{
		if (m_bIsInteracting) {			
			On_Exit();
			static_cast<CPlayer*>(_pOther)->Set_Interacting(false);
		}
	}
}

void CPipeBoard::OnCollisionEnter(CGameObject* _pOther)
{
	
}

void CPipeBoard::OnCollisionExit(CGameObject* _pOther)
{	
	m_pExamineButton->CallButton(false);
	m_pCloseButton->CallButton(false);
}

HRESULT CPipeBoard::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_PipeBoardTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

	pComponent = m_pTexTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_TexTransform", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	pComponent = m_pPipeBoardCom = dynamic_cast<CPipeBoardCom*>(Engine::Clone_Proto(L"Proto_PipeBoard"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_PipeBoard", pComponent });

	pComponent = m_pBoundBox = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pBoundBox->SetGameObjectPtr(this);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", pComponent });

	return S_OK;
}

void CPipeBoard::On_Interacted()
{
	_vec3 vBasePos;
	vBasePos = { 1000.f, 0.2f, 875.f };
	
	static_cast<Engine::CTransform*>(m_pPipeBoardCom->m_pCursor->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(vBasePos.x, vBasePos.y + 0.5f, vBasePos.z);
	static_cast<CBoardCursor*>(m_pPipeBoardCom->m_pCursor)->Set_Active(true);
	m_pPipeBoardCom->m_iCursorID = 25;
	m_bIsInteracting = true;
}

void CPipeBoard::On_Exit()
{
	static_cast<CBoardCursor*>(m_pPipeBoardCom->m_pCursor)->Set_Active(false);	
	m_bIsInteracting = false;
}

void CPipeBoard::Reset_Connected()
{
	for (auto& iter : m_pPipeBoardCom->m_vecBoardGrid) {
		if (iter == nullptr || iter == m_pPipeBoardCom->m_pStatingPipe)
			continue;
		static_cast<CPipeCom*>(iter->Get_Component(ID_DYNAMIC, L"Com_Pipe"))->m_bIsConnected = false;
	}	
	
	if (m_pPipeBoardCom->m_pPickedPipe != nullptr)
		static_cast<CPipeCom*>(m_pPipeBoardCom->m_pPickedPipe->Get_Component(ID_DYNAMIC, L"Com_Pipe"))->m_bIsConnected = false;	
}

void CPipeBoard::Check_Connected(CGameObject* _pSender, _int _eID)
{	 
	if (_eID % 7 == 0 || _eID % 7 == 6)
		return;

	_int iGridX = m_pPipeBoardCom->m_iGridSizeX;
	_int iGridZ = m_pPipeBoardCom->m_iGridSizeZ;

	CPipeCom* obj = static_cast<CPipeCom*>(m_pPipeBoardCom->m_vecBoardGrid[_eID]->Get_Component(ID_DYNAMIC, L"Com_Pipe"));

	if (_pSender != m_pPipeBoardCom->m_pStatingPipe && obj->m_bIsFixed == true) {
		obj->m_bIsConnected = true;
		On_Connected(static_cast<CPipe*>(m_pPipeBoardCom->m_vecBoardGrid[_eID])->Get_PipeID());
		Play_Sound(L"SFX_98_MazePuzzle_LineOn.wav", SOUND_PUZZLE_SUB, 1.f);
		return;
	}
	
	for (auto& iter : obj->m_FlowList)
	{
		switch (iter)
		{
		case FLOW_UP:
			if (_eID - iGridX >= 0 && m_pPipeBoardCom->m_vecBoardGrid[_eID - iGridX] != nullptr	&& m_pPipeBoardCom->m_vecBoardGrid[_eID - iGridX] != _pSender) {
				if(static_cast<CPipeCom*>(m_pPipeBoardCom->m_vecBoardGrid[_eID - iGridX]->Get_Component(ID_DYNAMIC, L"Com_Pipe"))->Is_PipeType_Exist(FLOW_DOWN))
					Check_Connected(m_pPipeBoardCom->m_vecBoardGrid[_eID], _eID - iGridX);
			}
			break;
		case FLOW_DOWN:
			if (_eID + iGridX <= iGridX * iGridZ &&	m_pPipeBoardCom->m_vecBoardGrid[_eID + iGridX] != nullptr && m_pPipeBoardCom->m_vecBoardGrid[_eID + iGridX] != _pSender) {
				if (static_cast<CPipeCom*>(m_pPipeBoardCom->m_vecBoardGrid[_eID + iGridX]->Get_Component(ID_DYNAMIC, L"Com_Pipe"))->Is_PipeType_Exist(FLOW_UP))
					Check_Connected(m_pPipeBoardCom->m_vecBoardGrid[_eID], _eID + iGridX);
			}
			break;
		case FLOW_LEFT:
			if (_eID - 1 >= 0 && m_pPipeBoardCom->m_vecBoardGrid[_eID - 1] != nullptr && m_pPipeBoardCom->m_vecBoardGrid[_eID - 1] != _pSender ) {
				if (static_cast<CPipeCom*>(m_pPipeBoardCom->m_vecBoardGrid[_eID - 1]->Get_Component(ID_DYNAMIC, L"Com_Pipe"))->Is_PipeType_Exist(FLOW_RIGHT))
					Check_Connected(m_pPipeBoardCom->m_vecBoardGrid[_eID], _eID - 1);
			}
			break;
		case FLOW_RIGHT:
			if (_eID + 1 <= iGridX * iGridZ && m_pPipeBoardCom->m_vecBoardGrid[_eID + 1] != nullptr	&& m_pPipeBoardCom->m_vecBoardGrid[_eID + 1] != _pSender) {
				if (static_cast<CPipeCom*>(m_pPipeBoardCom->m_vecBoardGrid[_eID + 1]->Get_Component(ID_DYNAMIC, L"Com_Pipe"))->Is_PipeType_Exist(FLOW_LEFT))
					Check_Connected(m_pPipeBoardCom->m_vecBoardGrid[_eID], _eID + 1);
			}
			break;
		case FLOW_END:
			break;
		default:
			break;
		}
	}

	obj->m_bIsConnected = true;
}

void CPipeBoard::On_Connected(_int _iPipeID)
{
	switch (_iPipeID)
	{
	case 0:
		for (int i = 0; i < m_vecStoneBlocks[0].size(); i++)		
			static_cast<CStoneBlock*>(m_vecStoneBlocks[0][i])->Move_StoneBlockOnce();
		break;
	case 1:
		for (int i = 0; i < m_vecStoneBlocks[1].size(); i++)
			static_cast<CStoneBlock*>(m_vecStoneBlocks[1][i])->Move_StoneBlockOnce();
		static_cast<CNue*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Nue"))->Set_QuestClear();
		break;
	case 2:
		for (int i = 0; i < m_vecStoneBlocks[2].size(); i++)
			static_cast<CStoneBlock*>(m_vecStoneBlocks[2][i])->Move_StoneBlockOnce();
		break;
	case 3:
		for (int i = 0; i < m_vecStoneBlocks[3].size(); i++)
			static_cast<CStoneBlock*>(m_vecStoneBlocks[3][i])->Move_StoneBlockOnce();		
		break;
	case 4:
		for (int i = 0; i < m_vecStoneBlocks[4].size(); i++)
			static_cast<CStoneBlock*>(m_vecStoneBlocks[4][i])->Move_StoneBlockOnce();
		break;
	default:
		break;
	}
}

void CPipeBoard::Key_Input(const _float& fTimeDelta)
{
	if (!m_bIsInteracting)
		return;	

	if (Engine::GetKeyUp(DIK_UP))
	{
		if (m_pPipeBoardCom->m_iCursorID - m_pPipeBoardCom->m_iGridSizeZ <= 0)
			return;

		_vec3 vBasePos;
		static_cast<Engine::CTransform*>(m_pPipeBoardCom->m_pCursor->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Get_Info(INFO_POS, &vBasePos);
		static_cast<Engine::CTransform*>(m_pPipeBoardCom->m_pCursor->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(vBasePos.x, 0.61f, vBasePos.z + 24.5f);
		m_pPipeBoardCom->m_iCursorID -= m_pPipeBoardCom->m_iGridSizeZ;
		Play_Sound(L"SFX_546_JigsawPuzzlePiece_Move.wav", SOUND_PUZZLE_SUB, 1.f);

		if (m_pPipeBoardCom->m_pPickedPipe != nullptr)
			static_cast<Engine::CTransform*>(m_pPipeBoardCom->m_pPickedPipe->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(vBasePos.x, 0.6f, vBasePos.z + 24.5f);

	}

	if (Engine::GetKeyUp(DIK_DOWN))
	{	
		if (m_pPipeBoardCom->m_iCursorID + m_pPipeBoardCom->m_iGridSizeZ > (m_pPipeBoardCom->m_iGridSizeX * m_pPipeBoardCom->m_iGridSizeZ))
			return;

		_vec3 vBasePos;
		static_cast<Engine::CTransform*>(m_pPipeBoardCom->m_pCursor->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Get_Info(INFO_POS, &vBasePos);
		static_cast<Engine::CTransform*>(m_pPipeBoardCom->m_pCursor->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(vBasePos.x, 0.61f, vBasePos.z - 24.5f);
		m_pPipeBoardCom->m_iCursorID += m_pPipeBoardCom->m_iGridSizeZ;
		Play_Sound(L"SFX_546_JigsawPuzzlePiece_Move.wav", SOUND_PUZZLE_SUB, 1.f);

		if (m_pPipeBoardCom->m_pPickedPipe != nullptr)
			static_cast<Engine::CTransform*>(m_pPipeBoardCom->m_pPickedPipe->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(vBasePos.x, 0.6f, vBasePos.z - 24.5f);
	}

	if (Engine::GetKeyUp(DIK_LEFT))
	{
		if ((m_pPipeBoardCom->m_iCursorID % m_pPipeBoardCom->m_iGridSizeX) == 1)
			return;

		_vec3 vBasePos;
		static_cast<Engine::CTransform*>(m_pPipeBoardCom->m_pCursor->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Get_Info(INFO_POS, &vBasePos);
		static_cast<Engine::CTransform*>(m_pPipeBoardCom->m_pCursor->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(vBasePos.x - 24.5f, 0.61f, vBasePos.z);
		m_pPipeBoardCom->m_iCursorID--;
		Play_Sound(L"SFX_546_JigsawPuzzlePiece_Move.wav", SOUND_PUZZLE_SUB, 1.f);

		if (m_pPipeBoardCom->m_pPickedPipe != nullptr) {			
			static_cast<Engine::CTransform*>(m_pPipeBoardCom->m_pPickedPipe->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(vBasePos.x - 24.5f, 0.6f, vBasePos.z);
		}		
	}

	if (Engine::GetKeyUp(DIK_RIGHT))
	{
		if ((m_pPipeBoardCom->m_iCursorID % m_pPipeBoardCom->m_iGridSizeX) == 0)
			return;

		_vec3 vBasePos;
		static_cast<Engine::CTransform*>(m_pPipeBoardCom->m_pCursor->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Get_Info(INFO_POS, &vBasePos);
		static_cast<Engine::CTransform*>(m_pPipeBoardCom->m_pCursor->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(vBasePos.x + 24.5f, 0.51f, vBasePos.z);
		m_pPipeBoardCom->m_iCursorID++;
		Play_Sound(L"SFX_546_JigsawPuzzlePiece_Move.wav", SOUND_PUZZLE_SUB, 1.f);

		if (m_pPipeBoardCom->m_pPickedPipe != nullptr)
			static_cast<Engine::CTransform*>(m_pPipeBoardCom->m_pPickedPipe->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(vBasePos.x + 24.5f, 0.6f, vBasePos.z);
	}

	if (Engine::GetKeyUp(DIK_S))
	{
		if (m_pPipeBoardCom->m_vecBoardGrid[m_pPipeBoardCom->m_iCursorID - 1] != nullptr && m_pPipeBoardCom->m_pPickedPipe == nullptr &&
			static_cast<Engine::CPipeCom*>(m_pPipeBoardCom->m_vecBoardGrid[m_pPipeBoardCom->m_iCursorID - 1]->Get_Component(ID_DYNAMIC, L"Com_Pipe"))->m_bIsFixed == false) {
			m_pPipeBoardCom->m_pPickedPipe = m_pPipeBoardCom->m_vecBoardGrid[m_pPipeBoardCom->m_iCursorID - 1];
			m_pPipeBoardCom->m_vecBoardGrid[m_pPipeBoardCom->m_iCursorID - 1] = nullptr;

			Play_Sound(L"SFX_545_JigsawPuzzlePiece_PickUp.wav", SOUND_PUZZLE_SUB, 1.f);

			Reset_Connected();
			Check_Connected(m_pPipeBoardCom->m_pStatingPipe, 31);
		}
		else {
			if (m_pPipeBoardCom->m_pPickedPipe == nullptr || m_pPipeBoardCom->m_vecBoardGrid[m_pPipeBoardCom->m_iCursorID - 1] != nullptr)
				return;			

			_vec3 vPos;			
			m_pPipeBoardCom->m_vecBoardGrid[m_pPipeBoardCom->m_iCursorID - 1] = m_pPipeBoardCom->m_pPickedPipe;
			CTransform* tr = static_cast<Engine::CTransform*>(m_pPipeBoardCom->m_pPickedPipe->Get_Component(ID_DYNAMIC, L"Com_Transform"));
			tr->Get_Info(INFO_POS, &vPos);
			tr->Set_Pos(vPos.x, 0.5f, vPos.z);
			m_pPipeBoardCom->m_pPickedPipe = nullptr;			

			Play_Sound(L"SFX_547_JigsawPuzzlePiece_PickDown.wav", SOUND_PUZZLE_SUB, 1.f);

			Reset_Connected();
			Check_Connected(m_pPipeBoardCom->m_pStatingPipe, 31);	
		}
		
	}
}

CPipeBoard* CPipeBoard::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPipeBoard* pPipeBoard = new CPipeBoard(pGraphicDev);

	if (FAILED(pPipeBoard->Ready_GameObject()))
	{
		Safe_Release(pPipeBoard);
		MSG_BOX("pPipeBoard Create Failed");
		return nullptr;
	}

	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::OBJECT, pPipeBoard);
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::PUZZLE, pPipeBoard);
	return pPipeBoard;	
}

void CPipeBoard::Free()
{
	Engine::CGameObject::Free();
}

void CPipeBoard::Match_Puzzle()
{
	m_pPipeBoardCom->m_iCurPipeCount++;
	m_pPipeBoardCom->m_bIsPipeSpawned = false;
}