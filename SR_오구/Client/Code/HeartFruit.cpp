#include "pch.h"
#include "HeartFruit.h"
#include "Player.h"

CHeartFruit::CHeartFruit(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev), m_bDead(false)
{
}

CHeartFruit::~CHeartFruit()
{
}

HRESULT CHeartFruit::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

void CHeartFruit::LateReady_GameObject()
{
	m_pPickUpButton = dynamic_cast<CPickUpButton*>(CPickUpButton::Create(m_pGraphicDev));
	NULL_CHECK_RETURN(m_pPickUpButton);

	m_pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
	NULL_CHECK_RETURN(m_pPlayer);

	m_pEffect = dynamic_cast<CAddHP*>(Engine::Get_GameObject(L"Layer_UI", L"AddHP_UI"));
	NULL_CHECK_RETURN(m_pEffect);

	CGameObject::LateReady_GameObject();
}

_int CHeartFruit::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return 0;

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	if (m_pPickUpButton)
	{
		m_pPickUpButton->Update_GameObject(fTimeDelta);
	}

	return CGameObject::Update_GameObject(fTimeDelta);
}

void CHeartFruit::LateUpdate_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return;

	if (m_pPickUpButton)
	{
		m_pPickUpButton->LateUpdate_GameObject(fTimeDelta);
	}

	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CHeartFruit::Render_GameObject()
{
	if (m_bDead)
	{
		Engine::CGameObject::Free();
		map<const _tchar*, CLayer*>& mapLayer = CManagement::GetInstance()->GetCurScenePtr()->GetLayerMapPtr();

		CLayer* pLayer = nullptr;
		for (auto iter = mapLayer.begin(); iter != mapLayer.end(); ++iter)
		{
			const _tchar* layerKey = iter->first;

			//if (_tcscmp(layerKey, _T("Layer_GameLogic")) == 0)
			if (lstrcmp(layerKey, L"Layer_GameLogic") == 0)
			{
				pLayer = iter->second;
				//break;
			}

		}
		map<const _tchar*, CGameObject*>& pMap = pLayer->GetLayerGameObjectPtr();
		auto iter = find_if(pMap.begin(), pMap.end(), CTag_Finder(L"HeartFruit"));

		if (iter != pMap.end())
		{
			pMap.erase(iter);
		}

		return;
	}

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();
	//m_pColliderCom->Render_Buffer();

	if (m_pPickUpButton)
	{
		m_pPickUpButton->Render_GameObject();
	}

	return;
}

void CHeartFruit::OnCollision(CGameObject* _pOther)
{
	if (_pOther->GetObjectKey() != L"Player")
		return;

	if (m_bDead)
		return;

	m_pPickUpButton->CallButton(true);

	if (Engine::GetKeyDown(DIK_A)) //줍기
	{
		// 1005 동영
		// 줍기 -> 플레이어, 오브젝트 멈춤 + 플레이어 애니메이션 실행 -> 애니메이션이 끝나면 UI 출력 
		Engine::Get_Layer(L"Layer_GameLogic")->SetGameState(GAMESTATE_EXPLAINATION);
		dynamic_cast<CPlayer*>(_pOther)->ChangePickUpState();

		m_bDead = true;
		m_pPlayer->SetPlayerMaxHP(1);
		//Engine::Play_Sound(L"SFX_147_OguItemAdd.wav", SOUND_EFFECT, 0.7);
		m_pEffect->Call_AddHP();
	}
}


HRESULT CHeartFruit::Add_Component()
{
	CComponent* pComponent = NULL;

	// RC TEX
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_HeartFruit"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });
	m_pTransformCom->m_vScale = { 15.f, 15.f, 15.f };
	
	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pColliderCom->SetGameObjectPtr(this);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", pComponent });

	return S_OK;

}

CHeartFruit* CHeartFruit::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CHeartFruit* pItem = new CHeartFruit(pGraphicDev);

	if (FAILED(pItem->Ready_GameObject()))
	{
		Safe_Release(pItem);
		MSG_BOX("CHeartFruit Create Failed");
		return nullptr;
	}

	return pItem;
}

void CHeartFruit::Free()
{
	Engine::CGameObject::Free();
}
