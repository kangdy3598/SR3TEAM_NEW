#include "pch.h"
#include "FlowerGlow.h"
#include "PlantOrb.h"
#include "Export_Utility.h"

CFlowerGlow::CFlowerGlow(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_iImageID(0)
{
}

CFlowerGlow::~CFlowerGlow()
{
}

HRESULT CFlowerGlow::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_bIsActive = false;
	m_eTag = TAG_NONE;
	m_pTransformCom->m_vScale = { 100.f, 40.f, 100.f };
	m_pTexTransformCom->m_vScale = { 100.f, 100.f, 0.f };
	m_pTexTransformCom->Rotation(ROT_X, 90.f * 3.14159265359f / 180.f);
	m_vecTexture.resize(6); 
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_MoonForest_FlowerLightStandArea_Blue.png", &m_vecTexture[0]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_MoonForest_FlowerLightStandArea_Yellow.png", &m_vecTexture[1]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_MoonForest_FlowerLightStandArea_Red.png", &m_vecTexture[2]);	
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_MoonForest_FlowerLightStandArea_YellowBlue.png", &m_vecTexture[3]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_MoonForest_FlowerLightStandArea_BlueRed.png", &m_vecTexture[4]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_MoonForest_FlowerLightStandArea_RedYellow.png", &m_vecTexture[5]);

	return S_OK;
}

_int CFlowerGlow::Update_GameObject(const _float& fTimeDelta)
{
	if (!m_bIsActive)
		return 0;

	Add_RenderGroup(RENDER_TRANSLUCNET, this);

	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	return iExit;
}

void CFlowerGlow::LateUpdate_GameObject(const _float& fTimeDelta)
{
	if (!m_bIsActive)
		return;

	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CFlowerGlow::Render_GameObject()
{
	if (!m_bIsActive)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTexTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetTexture(0, m_vecTexture[m_iImageID]);
	m_pBufferCom->Render_Buffer();

	//m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	//////m_pBoundBox->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CFlowerGlow::OnCollision(CGameObject* _pOther)
{
	if (_pOther->Get_Tag() == TAG_ENEMY) {
		if (static_cast<Engine::CPlantOrbCom*>(_pOther->Get_Component(ID_DYNAMIC, L"Com_PlantOrb")) == nullptr)
			return;

		if (static_cast<CPlantOrb*>(_pOther)->Get_ImageID() != m_iImageID)
			return;

		static_cast<CPlantOrb*>(_pOther)->Set_TargetID(9);
		static_cast<CPlantOrb*>(_pOther)->Set_Duration();
	}
}

void CFlowerGlow::OnCollisionEnter(CGameObject* _pOther)
{

}

void CFlowerGlow::OnCollisionExit(CGameObject* _pOther)
{
}

HRESULT CFlowerGlow::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	pComponent = m_pTexTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_TexTransform", pComponent });

	pComponent = m_pBoundBox = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pBoundBox->SetGameObjectPtr(this);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", pComponent });
	return S_OK;
}

CFlowerGlow* CFlowerGlow::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CFlowerGlow* pCrystal = new CFlowerGlow(pGraphicDev);

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

void CFlowerGlow::Free()
{
	Engine::CGameObject::Free();
}