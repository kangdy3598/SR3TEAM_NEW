#include "pch.h"
#include "ExpressMonkey.h"

_bool CExpressMonkey::g_bQuestClear(false);
_bool CExpressMonkey::g_bQuestAccept(false);

CExpressMonkey::CExpressMonkey(LPDIRECT3DDEVICE9 pGraphicDev)
    :CQuestNPC(pGraphicDev), m_bEnter(false)
{

}

CExpressMonkey::~CExpressMonkey()
{

}

HRESULT CExpressMonkey::Ready_GameObject()
{
    CQuestNPC::Ready_GameObject();

    SetObjectType(OBJ_TYPE::TALK_ABLE);

    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    D3DXCreateTextureFromFile(m_pGraphicDev, L"../Bin/Resource/Texture/NPC/MonkeyCome.png", &m_pComeTex);
    m_pAnimatorCom->CreateAnimation(L"MonkeyEnter", m_pComeTex, _vec2(0.f, 0.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.12f, 7);

    D3DXCreateTextureFromFile(m_pGraphicDev, L"../Bin/Resource/Texture/NPC/Monkey.png", &m_pNPCTex);
    m_pAnimatorCom->CreateAnimation(L"MonkeyIDLE", m_pNPCTex, _vec2(0.f, 0.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.12f, 7);

    m_tInfo.pName = L"숭숭 익스프레스";
    m_tInfo.pContent = L"익스프레스 이용권이 없으면 태워줄 수 없어요. 이용권을 가지고 다시 말 걸어주세요.";

    m_tQuestInfo.pQuestTitle = L"숭숭 익스프레스 이용권 찾기";
    m_tQuestInfo.pQuestContent = L"숭숭 익스프레스 이용권이 없으면 숭숭 익스프레스를 이용할 수 없다고?! 이용권을 찾아서 다시 오자!";

    _vec3 vMarkPos = m_pTransformCom->m_vInfo[INFO_POS];
    vMarkPos.y += 35.f;
    m_pMarkTransformCom->m_vInfo[INFO_POS] = vMarkPos;


    return S_OK;
}

void CExpressMonkey::LateReady_GameObject()
{
    CQuestNPC::LateReady_GameObject();
}

_int CExpressMonkey::Update_GameObject(const _float& fTimeDelta)
{
    CQuestNPC::Update_GameObject(fTimeDelta);

    _int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

    if (!g_bQuestClear && g_bQuestAccept)
    {
        if (m_pInven->Find_Item(CItem::QUEST, CItem::EXTICKET))
        {
            m_bQuestSucess = true;
        }
    }
    Add_RenderGroup(RENDER_ALPHA, this);

    return iExit;
}

void CExpressMonkey::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CExpressMonkey::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

    if (!m_bEnter)
    {
        m_pGraphicDev->SetTexture(0, m_pNPCTex);
        m_pAnimatorCom->Play(L"MonkeyIDLE", true);
    }
    else
    {
        m_pGraphicDev->SetTexture(0, m_pComeTex);
        m_pAnimatorCom->Play(L"MonkeyEnter", true);
    }

    m_pAnimatorCom->render();
    //m_pColliderCom->Render_Buffer();

    //m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pBalloonTransformCom->Get_WorldMatrix());
    //m_pTextureCom->Set_Texture();
    //m_pBufferCom->Render_Buffer();

    if (!g_bQuestClear)
    {
        m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pMarkTransformCom->Get_WorldMatrix());

        if (!g_bQuestAccept)
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

void CExpressMonkey::OnCollision(CGameObject* _pOther)
{
    if (_pOther->GetObjectKey() != L"Player")
        return;

    m_bEnter = true;

    // [S]버튼 출력.
    // [S]버튼 클릭 시 텍스트박스 출력 + 텍스트 출력
    if (Engine::GetKeyDown(DIK_S))
    {
        m_bConversation = m_bConversation ? false : true;
        // 대화중일때 S를 누르면 대화종료
        // 대화중이 아닐때 S를 누르면 대화시작
        if (!m_bConversation)
        {
            if (!g_bQuestAccept)
            {
                m_bConversation = m_bConversation ? false : true;
                // 여기에 new퀘스트 UI 띄우기
                g_bQuestAccept = true; // 조건 문 뒤에 true로 바꿔줌.
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
                m_tInfo.pContent = L"숭숭 익스프레스로 원하는 곳으로 어디든 바로 이동 해봐요!!!";
                m_pTextBox->Set_Text(m_tInfo); //대화창 텍스트 세팅
                return;
            }

            if (m_bQuestSucess)
            {
                Engine::Play_Sound(L"SFX_446_QuestClear.wav", SOUND_EFFECT, 1.5f);

                g_bQuestClear = true;
                m_tInfo.pContent = L"오..이용권을 가지고 계시는군요............... 숭숭!!! 어디로 이동할까요??";
                m_pTextBox->Set_Text(m_tInfo); //대화창 텍스트 세팅
                m_pQuestUI->Get_QuestArray()->pop_back();
            }
        }

    }

    if (!m_bConversation)
    {
        m_bEnter = false;
        m_pInterButton->CallButton(true); // 대화중이 아닐 때 버튼 활성화
        // 대화하기[S]
    }

}



void CExpressMonkey::OnCollisionEnter(CGameObject* _pOther)
{
    // 풍선커지기 + 반가워하는 포즈 애니메이션
}

void CExpressMonkey::OnCollisionExit(CGameObject* _pOther)
{
    m_pInterButton->CallButton(false);
}



HRESULT CExpressMonkey::Add_Component()
{
    CComponent* pComponent = NULL;


    //pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    //NULL_CHECK_RETURN(pComponent, E_FAIL);
    //m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

    //pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Balloon"));
    //NULL_CHECK_RETURN(pComponent, E_FAIL);
    //m_mapComponent[ID_STATIC].insert({ L"Com_TextureBalloon",pComponent });

    //pComponent = m_pBalloonTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    //NULL_CHECK_RETURN(pComponent, E_FAIL);
    //m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });
    //m_pBalloonTransformCom->m_vScale = { 30.f, 35.f, 1.f };
    //m_pBalloonTransformCom->Set_Pos(1030.f, 32.5f, 1005.f);


    pComponent = m_pAnimatorCom = dynamic_cast<CAnimator2*>(Engine::Clone_Proto(L"Proto_Animator"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Ani_Buffer", pComponent });

    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });
    m_pTransformCom->m_vScale = { 30.f, 30.f, 30.f };
    m_pTransformCom->Set_Pos(1030.f, 30.f, 1000.f);

    pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_pColliderCom->SetGameObjectPtr(this);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", pComponent });


    return S_OK;
}

CExpressMonkey* CExpressMonkey::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CExpressMonkey* pNPC = new CExpressMonkey(pGraphicDev);

    if (FAILED(pNPC->Ready_GameObject()))
    {
        Safe_Release(pNPC);
        MSG_BOX("ExpressMonkey Create Failed");
        return nullptr;
    }

    return pNPC;
}

void CExpressMonkey::Free()
{
    Engine::CGameObject::Free();
}
