#include "pch.h"
#include "QuickSlot.h"
#include "Item.h"

CQuickSlot::CQuickSlot(LPDIRECT3DDEVICE9 pGraphicDev)
    :Engine::CGameObject(pGraphicDev)
    , m_fElapsedTime(100.f)
{
    for (int i = 0; i < 4; ++i)
        m_pQuickSlot[i] = nullptr;
}

CQuickSlot::~CQuickSlot()
{
}

HRESULT CQuickSlot::Ready_GameObject()
{
    

    m_bDontDestroy = true;

    m_QuickInterval = 65.f; // 퀵슬롯 아이템 간격
    m_vFirstItemPos = { -516.f, 242.f, 0.1f };

    m_ResultScale = { 210.f, 39.f, 1.f };
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    return S_OK;
}

_int CQuickSlot::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

    Engine::Add_RenderGroup(RENDER_UI, this);

    BulkingSlot(fTimeDelta);

    return iExit;
}

void CQuickSlot::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CQuickSlot::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pTextureCom->Set_Texture();
    m_pBufferCom->Render_Buffer();

    for (auto& pItem : m_pQuickSlot)
    {
        if (pItem == nullptr)
            continue;

        pItem->Render_QuickItem();
    }
}

void CQuickSlot::Add_QuickSlotItem(_int _iIndex, CItem* _pItem)
{
    m_pQuickSlot[_iIndex] = _pItem;  //퀵슬롯에 아템 넣기!
    _pItem->Set_QuickPos(Set_QuickSlotPos(_iIndex)); //퀵슬롯 트랜스폼 위치 변경 세팅ㅠㅠ
}

void CQuickSlot::Use_QuickItem(_int _iIdx)
{
    if (nullptr == m_pQuickSlot[_iIdx])
        return;
    m_pQuickSlot[_iIdx]->Use_Item();
}

void CQuickSlot::BulkingSlot(const _float& fTimeDelta)
{
    // 셀렉터에서 m_pQuickSlot = Set_Time();

    if (m_fElapsedTime <= 0.5f)
    {
        if (m_fElapsedTime <= 0.25f)
        {
            m_fElapsedTime += fTimeDelta;
            m_pTransformCom->m_vScale += ((m_ResultScale - m_PrevScale) / 0.25f) * fTimeDelta;
            return;
        }
        m_fElapsedTime += fTimeDelta;
        m_pTransformCom->m_vScale += ((m_PrevScale - m_ResultScale) / 0.25f) * fTimeDelta;
    }

}

HRESULT CQuickSlot::Add_Component()
{
    CComponent* pComponent = NULL;

    // 렉트텍스쳐
    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

    //SHORTKEY
    pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_KeyBoardUI"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TextureShortKey", pComponent });

    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TransformShortKey", pComponent });
    m_pTransformCom->m_vInfo[INFO_POS] = { -455.f, 245.f, 0.1f };
    m_pTransformCom->m_vScale = { 200.f, 37.5f, 1.f };

    m_PrevScale = m_pTransformCom->m_vScale; // 벌킹을 위해 이전 스케일 저장.

    return S_OK;
}

CQuickSlot* CQuickSlot::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CQuickSlot* pUI = new CQuickSlot(pGraphicDev);

    if (FAILED(pUI->Ready_GameObject()))
    {
        Safe_Release(pUI);
        MSG_BOX("QuickSlot Create Failed");
        return nullptr;
    }

    return pUI;
}

void CQuickSlot::Free()
{
    Engine::CGameObject::Free();
}
