#include "pch.h"
#include "FightUI.h"

CFightUI::CFightUI(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
	, m_bCall(false), m_fElapsedTime(0.f)
{
}

CFightUI::~CFightUI()
{
}

HRESULT CFightUI::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	return S_OK;
}

_int CFightUI::Update_GameObject(const _float& fTimeDelta)
{
    if (!m_bCall)
        return 0;

    _int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

    Engine::Add_RenderGroup(RENDER_UI, this);
	m_fElapsedTime += fTimeDelta;

    if (m_fElapsedTime >= 2.f)
		m_bCall = false;

    return iExit;
}

void CFightUI::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CFightUI::Render_GameObject()
{
	if (!m_bCall)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();

}

HRESULT CFightUI::Add_Component()
{
    CComponent* pComponent = NULL;

    // 렉트텍스쳐
    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

    //TextBox
    pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_FightUI"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });
    m_pTransformCom->m_vInfo[INFO_POS] = { 0.f, 0.f, 0.1f };
    m_pTransformCom->m_vScale = { 131.5f, 41.5f, 1.f };

    return S_OK;

}

CFightUI* CFightUI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CFightUI* pUI = new CFightUI(pGraphicDev);

    if (FAILED(pUI->Ready_GameObject()))
    {
        Safe_Release(pUI);
        MSG_BOX("CFightUI Create Failed");
        return nullptr;
    }
    return pUI;
}

void CFightUI::Free()
{
    Engine::CGameObject::Free();
}
