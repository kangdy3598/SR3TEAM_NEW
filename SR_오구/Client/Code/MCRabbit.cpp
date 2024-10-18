#include "pch.h"
#include "MCRabbit.h"
#include "FightUI.h"
#include "VictoryUI.h"



CMCRabbit::CMCRabbit(LPDIRECT3DDEVICE9 pGraphicDev)
    :CQuestNPC(pGraphicDev), m_pRhino(nullptr)
{
}

CMCRabbit::~CMCRabbit()
{
}

HRESULT CMCRabbit::Ready_GameObject()
{
    CQuestNPC::Ready_GameObject();

    SetObjectType(OBJ_TYPE::TALK_ABLE);
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    m_eMonster = NUM_END;

    D3DXCreateTextureFromFile(m_pGraphicDev, L"../Bin/Resource/Texture/NPC/RabbitNPC.png", &m_pNPCTex);
    m_pAnimatorCom->CreateAnimation(L"NPCRabbit", m_pNPCTex, _vec2(0.f, 0.f), _vec2(128.f, 128.f), _vec2(128.f, 0.f), 0.15f, 1);

    m_tInfo.pName = L"중계를 즐기는 토끼";
    m_tInfo.pContent = L"아직 전투 상대를 선택하시지 않았군여!!!                  선택 후에 다시 말걸어주세여!!!!!!!!!!!";

    return S_OK;
}

void CMCRabbit::LateReady_GameObject()
{
    CQuestNPC::LateReady_GameObject();

    m_pRhino = dynamic_cast<CRhino*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Rhino"));
    NULL_CHECK_RETURN(m_pRhino);

}

_int CMCRabbit::Update_GameObject(const _float& fTimeDelta)
{
    CQuestNPC::Update_GameObject(fTimeDelta);

    _int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

    if (m_iDefeatCount >= 4)
    {
        Set_Victory();
        m_iDefeatCount = 0;
    }

    Add_RenderGroup(RENDER_ALPHA, this);

    return iExit;
}

void CMCRabbit::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CMCRabbit::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetTexture(0, m_pNPCTex);
    m_pAnimatorCom->Play(L"NPCRabbit", true);
    m_pAnimatorCom->render();
    m_pColliderCom->Render_Buffer();
}

void CMCRabbit::OnCollision(CGameObject* _pOther)
{
    if (_pOther->GetObjectKey() != L"Player")
        return;

    if (Engine::GetKeyDown(DIK_S))
    {
        m_bConversation = m_bConversation ? false : true;

        if (m_bConversation)
        {
            m_pInterButton->CallButton(false); // 대화 중일 경우 버튼 출력이 필요 없음!!!!

            if (m_eMonster != NUM_END)
                m_tInfo.pContent = L"선택한 상대와 결투를 시작합니다!!!!!!!!!!!!!!!!        거절은 거절한다!!!!";

            m_pTextBox->Set_Text(m_tInfo); //대화창 텍스트 세팅
            m_pTextBox->CallTextBox(true); //대화창 호출
        }

        if (!m_bConversation)
        {
            if (m_eMonster != NUM_END)
            {
                Create_Monster();
                CFightUI* pFightUI = dynamic_cast<CFightUI*>(Engine::Get_GameObject(L"Layer_UI", L"Fight_UI"));
                NULL_CHECK_RETURN(pFightUI);
                pFightUI->CallFight();
            }
        }

    }

    if (!m_bConversation)
    {
        m_pInterButton->CallButton(true); // 대화중이 아닐 때 버튼 활성화
        // 대화하기[S]
    }

}

void CMCRabbit::OnCollisionExit(CGameObject* _pOther)
{
    m_pInterButton->CallButton(false);
}

void CMCRabbit::Set_Victory()
{
    m_pPlayer->SetPlayerPos(_vec3(500.f, 20.f, 800.f));

    CVictoryUI* pUI = dynamic_cast<CVictoryUI*>(Engine::Get_GameObject(L"Layer_UI", L"Victory_UI"));
    NULL_CHECK_RETURN(pUI, );
    pUI->CallVictory();
}

HRESULT CMCRabbit::Add_Component()
{
    CComponent* pComponent = NULL;

    pComponent = m_pAnimatorCom = dynamic_cast<CAnimator2*>(Engine::Clone_Proto(L"Proto_Animator"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Ani_Buffer", pComponent });

    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });
    m_pTransformCom->m_vScale = { 20.f, 20.f, 20.f };
    m_pTransformCom->Set_Pos(500.f, 20.f, 800.f);

    pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_pColliderCom->SetGameObjectPtr(this);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", pComponent });


    return S_OK;

}

void CMCRabbit::Create_Monster()
{
    _vec3 vPlayerPos = { 500.f, 16.f, 300.f };

    switch (m_eMonster)
    {
    case RHINO:
        m_pRhino->Set_Create();
        m_pRhino->LateReady_GameObject();

        m_pPlayer->SetPlayerPos(vPlayerPos);
        m_pPlayer->GetCamera()->DirectMoveToPlayer();
        break;
    case MOTH_MAGE:

        for (int i = 0; i < 4; i++)
        {
            _tchar* objectName = new _tchar[32];
            swprintf(objectName, 32, L"MonsterMothMage%d", i);

            CGameObject* pGameObject = CMonsterMothMage::Create(m_pGraphicDev);
            NULL_CHECK_RETURN(pGameObject, );

            CLayer* pLayer = Get_Layer(L"Layer_GameLogic");
            FAILED_CHECK_RETURN(pLayer->Add_GameObject(objectName, pGameObject), );

            dynamic_cast<CMonsterMothMage*>(pGameObject)->testNum = i;
            dynamic_cast<CMonsterMothMage*>(pGameObject)->SetMCRabbit(this);
            dynamic_cast<CMonster*>(pGameObject)->GetLayer(pLayer);
            CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::MONSTER, pGameObject);
            dynamic_cast<CMonster*>(pGameObject)->LateReady_GameObject();

            
            switch (i)
            {
            case 0:
                dynamic_cast<CTransform*>(
                    pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform")
                    )->Set_Pos(_vec3(340.f, 30.f, 460.f));
                break;
            case 1:
                dynamic_cast<CTransform*>(
                    pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform")
                    )->Set_Pos(_vec3(420.f, 30.f, 540.f));
                break;
            case 2:
                dynamic_cast<CTransform*>(
                    pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform")
                    )->Set_Pos(_vec3(580.f, 30.f, 540.f));
                break;
            case 3:
                dynamic_cast<CTransform*>(
                    pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform")
                    )->Set_Pos(_vec3(560.f, 30.f, 460.f));
                break;
            }


        }
        m_pPlayer->SetPlayerPos(vPlayerPos);
        m_pPlayer->GetCamera()->DirectMoveToPlayer();
        break;
    default:
        m_tInfo.pContent = L"아직 대전 상대를 고르지 않았군여?!?!?!!?!??! 선택 후 다시 말 걸어주세여!!!!!";
        break;
    }
}

CMCRabbit* CMCRabbit::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CMCRabbit* pNPC = new CMCRabbit(pGraphicDev);

    if (FAILED(pNPC->Ready_GameObject()))
    {
        Safe_Release(pNPC);
        MSG_BOX("MCRabbit Create Failed");
        return nullptr;
    }

    return pNPC;
}

void CMCRabbit::Free()
{
    Engine::CGameObject::Free();
}
