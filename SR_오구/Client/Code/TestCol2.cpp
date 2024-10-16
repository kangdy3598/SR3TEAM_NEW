#include "pch.h"
#include "TestCol2.h"
#include "Define.h"
#include "Export_System.h"

CTestCol2::CTestCol2(LPDIRECT3DDEVICE9 pGraphicDev)
    :Engine::CGameObject(pGraphicDev)
{
}

CTestCol2::~CTestCol2()
{
}

HRESULT CTestCol2::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    m_pTransformCom->m_vScale = { 10.f, 10.f, 10.f };

    SetObjectType(OBJ_TYPE::PUSH_ABLE);

    LoadTextureFromFile(m_pGraphicDev,
        "../Bin/Resource/Texture/puzzle/Sprite_CrystalPuzzle_Gray.png",
        &m_Texture);
    return S_OK;
}

void CTestCol2::LateReady_GameObject()
{
    Engine::CGameObject::LateReady_GameObject();
    CTransform* PlayerTrasform = static_cast<Engine::CTransform*>(
        m_CPlayer->Get_Component(ID_DYNAMIC, L"Com_Transform"));
    _vec3 PlayerPos;
    PlayerTrasform->Get_Info(INFO_POS, &PlayerPos);
    PlayerPos.x -= 30;
    PlayerPos.z += 30;
    m_pTransformCom->Set_Pos(PlayerPos);
}

_int CTestCol2::Update_GameObject(const _float& fTimeDelta)
{
    Add_RenderGroup(RENDER_ALPHA, this);

    return Engine::CGameObject::Update_GameObject(fTimeDelta);
}

void CTestCol2::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CTestCol2::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pGraphicDev->SetTexture(0, m_Texture);
    m_pBufferCom->Render_Buffer();
    ////m_pBoundBox->Render_Buffer();

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);  // 이거 설정안해주면 안됨 전역적으로 장치세팅이 저장되기 때문에
    m_pGraphicDev->SetTexture(0, NULL);  // 이거 설정안해주면 그대로 텍스처 나옴 이것도 마찬가지로 전역적으로 장치세팅이 되므로
}

void CTestCol2::OnCollision(CGameObject* _pOther)
{
    if (_pOther->GetObjectKey() == L"Player")
    {
        /*CPlayer* obj = dynamic_cast<CPlayer*>(_pOther);

        if (obj->IsInvincible())
            return;

        dynamic_cast<CStateController*>(obj->Get_Component(
            ID_DYNAMIC, L"Com_State"))->ChangeState(PlayerHurt::GetInstance(), obj);*/


    }
}

HRESULT CTestCol2::Add_Component()
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

}

CTestCol2* CTestCol2::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CTestCol2* pTestCol2 = new CTestCol2(pGraphicDev);

    if (FAILED(pTestCol2->Ready_GameObject()))
    {
        Safe_Release(pTestCol2);
        MSG_BOX("pTestCol2 Create Failed");
        return nullptr;
    }

    return pTestCol2;
}

void CTestCol2::Free()
{
    Engine::CGameObject::Free();
}
