#include "StateController.h"
#include "State.h"

CStateController::CStateController()
{
}

CStateController::CStateController(LPDIRECT3DDEVICE9 pGraphicDev)
    :CComponent(pGraphicDev)
    , m_pState(nullptr)
{
}

CStateController::CStateController(const CStateController& rhs)
    :CComponent(rhs)
    , m_pState(nullptr)
{

    //m_pState = rhs.m_pState;
    //m_pState->AddRef();
}

CStateController::~CStateController()
{
}

HRESULT CStateController::Ready_State()
{
    return S_OK;
}

_int CStateController::Update_Component(const _float& fTimeDelta)
{
    m_pState->Update(fTimeDelta);


    return 0;
}

void CStateController::LateUpdate_Component()
{
}

void CStateController::ChangeState(CState* _nextState, CGameObject* _ob)
{
    if (m_pState != nullptr)
        m_pState->Exit();

    m_pState = _nextState;

    if (!m_pState->m_CGameObject)
        m_pState->m_CGameObject = _ob;

    m_pState->Enter();
}


CStateController* CStateController::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CStateController* pState = new CStateController(pGraphicDev);

    if (FAILED(pState->Ready_State()))
    {
        Safe_Release(pState);
        MSG_BOX("State Create Failed");
        return nullptr;
    }

    return pState;
}

CComponent* CStateController::Clone()
{
    return new CStateController(*this);
}

void CStateController::Free()
{
    CComponent::Free();
}
