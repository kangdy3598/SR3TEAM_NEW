#include "pch.h"
#include "TestCol.h"
#include "Define.h"
#include "Export_System.h"

CTestCol::CTestCol(LPDIRECT3DDEVICE9 pGraphicDev)
    :Engine::CGameObject(pGraphicDev)
{
}

CTestCol::~CTestCol()
{
}

HRESULT CTestCol::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    m_pTransformCom->m_vScale = { 10.f, 10.f, 10.f };

    int type = OBJ_TYPE::LIFT_ABLE + OBJ_TYPE::NOTPASS_ABLE;
    SetObjectType(type);

    LoadTextureFromFile(m_pGraphicDev,
        "../Bin/Resource/Texture/puzzle/Sprite_FirePit.png",
        &m_Texture);
    return S_OK;
}

void CTestCol::LateReady_GameObject()
{
    Engine::CGameObject::LateReady_GameObject();
    CTransform* PlayerTrasform = static_cast<Engine::CTransform*>(
        m_CPlayer->Get_Component(ID_DYNAMIC, L"Com_Transform"));
    _vec3 PlayerPos;
    PlayerTrasform->Get_Info(INFO_POS, &PlayerPos);
    PlayerPos.x += 30;
    PlayerPos.z += 30;
    m_pTransformCom->Set_Pos(PlayerPos);
}

_int CTestCol::Update_GameObject(const _float& fTimeDelta)
{
    Add_RenderGroup(RENDER_ALPHA, this);

    return Engine::CGameObject::Update_GameObject(fTimeDelta);
}

void CTestCol::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CTestCol::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pGraphicDev->SetTexture(0, m_Texture);
    m_pBufferCom->Render_Buffer();
    //////m_pBoundBox->Render_Buffer();

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);  // 이거 설정안해주면 안됨 전역적으로 장치세팅이 저장되기 때문에
    m_pGraphicDev->SetTexture(0, NULL);  // 이거 설정안해주면 그대로 텍스처 나옴 이것도 마찬가지로 전역적으로 장치세팅이 되므로
}

void CTestCol::OnCollision(CGameObject* _pOther)
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

HRESULT CTestCol::Add_Component()
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

CTestCol* CTestCol::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CTestCol* pTestCol = new CTestCol(pGraphicDev);

    if (FAILED(pTestCol->Ready_GameObject()))
    {
        Safe_Release(pTestCol);
        MSG_BOX("pTestCol Create Failed");
        return nullptr;
    }

    return pTestCol;
}

void CTestCol::Free()
{
    Engine::CGameObject::Free();
}
