#include "pch.h"
#include "Monster.h"
#include "Define.h"
#include "Export_System.h"

CMonster::CMonster(LPDIRECT3DDEVICE9 pGraphicDev)
    :Engine::CGameObject(pGraphicDev)
    , m_vKnockBackDir(_vec3(0.f, 0.f, 0.f))
    , m_bKnockBackTrigger(false)
    , m_bInvincible(false)
    , m_activation(true)
    , m_iDirIndex(0)
{
}

CMonster::~CMonster()
{
}

HRESULT CMonster::Ready_GameObject()
{
    return S_OK;
}

void CMonster::LateReady_GameObject()
{
}

_int CMonster::Update_GameObject(const _float& fTimeDelta)
{



    return Engine::CGameObject::Update_GameObject(fTimeDelta);
}

void CMonster::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CMonster::Render_GameObject()
{

}

void CMonster::OnCollision(CGameObject* _pOther)
{
    if (!m_activation)
        return;
    if (_pOther->GetObjectKey() != L"PlayerInteractionBox")
        return;

    m_CPlayer = dynamic_cast<CPlayerInteractionBox*>(_pOther)->GetPlayer();
    if (m_CPlayer->GetSwingTrigger() && !m_bInvincible)
    {
        m_vKnockBackDir = m_CPlayer->GetPlayerDirVector2();
        m_bKnockBackTrigger = true;
        m_bInvincible = true;
        SetMonsterCurHP(-1);
    }
}

void CMonster::KnockBack(const _float& fTimeDelta, _vec3 vKnockBackDir)
{
    if (!m_activation)
        return;
    if (!m_bKnockBackTrigger)
        return;

    static float distance = 0.f;
    static float speed = 60.f;
    static float tickTime = 0.f;

    if (speed <= 10)
    {
        speed = 60.f;
        distance = 0.f;
        tickTime = 0.f;
        m_vKnockBackDir = { 0.f, 0.f, 0.f };
        m_bKnockBackTrigger = false;
        return;
    }

    tickTime += fTimeDelta;
    if (tickTime >= 0.5f)
    {
        tickTime = 0.f;
        speed *= 0.1f;
    }

    CTransform* transform =
        dynamic_cast<CTransform*>(Get_Component(ID_DYNAMIC, L"Com_Transform"));
    transform->Move_Pos(&m_vKnockBackDir, fTimeDelta, speed);
}

void CMonster::DurationInvincible(const _float& fTimeDelta)
{
    if (!m_bInvincible)
        return;

    static float fDurationTime = 0.f;
    fDurationTime += fTimeDelta;
    if (fDurationTime >= 1.f)
    {
        fDurationTime = 0.f;
        m_bInvincible = false;
    }
}

void CMonster::MoveToPlayer(const _float& fTimeDelta)
{
    _vec3 playerPos;
    dynamic_cast<CTransform*>(
        m_CPlayer->Get_Component(ID_DYNAMIC, L"Com_Transform")
        )->Get_Info(INFO_POS, &playerPos);

    _vec3 monsterPos;
    m_pTransformCom->Get_Info(INFO_POS, &monsterPos);

    m_vToPlayerDir = playerPos - monsterPos;

    _vec3 dir;
    D3DXVec3Normalize(&dir, &m_vToPlayerDir);
    dir.y = 0;

    CTransform* transform =
        dynamic_cast<CTransform*>(Get_Component(ID_DYNAMIC, L"Com_Transform"));
    transform->Move_Pos(&dir, fTimeDelta, m_fMoveSpeed);

    if (dir.x < -0.85)
    {
        m_iDirIndex = 6;
    }
    else if (dir.x < -0.55)
    {
        if (dir.z < 0) m_iDirIndex = 7;
        else m_iDirIndex = 5;
    }
    else if (dir.x < 0.55)
    {
        if (dir.z < 0) m_iDirIndex = 0;
        else m_iDirIndex = 4;
    }
    else if (dir.x < 0.85)
    {
        if (dir.z < 0) m_iDirIndex = 1;
        else m_iDirIndex = 3;
    }
    else
        m_iDirIndex = 2;
}
HRESULT CMonster::Add_Component()
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

CMonster* CMonster::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CMonster* pMonster = new CMonster(pGraphicDev);

    if (FAILED(pMonster->Ready_GameObject()))
    {
        Safe_Release(pMonster);
        MSG_BOX("Monster Create Failed");
        return nullptr;
    }

    return pMonster;
}

void CMonster::Free()
{
    Engine::CGameObject::Free();
}
