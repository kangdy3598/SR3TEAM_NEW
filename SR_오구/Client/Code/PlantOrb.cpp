#include "pch.h"
#include "PlantOrb.h"
#include "Player.h"
#include "Export_Utility.h"

CPlantOrb::CPlantOrb(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_iImageID(1), m_fDruration(0)
{
}

CPlantOrb::~CPlantOrb()
{
}

HRESULT CPlantOrb::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->m_vScale = { 10.f, 10.f, 10.f };
	m_pShadowTransformCom->m_vScale = { 8.f, 8.f, 0.f };
	m_pShadowTransformCom->Rotation(ROT_X, 90.f * 3.14159265359f / 180.f);
	m_eTag = TAG_ENEMY;
	m_vecTexture.resize(5);	
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Plant_Bullet_Blue.png", &m_vecTexture[0]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Plant_Bullet_Yellow.png", &m_vecTexture[1]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Plant_Bullet_Red.png", &m_vecTexture[2]);	
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Plant_Bullet_BlueRed.png", &m_vecTexture[3]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Plant_Bullet_BlueRed.png", &m_vecTexture[4]);

	return S_OK;
}

_int CPlantOrb::Update_GameObject(const _float& fTimeDelta)
{
	if (!m_bIsActive)
		return 0;

	Add_RenderGroup(RENDER_ALPHA, this);
	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	m_fDruration += fTimeDelta;
	if (m_fDruration >= 2) {
		m_bIsActive = false;
		m_fDruration = 0;
	}

	Move(fTimeDelta);
	return iExit;
}

void CPlantOrb::LateUpdate_GameObject(const _float& fTimeDelta)
{
	if (!m_bIsActive)
		return;

	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CPlantOrb::Render_GameObject()
{
	if (!m_bIsActive)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetTexture(0, m_vecTexture[m_iImageID]);
	m_pBufferCom->Render_Buffer();
	////m_pBoundBox->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pShadowTransformCom->Get_WorldMatrix());
	m_pShadowTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CPlantOrb::OnCollision(CGameObject* _pOther)
{
}

void CPlantOrb::OnCollisionEnter(CGameObject* _pOther)
{
	if (_pOther->Get_Tag() == TAG_NONE)
		return;

	if (_pOther->Get_Tag() == TAG_PLAYER) {
		static_cast<CPlayer*>(_pOther)->SetPlayerCurHP(-1);
	}

	m_fDruration = 0;
	m_bIsActive = false;
}

void CPlantOrb::OnCollisionExit(CGameObject* _pOther)
{
}

void CPlantOrb::Init_Pos(_float _fX, _float _fZ, _float _fDir)
{	
	m_pTransformCom -> Set_Pos(_fX + (15 * _fDir), 20.f, _fZ);
	m_pShadowTransformCom->Set_Pos(_fX + (15 * _fDir), 0.2f, _fZ);
}

void CPlantOrb::Move(const _float& fTimeDelta)
{
	_vec3 vRight;	
	m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);
	m_pTransformCom->Move_Pos(D3DXVec3Normalize( &vRight, &vRight), fTimeDelta, m_fSpeed * 5.f);
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	m_pShadowTransformCom->Set_Pos(vPos.x, 0.03f, vPos.z);
}

HRESULT CPlantOrb::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	pComponent = m_pBoundBox = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pBoundBox->SetGameObjectPtr(this);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", pComponent });

	pComponent = m_pPlantOrbCom = dynamic_cast<CPlantOrbCom*>(Engine::Clone_Proto(L"Proto_PlantOrbCom"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_PlantOrb", pComponent });

	pComponent = m_pShadowTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Shadow"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_ShadowTexture", pComponent });

	pComponent = m_pShadowTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_ShadowTransform", pComponent });

	return S_OK;
}

CPlantOrb* CPlantOrb::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlantOrb* pCrystal = new CPlantOrb(pGraphicDev);

	if (FAILED(pCrystal->Ready_GameObject()))
	{
		Safe_Release(pCrystal);
		MSG_BOX("pPipeBoard Create Failed");
		return nullptr;
	}
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::OBJECT, pCrystal);
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::PUZZLE, pCrystal);
	return pCrystal;
}

void CPlantOrb::Free()
{
	Engine::CGameObject::Free();
}