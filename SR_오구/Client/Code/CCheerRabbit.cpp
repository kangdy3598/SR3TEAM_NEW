#include "pch.h"
#include "CCheerRabbit.h"

CCheerRabbit::CCheerRabbit(LPDIRECT3DDEVICE9 pGraphicDev)
    :CQuestNPC(pGraphicDev)
{
}

CCheerRabbit::~CCheerRabbit()
{
}

HRESULT CCheerRabbit::Ready_GameObject()
{
    CQuestNPC::Ready_GameObject();

    SetObjectType(OBJ_TYPE::TALK_ABLE);
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    D3DXCreateTextureFromFile(m_pGraphicDev, L"../Bin/Resource/Texture/NPC/CheerRabbit.png", &m_pNPCTex);
    m_pAnimatorCom->CreateAnimation(L"JumpRabbit", m_pNPCTex, _vec2(0.f, 0.f), _vec2(128.f, 128.f), _vec2(128.f, 0.f), 0.12f, 3);

    m_tInfo.pName = L"댄싱 토끼";
    m_tInfo.pContent = L"용사만이 이 문을 열 수 있다네!!!! 용사님을 부르는 댄스!!!!!!";

    return S_OK;
}

void CCheerRabbit::LateReady_GameObject()
{
    CQuestNPC::LateReady_GameObject();
}

_int CCheerRabbit::Update_GameObject(const _float& fTimeDelta)
{
    CQuestNPC::Update_GameObject(fTimeDelta);

    _int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

    Add_RenderGroup(RENDER_ALPHA, this);

    return iExit;
}

void CCheerRabbit::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CCheerRabbit::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetTexture(0, m_pNPCTex);
    m_pAnimatorCom->Play(L"JumpRabbit", true);
    m_pAnimatorCom->render();
    //m_pColliderCom->Render_Buffer();
}

void CCheerRabbit::OnCollision(CGameObject* _pOther)
{
    if (_pOther->GetObjectKey() != L"Player")
        return;

    if (Engine::GetKeyDown(DIK_S))
    {
        m_bConversation = m_bConversation ? false : true;

        if (m_bConversation)
        {
            m_pInterButton->CallButton(false); // 대화 중일 경우 버튼 출력이 필요 없음!!!!

            m_pTextBox->Set_Text(m_tInfo); //대화창 텍스트 세팅
            m_pTextBox->CallTextBox(true); //대화창 호출
        }
    }

    if (!m_bConversation)
    {
        m_pInterButton->CallButton(true); // 대화중이 아닐 때 버튼 활성화
        // 대화하기[S]
    }
}

void CCheerRabbit::OnCollisionEnter(CGameObject* _pOther)
{
}

void CCheerRabbit::OnCollisionExit(CGameObject* _pOther)
{
    m_pInterButton->CallButton(false);
}

HRESULT CCheerRabbit::Add_Component()
{
    CComponent* pComponent = NULL;

    pComponent = m_pAnimatorCom = dynamic_cast<CAnimator2*>(Engine::Clone_Proto(L"Proto_Animator"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Ani_Buffer", pComponent });

    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });
    m_pTransformCom->m_vScale = { 15.f, 15.f, 30.f };
    m_pTransformCom->Set_Pos(580.f, 25.f, 1000.f);

    pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_pColliderCom->SetGameObjectPtr(this);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", pComponent });


    return S_OK;
}

CCheerRabbit* CCheerRabbit::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CCheerRabbit* pNPC = new CCheerRabbit(pGraphicDev);

    if (FAILED(pNPC->Ready_GameObject()))
    {
        Safe_Release(pNPC);
        MSG_BOX("CheerRabbit Create Failed");
        return nullptr;
    }

    return pNPC;
}

void CCheerRabbit::Free()
{
    Engine::CGameObject::Free();
}
