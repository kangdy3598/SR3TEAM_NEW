#include "pch.h"
#include "ArenaMonster.h"

CArenaMonster::CArenaMonster(LPDIRECT3DDEVICE9 pGraphicDev)
    :Engine::CGameObject(pGraphicDev), m_bCreate(false)
{
}

CArenaMonster::~CArenaMonster()
{
}

HRESULT CArenaMonster::Ready_GameObject()
{
    return S_OK;
}

void CArenaMonster::LateReady_GameObject()
{
    Engine::CGameObject::LateReady_GameObject();
    m_pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
}

_int CArenaMonster::Update_GameObject(const _float& fTimeDelta)
{
    return Engine::CGameObject::Update_GameObject(fTimeDelta);
}

void CArenaMonster::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CArenaMonster::Render_GameObject()
{
}

HRESULT CArenaMonster::Add_Component()
{
    CComponent* pComponent = NULL;

    pComponent = m_pEffectTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_EffectTransform", pComponent });
   
    pComponent = m_pEffectAnimatorCom = dynamic_cast<CAnimator2*>(Engine::Clone_Proto(L"Proto_Animator"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Ani_Buffer", pComponent });

    return S_OK;
}

void CArenaMonster::Free()
{
    Engine::CGameObject::Free();
}
