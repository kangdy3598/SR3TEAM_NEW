#include "pch.h"
#include "StoreNPC.h"

CStoreNPC::CStoreNPC(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
	, m_pInterButton(nullptr), m_pTextBox(nullptr)
	, m_bConversation(false), m_bStoreOn(false)
	, m_pTex(nullptr)
{
}

CStoreNPC::~CStoreNPC()
{
}

HRESULT CStoreNPC::Ready_GameObject()
{
	return S_OK;
}

void CStoreNPC::LateReady_GameObject()
{
	//m_pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
	//NULL_CHECK_RETURN(m_pPlayer);

	m_pStoreUI = dynamic_cast<CStoreUI*>(Engine::Get_GameObject(L"Layer_UI", L"Store_UI"));
	NULL_CHECK_RETURN(m_pStoreUI);


	if (!m_pInterButton) //Interaction
	{
		m_pInterButton = CInteractionButton::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(m_pInterButton);
	}
	if (!m_pTextBox)
	{
		m_pTextBox = CTextBox::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(m_pInterButton);
	}
}

_int CStoreNPC::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	m_pInterButton->Update_GameObject(fTimeDelta);
	m_pTextBox->Update_GameObject(fTimeDelta);

	return iExit;
}

void CStoreNPC::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CStoreNPC::Render_GameObject()
{
}

HRESULT CStoreNPC::Add_Component()
{
	return S_OK;
}

void CStoreNPC::Free()
{
	Engine::CGameObject::Free();
}
