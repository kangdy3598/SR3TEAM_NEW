#include "pch.h"
#include "PlayerInteractionBox.h"
#include "Define.h"
#include "Export_System.h"

CPlayerInteractionBox::CPlayerInteractionBox(LPDIRECT3DDEVICE9 pGraphicDev)
    :Engine::CGameObject(pGraphicDev)
    
{
}

CPlayerInteractionBox::~CPlayerInteractionBox()
{
}

HRESULT CPlayerInteractionBox::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    m_pTransformCom->m_vScale = { 5.f, 5.f, 5.f };

    m_bDontDestroy = true;
    return S_OK;
}

void CPlayerInteractionBox::LateReady_GameObject()
{
    m_CPlayer->SetInteractionBox(this);
    Engine::CGameObject::LateReady_GameObject();
}

_int CPlayerInteractionBox::Update_GameObject(const _float& fTimeDelta)
{
    Add_RenderGroup(RENDER_ALPHA, this);

    _vec3 vPlayerDir = m_CPlayer->GetPlayerDirVector();
    if ((vPlayerDir.x != 0 || vPlayerDir.z != 0)&& m_CPlayer->m_bSmashEnd == false)
    {
        CTransform* m_playerTransform = dynamic_cast<CTransform*>(Engine::Get_Component(
            ID_DYNAMIC, L"Layer_GameLogic", L"Player", L"Com_Transform"));

        _vec3 pos;
        m_playerTransform->Get_Info(INFO_POS, &pos);
        pos.x += vPlayerDir.x * 15;
        pos.z -= vPlayerDir.z * 15;
        m_pTransformCom->m_vScale = { 5.f, 5.f, 5.f };
        m_pTransformCom->Set_Pos(pos);
    }

    // 10월 11일 코드 추가 
    if(m_CPlayer->m_bSmashEnd ==true)
    {
        CTransform* m_playerTransform = dynamic_cast<CTransform*>(Engine::Get_Component(
            ID_DYNAMIC, L"Layer_GameLogic", L"Player", L"Com_Transform"));

        _vec3 pos;
        m_playerTransform->Get_Info(INFO_POS, &pos);
        m_pTransformCom->m_vScale = { 20.f,20.f,20.f };
        pos.x += vPlayerDir.x * 30; 
        pos.z -= vPlayerDir.z * 30; 

        m_pTransformCom->Set_Pos(pos);

        m_CPlayer->m_bSmashEnd = false;
    }

    

    return Engine::CGameObject::Update_GameObject(fTimeDelta);
}

void CPlayerInteractionBox::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CPlayerInteractionBox::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    ////m_pBoundBox->Render_Buffer();

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);  // 이거 설정안해주면 안됨 전역적으로 장치세팅이 저장되기 때문에
    m_pGraphicDev->SetTexture(0, NULL);  // 이거 설정안해주면 그대로 텍스처 나옴 이것도 마찬가지로 전역적으로 장치세팅이 되므로
}

HRESULT CPlayerInteractionBox::Add_Component()
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

CPlayerInteractionBox* CPlayerInteractionBox::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CPlayerInteractionBox* pinteractionBox = new CPlayerInteractionBox(pGraphicDev);

    if (FAILED(pinteractionBox->Ready_GameObject()))
    {
        Safe_Release(pinteractionBox);
        MSG_BOX("pPlayerInteractionBox Create Failed");
        return nullptr;
    }

    return pinteractionBox;
}

void CPlayerInteractionBox::OnCollisionEnter(CGameObject* _pOther)
{
    if (m_CPlayer->GetPlayerState() == PLAYERSTATE::PLY_ROLLING ||
        m_CPlayer->GetPlayerState() == PLAYERSTATE::PLY_ROLLINGDIAGONAL
        )
        return;

    m_CPlayer->SetInteractingObj(_pOther);
    switch (_pOther->GetObjectType())
    {
    case OBJ_TYPE::PUSH_ABLE:

        if (!m_CPlayer->IsPlayerDiagonal())
            m_CPlayer->SetPushTrigger(true);

        break;
    }

    if(m_CPlayer->GetPlayerState() == PLAYERSTATE::PLY_SMASH)
    {
        _vec3 test = m_CPlayer->GetPlayerDirVector2();
        dynamic_cast<CTransform*>(_pOther->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Move_Pos(&test, CTimerMgr::GetInstance()->Get_TimeDelta(L"Timer_FPS60"), 100.f);
    }
}

void CPlayerInteractionBox::OnCollision(CGameObject* _pOther)
{

}

void CPlayerInteractionBox::OnCollisionExit(CGameObject* _pOther)
{
    m_CPlayer->SetInteractingObj(nullptr);
    if (_pOther->IncludingType(OBJ_TYPE::PUSH_ABLE))
        m_CPlayer->SetPushTrigger(false);
}

void CPlayerInteractionBox::Free()
{
    Engine::CGameObject::Free();
}
