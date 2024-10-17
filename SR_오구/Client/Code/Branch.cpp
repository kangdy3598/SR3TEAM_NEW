#include "pch.h"
#include "Branch.h"
#include "Player.h"
#include "ItemUI.h"

_bool CBranch::g_Acquired(false);

CBranch::CBranch(LPDIRECT3DDEVICE9 pGraphicDev)
	:CItem(pGraphicDev)
{
}

CBranch::~CBranch()
{
}

HRESULT CBranch::Ready_GameObject()
{
	CItem::Ready_GameObject();

	m_tInfo = { OTHER,
		BRANCH,
		L"나뭇가지",
		L"흔한 나뭇가지.",
		1, 1 };

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

void CBranch::LateReady_GameObject()
{
	CItem::LateReady_GameObject();
}

_int CBranch::Update_GameObject(const _float& fTimeDelta)
{

	return CItem::Update_GameObject(fTimeDelta);
}

void CBranch::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CItem::LateUpdate_GameObject(fTimeDelta);
}

void CBranch::Render_GameObject()
{
	if (m_tInfo.bOnField)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
		m_pTextureCom->Set_Texture();
		m_pBufferCom->Render_Buffer();
		//m_pColliderCom->Render_Buffer();

		return;
	}
	else if (m_pPlayer->GetVisitingStore())
	{
		//아이템
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pStoreTransformCom->Get_WorldMatrix());
		m_pTextureCom->Set_Texture();
		m_pBufferCom->Render_Buffer();

		//가격태그
		m_pCountRCTransformCom->m_vInfo[INFO_POS].x = m_pStoreTransformCom->m_vInfo[INFO_POS].x + 36;
		m_pCountRCTransformCom->m_vInfo[INFO_POS].y = m_pStoreTransformCom->m_vInfo[INFO_POS].y - 44;
		m_pCountRCTransformCom->m_vScale = { 40.f, 17.f, 1.f };

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pCountRCTransformCom->Get_WorldMatrix());
		m_pPriceTextureCom->Set_Texture();
		m_pBufferCom->Render_Buffer();

		_vec2 vCountPos;

		vCountPos.x = m_pStoreTransformCom->m_vInfo[INFO_POS].x + (WINCX * 0.5f) + 28;
		vCountPos.y = -(m_pStoreTransformCom->m_vInfo[INFO_POS].y) + (WINCY * 0.5f) + 34;

		wchar_t ItemCount[32];

		swprintf(ItemCount, 32, L"%d", m_tInfo.iPrice);

		Engine::Render_Font(L"Font_OguBold24", ItemCount, &vCountPos, D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.f));



	}
	else if (m_pInven->Get_CurFilter() == m_tInfo.eType
		&& m_pPlayer->GetPlayerInven()
		&& !m_tInfo.bOnField)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
		m_pTextureCom->Set_Texture();
		m_pBufferCom->Render_Buffer();

		m_pCountRCTransformCom->m_vInfo[INFO_POS].x = m_pTransformCom->m_vInfo[INFO_POS].x + 46;
		m_pCountRCTransformCom->m_vInfo[INFO_POS].y = m_pTransformCom->m_vInfo[INFO_POS].y - 44;
		m_pCountRCTransformCom->m_vScale = { 20.f, 17.f, 1.f };

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pCountRCTransformCom->Get_WorldMatrix());
		m_pCountRCTextureCom->Set_Texture();
		m_pBufferCom->Render_Buffer();

		_vec2 vCountPos;

		vCountPos.x = m_pTransformCom->m_vInfo[INFO_POS].x + (WINCX * 0.5f) + 34;
		vCountPos.y = -(m_pTransformCom->m_vInfo[INFO_POS].y) + (WINCY * 0.5f) + 34;

		wchar_t Division[32] = L"x";
		wchar_t ItemCount[32];

		swprintf(ItemCount, 32, L"%d", m_tInfo.iItemCount);

		wcscat_s(Division, 32, ItemCount);   // "x + 개수"
		Engine::Render_Font(L"Font_OguBold24", Division, &vCountPos, D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.f));
	}
}

void CBranch::OnCollisionEnter(CGameObject* _pOther)
{
	if (CBranch::g_Acquired == true)
	{
		m_pInven->Add_Item(dynamic_cast<CItem*>(this));

		//아이템 획득 이펙트 발생
		return;
	}

	/*if (m_pPlayer->GetPlayerState() == PLAYERSTATE::PLY_PICKUP &&
		m_pPlayer->GetAnimationComp()->IsAnimationEnd())
	{
		m_pItemUI->CallItemUI(true);
		m_pItemUI->Set_Texture(m_pTextureCom);
		m_pItemUI->Set_Text(m_tInfo);
	}*/
}

void CBranch::OnCollision(CGameObject* _pOther)
{
	if (_pOther->GetObjectKey() != L"Player")
		return;

	if (CBranch::g_Acquired)
		return;

	m_pPickUpButton->CallButton(true);

	if (GetKeyDown(DIK_A)) //줍기
	{
		CBranch::g_Acquired = true;

		// 1005 동영
		// 줍기 -> 플레이어, 오브젝트 멈춤 + 플레이어 애니메이션 실행 -> 애니메이션이 끝나면 UI 출력 
		Engine::Get_Layer(L"Layer_GameLogic")->SetGameState(GAMESTATE_EXPLAINATION);
		dynamic_cast<CPlayer*>(_pOther)->ChangePickUpState();
		//m_pItemUI->CallItemUI(true);
		//m_pItemUI->Set_Texture(m_pTextureCom);
		//m_pItemUI->Set_Text(m_tInfo);
		m_pInven->Add_Item(dynamic_cast<CItem*>(this));
		m_pColliderCom = nullptr;
	}

	/*if (CBranch::g_Acquired == false)
	{

		
	}*/
}


HRESULT CBranch::Add_Component()
{
	CComponent* pComponent = NULL;

	// RC TEX
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	//SmallFruit
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Branch"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_TextureBranch", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });
	m_pTransformCom->m_vScale = { 55.f, 55.f, 1.f };
	m_pTransformCom->m_vInfo[INFO_POS] = { 0.f, 0.f, 0.1f };

	//CountRC
	pComponent = m_pCountRCTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_CountRC"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_TextureCountRC", pComponent });

	pComponent = m_pCountRCTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_TransformCountRC", pComponent });
	m_pCountRCTransformCom->m_vScale = { 20.f, 17.f, 1.f };
	m_pCountRCTransformCom->m_vInfo[INFO_POS] = { 0.f, 0.f, 0.1f };

	return S_OK;
}

CBranch* CBranch::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBranch* pItem = new CBranch(pGraphicDev);

	if (FAILED(pItem->Ready_GameObject()))
	{
		Safe_Release(pItem);
		MSG_BOX("Branch Create Failed");
		return nullptr;
	}

	return pItem;
}
