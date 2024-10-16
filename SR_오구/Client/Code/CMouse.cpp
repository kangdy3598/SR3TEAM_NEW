#include "pch.h"
#include "CMouse.h"

_bool CMouse::g_bQuestClear(false);

CMouse::CMouse(LPDIRECT3DDEVICE9 pGraphicDev)
    :CQuestNPC(pGraphicDev)
{
}

CMouse::~CMouse()
{
}

HRESULT CMouse::Ready_GameObject()
{
    CQuestNPC::Ready_GameObject();

    SetObjectType(OBJ_TYPE::TALK_ABLE);

    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    D3DXCreateTextureFromFile(m_pGraphicDev, L"../Bin/Resource/Texture/NPC/Mouse.png", &m_pNPCTex);
    m_pAnimatorCom->CreateAnimation(L"Mouse", m_pNPCTex, _vec2(0.f, 0.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.12f, 1);

    m_tInfo.pName = L"쥐 박사";
    m_tInfo.pContent = L"자네! 약해도 너무 약해보이는데? 마침 실험체도 필요했으니 내가 개발한 신기술을 전수 받을 생각이 있나?";

    m_tQuestInfo.pQuestTitle = L"쥐 박사의 신기술";
    m_tQuestInfo.pQuestContent = L"내가 약해보인다며 신기술을 배워보라고 한다. 약한 건 맞지만 실험체라니! ";

    _vec3 vMarkPos = m_pTransformCom->m_vInfo[INFO_POS];
    vMarkPos.y += 30.f;
    m_pMarkTransformCom->m_vInfo[INFO_POS] = vMarkPos;
    m_pMarkTransformCom->m_vScale = { 15.f, 17.f, 20.f };

    return S_OK;
}

void CMouse::LateReady_GameObject()
{
    CQuestNPC::LateReady_GameObject();
}

_int CMouse::Update_GameObject(const _float& fTimeDelta)
{
    CQuestNPC::Update_GameObject(fTimeDelta);

    _int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

    if (!g_bQuestClear && m_bQuestAccept)
    {
         m_bQuestSucess = true;
    }
    Add_RenderGroup(RENDER_ALPHA, this);

    return iExit;

}

void CMouse::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CMouse::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());


    m_pGraphicDev->SetTexture(0, m_pNPCTex);
    m_pAnimatorCom->Play(L"Mouse", true);
    m_pAnimatorCom->render();
    //m_pColliderCom->Render_Buffer();

    if (!g_bQuestClear)
    {
        m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pMarkTransformCom->Get_WorldMatrix());

        if (!m_bQuestAccept)
        {
            m_pGraphicDev->SetTexture(0, m_pTex[EXCLAMATION]);
            m_pMarkAnimatorCom->Play(L"ExclamMarkAnim", true);
            m_pMarkAnimatorCom->render();
            return;
        }
        if (m_bQuestSucess)
        {
            m_pGraphicDev->SetTexture(0, m_pTex[YELLOWQUEST]);
            m_pMarkAnimatorCom->Play(L"YellowQuestAnim", true);
            m_pMarkAnimatorCom->render();
            return;
        }

        m_pGraphicDev->SetTexture(0, m_pTex[GRAYQUEST]);
        m_pMarkAnimatorCom->Play(L"GrayQuestAnim", true);
        m_pMarkAnimatorCom->render();

    }
}

void CMouse::OnCollision(CGameObject* _pOther)
{
    if (_pOther->GetObjectKey() != L"Player")
        return;

    //m_bEnter = true;

    // [S]버튼 출력.
    // [S]버튼 클릭 시 텍스트박스 출력 + 텍스트 출력
    if (Engine::GetKeyDown(DIK_S))
    {
        m_bConversation = m_bConversation ? false : true;
        // 대화중일때 S를 누르면 대화종료
        // 대화중이 아닐때 S를 누르면 대화시작
        if (!m_bConversation)
        {
            if (!m_bQuestAccept)
            {
                m_bConversation = m_bConversation ? false : true;
                // 여기에 new퀘스트 UI 띄우기
                m_bQuestAccept = true; // 조건 문 뒤에 true로 바꿔줌.
                m_pQuestUI->Add_Quest(m_tQuestInfo);

                m_pQuestAcceptUI->CallQuestAcceptUI(true);
                m_pQuestAcceptUI->Set_Text(m_tQuestInfo);
                //m_pInterButton->CallButton(true); // 대화중이 아닐 때 버튼 활성화

                return;
            }
        }

        if (m_bConversation)
        {
            m_pInterButton->CallButton(false); // 대화 중일 경우 버튼 출력이 필요 없음!!!!

            m_pTextBox->Set_Text(m_tInfo); //대화창 텍스트 세팅
            m_pTextBox->CallTextBox(true); //대화창 호출

            // 최초에는 기본 퀘스트 말풍을 보여줘야해서 아이템을 가지고 있더라도 false 상태로 출력하기 위해
            //if (!m_bQuestClear && m_bQuestAccept)

            if (g_bQuestClear)
            {
                m_tInfo.pContent = L"내가 전수해준 신기술은 어떻던가?";
                m_pTextBox->Set_Text(m_tInfo); //대화창 텍스트 세팅
                return;
            }

            if (m_bQuestSucess)
            {
                Engine::Play_Sound(L"SFX_446_QuestClear.wav", SOUND_EFFECT, 1.f);

                g_bQuestClear = true;
                m_tInfo.pContent = L"역시 약한건 싫은가 보군. ??키를 눌러 스킬을 사용해보게!";
                m_pTextBox->Set_Text(m_tInfo); //대화창 텍스트 세팅
                m_pQuestUI->Get_QuestArray()->pop_back();
            }
        }

    }

    if (!m_bConversation)
    {
        m_pInterButton->CallButton(true); // 대화중이 아닐 때 버튼 활성화
        // 대화하기[S]
    }
}

void CMouse::OnCollisionEnter(CGameObject* _pOther)
{
}

void CMouse::OnCollisionExit(CGameObject* _pOther)
{
    m_pInterButton->CallButton(true); // 대화중이 아닐 때 버튼 활성화
}

HRESULT CMouse::Add_Component()
{
    CComponent* pComponent = NULL;

    pComponent = m_pAnimatorCom = dynamic_cast<CAnimator2*>(Engine::Clone_Proto(L"Proto_Animator"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Ani_Buffer", pComponent });

    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });
    m_pTransformCom->m_vScale = { 10.f, 10.f, 15.f };
    m_pTransformCom->Set_Pos(1163.f, 25.f, 1350.f);

    pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_pColliderCom->SetGameObjectPtr(this);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", pComponent });


    return S_OK;

}

CMouse* CMouse::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CMouse* pNPC = new CMouse(pGraphicDev);

    if (FAILED(pNPC->Ready_GameObject()))
    {
        Safe_Release(pNPC);
        MSG_BOX("CMouse Create Failed");
        return nullptr;
    }

    return pNPC;
}

void CMouse::Free()
{
    Engine::CGameObject::Free();
}
