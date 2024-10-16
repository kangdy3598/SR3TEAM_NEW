#include "pch.h"
#include "Pipe.h"
#include "PipeBoard.h"
#include "Export_Utility.h"

CPipe::CPipe(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CPipe::~CPipe()
{
}

HRESULT CPipe::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->m_vScale = { 12.f, 12.f, 0.f };
	m_pTransformCom->Rotation(ROT_X, 90.f * 3.14159265359f / 180.f);
	m_iImageID = 0;	
	m_vecTexture.resize(12);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_MoonForest_MazePushPuzzleThreeWay.png", &m_vecTexture[0]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_MoonForest_MazePushPuzzleRight.png", &m_vecTexture[1]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_MoonForest_MazePushPuzzleConer.png", &m_vecTexture[2]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_MoonForest_MazePickPuzzleThreeWay.png", &m_vecTexture[3]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_MoonForest_MazePickPuzzleStraight.png", &m_vecTexture[4]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_MoonForest_MazePickPuzzleBent.png", &m_vecTexture[5]);

	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_MoonForest_MazePushPuzzleThreeWay_Connected.png", &m_vecTexture[6]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_MoonForest_MazePushPuzzleRight_Connected.png", &m_vecTexture[7]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_MoonForest_MazePushPuzzleConer_Connected.png", &m_vecTexture[8]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_MoonForest_MazePickPuzzleThreeWay_Connected.png", &m_vecTexture[9]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_MoonForest_MazePickPuzzleStraight_Connected.png", &m_vecTexture[10]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_MoonForest_MazePickPuzzleBent_Connected.png", &m_vecTexture[11]);

	return S_OK;
}

_int CPipe::Update_GameObject(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);

	//if(m_pPipeCom->m_ePipeType == Engine::EPipeType::)

	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	return iExit;
}

void CPipe::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CPipe::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetTexture(0, m_vecTexture[m_iImageID + (m_pPipeCom->m_bIsConnected * 6)]);	
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CPipe::Initialize_Pipe_Option(PIPEFLOW _eFlow0, PIPEFLOW _eFlow1, PIPEFLOW _eFlow2, PIPEFLOW _eFlow3, _bool _bIsFixed , _int _iImageID, _vec3 _vPos, _vec3 _fAngle)
{	
	if(_eFlow0 != FLOW_END)
		m_pPipeCom->Add_PipeFlow(_eFlow0);
	if (_eFlow1 != FLOW_END)
		m_pPipeCom->Add_PipeFlow(_eFlow1);
	if (_eFlow2 != FLOW_END)
		m_pPipeCom->Add_PipeFlow(_eFlow2);
	if (_eFlow3 != FLOW_END)
		m_pPipeCom->Add_PipeFlow(_eFlow3);

	m_pPipeCom->m_bIsFixed = _bIsFixed;
	m_iImageID = _iImageID;
	m_pTransformCom->Set_Pos(_vPos.x, _vPos.y, _vPos.z);
	m_pTransformCom->Rotation(ROT_X, _fAngle.x * 3.14159265359f / 180.f);
	m_pTransformCom->Rotation(ROT_Y, _fAngle.y * 3.14159265359f / 180.f);
	m_pTransformCom->Rotation(ROT_Z, _fAngle.z * 3.14159265359f / 180.f);	
}

HRESULT CPipe::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	//pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_PipeTex"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });
		
	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	pComponent = m_pPipeCom = dynamic_cast<CPipeCom*>(Engine::Clone_Proto(L"Proto_Pipe"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Pipe", pComponent });

	return S_OK;
}

CPipe* CPipe::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPipe* pPipe = new CPipe(pGraphicDev);

	if (FAILED(pPipe->Ready_GameObject()))
	{
		Safe_Release(pPipe);
		MSG_BOX("pPipeBoard Create Failed");
		return nullptr;
	}

	return pPipe;
}

void CPipe::Free()
{
	Engine::CGameObject::Free();
}
