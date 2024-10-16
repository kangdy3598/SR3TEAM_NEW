#include "pch.h"
#include "RhinoMaster.h"

CRhinoMaster::CRhinoMaster(LPDIRECT3DDEVICE9 pGraphicDev)
    :CQuestNPC(pGraphicDev)
{
}

CRhinoMaster::~CRhinoMaster()
{
}

HRESULT CRhinoMaster::Ready_GameObject()
{
    CQuestNPC::Ready_GameObject();

    SetObjectType(OBJ_TYPE::TALK_ABLE);
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    D3DXCreateTextureFromFile(m_pGraphicDev, L"../Bin/Resource/Texture/NPC/NoArm.png", &m_pNPCTex);
    m_pAnimatorCom->CreateAnimation(L"NoArm", m_pNPCTex, _vec2(0.f, 0.f), _vec2(128.f, 128.f), _vec2(128.f, 0.f), 0.15f, 3);

    m_tInfo.pName = L"외팔이";
    m_tInfo.pContent = L"내 팔을 앗아간 '거대 사막뿔소'로 대전 상대로 지정 시켜주마.";

    return S_OK;
}

void CRhinoMaster::LateReady_GameObject()
{
    CQuestNPC::LateReady_GameObject();
    m_pRabbit = dynamic_cast<CMCRabbit*>(Engine::Get_GameObject(L"Layer_GameLogic", L"NPCRabbit"));
    NULL_CHECK_RETURN(m_pRabbit);
}

_int CRhinoMaster::Update_GameObject(const _float& fTimeDelta)
{
    CQuestNPC::Update_GameObject(fTimeDelta);

    _int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

    Add_RenderGroup(RENDER_ALPHA, this);

    return iExit;
}

void CRhinoMaster::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CRhinoMaster::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetTexture(0, m_pNPCTex);
    m_pAnimatorCom->Play(L"NoArm", true);
    m_pAnimatorCom->render();
    //m_pColliderCom->Render_Buffer();
}

void CRhinoMaster::OnCollision(CGameObject* _pOther)
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
            m_pRabbit->Set_Monster(CMCRabbit::RHINO);
        }
    }

    if (!m_bConversation)
    {
        m_pInterButton->CallButton(true); // 대화중이 아닐 때 버튼 활성화
        // 대화하기[S]
    }
}

void CRhinoMaster::OnCollisionExit(CGameObject* _pOther)
{
    m_pInterButton->CallButton(false);
}

HRESULT CRhinoMaster::Add_Component()
{
    CComponent* pComponent = NULL;

    pComponent = m_pAnimatorCom = dynamic_cast<CAnimator2*>(Engine::Clone_Proto(L"Proto_Animator"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Ani_Buffer", pComponent });

    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });
    m_pTransformCom->m_vScale = { 20.f, 20.f, 20.f };
    m_pTransformCom->Set_Pos(308.f, 20.f, 805.f);

    pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_pColliderCom->SetGameObjectPtr(this);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", pComponent });


    return S_OK;
}

CRhinoMaster* CRhinoMaster::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CRhinoMaster* pNPC = new CRhinoMaster(pGraphicDev);

    if (FAILED(pNPC->Ready_GameObject()))
    {
        Safe_Release(pNPC);
        MSG_BOX("CRhinoMaster Create Failed");
        return nullptr;
    }

    return pNPC;
}

void CRhinoMaster::Free()
{
    Engine::CGameObject::Free();
}
