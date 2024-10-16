#include "pch.h"
#include "WhitePhosphorus.h"
#include "Export_Utility.h"

CWhitePhosphorus::CWhitePhosphorus(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_bIsLaunched(false), m_fDuration(0.05f), m_fTime(0)
{
	
}

CWhitePhosphorus::~CWhitePhosphorus()
{
}

HRESULT CWhitePhosphorus::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);		
	m_pTransformCom->m_vScale = { 10.f, 10.f, 10.f };		
	m_vecTexture.resize(9);	
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_JellyBomb.png", &m_vecTexture[0]);
	m_eTag = TAG_PLAYERATTACK;
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/JellyBomb_Anim/Sprite_JellyBombExplosion_01.png", &m_vecTexture[1]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/JellyBomb_Anim/Sprite_JellyBombExplosion_02.png", &m_vecTexture[2]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/JellyBomb_Anim/Sprite_JellyBombExplosion_03.png", &m_vecTexture[3]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/JellyBomb_Anim/Sprite_JellyBombExplosion_04.png", &m_vecTexture[4]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/JellyBomb_Anim/Sprite_JellyBombExplosion_05.png", &m_vecTexture[5]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/JellyBomb_Anim/Sprite_JellyBombExplosion_06.png", &m_vecTexture[6]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/JellyBomb_Anim/Sprite_JellyBombExplosion_07.png", &m_vecTexture[7]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/JellyBomb_Anim/Sprite_JellyBombExplosion_08.png", &m_vecTexture[8]);
	return S_OK;
}

_int CWhitePhosphorus::Update_GameObject(const _float& fTimeDelta)
{
	if (!m_bIsActive)
		return 0;

	Add_RenderGroup(RENDER_TRANSLUCNET, this);
	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	if (m_bIsLaunched) {
		const float gravity = -(rand() % 10);
		m_vVelocity.y += gravity * fTimeDelta * 10.f;
		vPos += m_vVelocity * fTimeDelta * 10.f;
		m_pTransformCom->Set_Pos(vPos);		

		if (vPos.y < 15.0f) {				
			m_bIsLaunched = false;
			vPos.y = 40.f;
			m_pTransformCom->m_vScale = { 40.f, 40.f, 40.f };
		}
	}

	if (!m_bIsLaunched) {
		m_fTime += fTimeDelta;
		if (m_fDuration < m_fTime) {
			++m_iImageID;
			m_fTime = 0;

			if (m_iImageID >= m_vecTexture.size()) {
				//Play_Sound(L"SFX_89_ObjectExplosion.wav", SOUND_EFFECT, 1.f);
				m_bIsActive = false;
			}			
		}
	}

	m_pTransformCom->Set_Pos(vPos);	

	return iExit;
}

void CWhitePhosphorus::LateUpdate_GameObject(const _float& fTimeDelta)
{
	if (!m_bIsActive)
		return;

	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CWhitePhosphorus::Render_GameObject()
{
	if (!m_bIsActive)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetTexture(0, m_vecTexture[m_iImageID]);
	m_pBufferCom->Render_Buffer();
	////m_pBoundBox->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CWhitePhosphorus::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_StonePushable"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	pComponent = m_pBoundBox = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pBoundBox->SetGameObjectPtr(this);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", pComponent });

	return S_OK;
}

CWhitePhosphorus* CWhitePhosphorus::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CWhitePhosphorus* pCrystal = new CWhitePhosphorus(pGraphicDev);

	if (FAILED(pCrystal->Ready_GameObject()))
	{
		Safe_Release(pCrystal);
		MSG_BOX("pPipeBoard Create Failed");
		return nullptr;
	}

	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::OBJECT, pCrystal);	

	return pCrystal;
}

void CWhitePhosphorus::Launch()
{
	m_pTransformCom->m_vScale = { 10.f, 10.f, 10.f };
	m_bIsLaunched = true;
	m_bIsActive = true;
	m_iImageID = 0;
	m_fTime = 0;
}

void CWhitePhosphorus::Free()
{
	Engine::CGameObject::Free();
}

void CWhitePhosphorus::OnCollision(CGameObject* _pOther)
{
}

void CWhitePhosphorus::OnCollisionEnter(CGameObject* _pOther)
{

}

void CWhitePhosphorus::OnCollisionExit(CGameObject* _pOther)
{
}

void CWhitePhosphorus::Init_Pos(_float _fX, _float _fZ)
{
	m_pTransformCom->Set_Pos(_fX , 200.f, _fZ);	
}