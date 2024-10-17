#include "pch.h"
#include "Stone.h"
#include "Export_Utility.h"
#include "PlayerInteractionBox.h"
#include "Player.h"

CStone::CStone(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_iStoneID(0), m_bIsLaunched(false), m_iImageID(0), m_fTime(0), m_iTargetID(0)
	, m_vDirSmash{0.f,0.f,0.f}
	, m_bThrowStone(false)
{
}

CStone::~CStone()
{
}

HRESULT CStone::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);	
	int type = OBJ_TYPE::LIFT_ABLE + OBJ_TYPE::PUSH_ABLE;
	SetObjectType(type);
	m_eTag = TAG_STONE;
	m_pTransformCom->m_vScale = { 15.f, 15.f, 15.f };
	m_pShadowTransformCom->m_vScale = { 14.f, 12.f, 14.f };
	m_pShadowTransformCom->Rotation(ROT_X, 90.f * 3.14159265359f / 180.f);
	m_vVelocity = _vec3{ 0.f, 30.f, 40.f };

	m_vecTexture.resize(11);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/StoneBreak_Anim/Sprite_StonePushable.png", &m_vecTexture[0]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/StoneBreak_Anim/Sprite_CatapultHit_01.png", &m_vecTexture[1]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/StoneBreak_Anim/Sprite_CatapultHit_02.png", &m_vecTexture[2]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/StoneBreak_Anim/Sprite_CatapultHit_03.png", &m_vecTexture[3]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/StoneBreak_Anim/Sprite_CatapultHit_04.png", &m_vecTexture[4]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/StoneBreak_Anim/Sprite_CatapultHit_05.png", &m_vecTexture[5]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/StoneBreak_Anim/Sprite_CatapultHit_06.png", &m_vecTexture[6]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/StoneBreak_Anim/Sprite_CatapultHit_07.png", &m_vecTexture[7]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/StoneBreak_Anim/Sprite_CatapultHit_08.png", &m_vecTexture[8]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/StoneBreak_Anim/Sprite_CatapultHit_09.png", &m_vecTexture[9]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/StoneBreak_Anim/Sprite_CatapultHit_10.png", &m_vecTexture[10]);	

	return S_OK;
}

_int CStone::Update_GameObject(const _float& fTimeDelta)
{
	if (!m_bIsActive)
		return 0;

	Add_RenderGroup(RENDER_ALPHA, this);
	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	// 돌이 스매쉬랑 충돌했을때
	if (m_vDirSmash != _vec3{0.f,0.f,0.f})
	{
		m_pTransformCom->Move_Pos(&m_vDirSmash, fTimeDelta, 300.f);
		
		if(m_bThrowStone ==true)
		{
			dwtime = GetTickCount64();
			m_bThrowStone = false; 
		}
	}

	if (m_iImageID != m_iTargetID) {
		m_fTime += fTimeDelta;

		if (m_fTime >= 0.02f) {
			if (m_iTargetID > m_iImageID)
				m_iImageID++;

			if (m_iImageID == m_iTargetID)
				m_bIsActive = false;

			m_fTime = 0;
		}
	}

	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	if (m_bIsLaunched) {
		const float gravity = -9.8f;
		m_vVelocity.y += gravity * fTimeDelta * 10.f;
		vPos += m_vVelocity * fTimeDelta * 10.f;
			m_pTransformCom->Set_Pos(vPos);
		if (vPos.y <= 10.0f) {
			vPos.y = 15.0f;			
			m_bIsLaunched = false;
			m_vVelocity = _vec3{ 0.f, 30.f, 40.f };
		}

		m_pTransformCom->Set_Pos(vPos);
	}	

	m_pShadowTransformCom->Set_Pos(vPos.x, 0.3f, vPos.z + 13.f);

	return iExit;
}

void CStone::LateUpdate_GameObject(const _float& fTimeDelta)
{
	if (!m_bIsActive)
		return;

	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CStone::Render_GameObject()
{
	if (!m_bIsActive)
		return;
	if (CManagement::GetInstance()->m_imap_stage == 2)
	{
		m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	}

	if (dwtime + 3000 < GetTickCount64())
	{
		Engine::CGameObject::Free();
		map<const _tchar*, CLayer*>& mapLayer = CManagement::GetInstance()->GetCurScenePtr()->GetLayerMapPtr();
		//그니깐 레이어에서 삭제해줘야한다는것. 
		CLayer* pLayer = nullptr;

		for (auto iter = mapLayer.begin(); iter != mapLayer.end(); ++iter)
		{
			const _tchar* layerKey = iter->first;

			if (_tcscmp(layerKey, _T("Layer_GameLogic")) == 0)
			{
				pLayer = iter->second;
			}
		}
		map<const _tchar*, CGameObject*>& pMap = pLayer->GetLayerGameObjectPtr();

		auto iter = find_if(pMap.begin(), pMap.end(), CTag_Finder(L"Stone0"));

		if (iter != pMap.end())
		{
			pMap.erase(iter);
		}

		return; 
	}

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetTexture(0, m_vecTexture[m_iImageID]);
	m_pBufferCom->Render_Buffer();
	m_pBoundBox->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pShadowTransformCom->Get_WorldMatrix());
	m_pShadowTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();
	
	if (CManagement::GetInstance()->m_imap_stage == 2)
	{
		m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, false);
	}
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CStone::Break()
{
	m_iTargetID = 11;
	m_bIsLaunched = false;
	m_pTransformCom->m_vScale = { 40.f, 40.f, 40.f };
}

HRESULT CStone::Add_Component()
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

	pComponent = m_pShadowTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Shadow"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_ShadowTexture", pComponent });

	pComponent = m_pShadowTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_ShadowTransform", pComponent });

	pComponent = m_pBoundBox = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pBoundBox->SetGameObjectPtr(this);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", pComponent });

	return S_OK;
}

CStone* CStone::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStone* pCrystal = new CStone(pGraphicDev);

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

void CStone::Launch()
{
	m_bIsLaunched = true;
}

void CStone::Free()
{
	Engine::CGameObject::Free();
}

void CStone::OnCollision(CGameObject* _pOther)
{
}

void CStone::OnCollisionEnter(CGameObject* _pOther)
{
	//여기서 먹고 업데이트가 되야하는데
	if(_pOther->GetObjectKey() == L"PlayerInteractionBox")
	{
		if(dynamic_cast<CPlayerInteractionBox*>(_pOther)->GetPlayer()->GetPlayerState() == PLY_SMASH)
		{
			m_vDirSmash = dynamic_cast<CPlayerInteractionBox*>(_pOther)->GetPlayer()->GetPlayerDirVector2();
			m_bThrowStone = true;

			m_pInteractionBox = dynamic_cast<CPlayerInteractionBox*>(_pOther);
		}
		
	}

}

void CStone::OnCollisionExit(CGameObject* _pOther)
{
	SetInteractingObj();
}

void CStone::SetInteractingObj()
{
	if (m_pInteractionBox)
		m_pInteractionBox->GetPlayer()->SetInteractingObj(nullptr);
}
