#include "pch.h"
#include "BardCat.h"

CBardCat::CBardCat(LPDIRECT3DDEVICE9 pGraphicDev)
    :CQuestNPC(pGraphicDev)
{
}

CBardCat::~CBardCat()
{
}

HRESULT CBardCat::Ready_GameObject()
{
    CQuestNPC::Ready_GameObject();

    SetObjectType(OBJ_TYPE::TALK_ABLE);

    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    D3DXCreateTextureFromFile(m_pGraphicDev, L"../Bin/Resource/Texture/NPC/BardCat.png", &m_pNPCTex);
    m_pAnimatorCom->CreateAnimation(L"BardCatIdle", m_pNPCTex, _vec2(0.f, 0.f), _vec2(128.f, 128.f), _vec2(128.f, 0.f), 0.12f, 5);
    m_pAnimatorCom->CreateAnimation(L"BardCatSong", m_pNPCTex, _vec2(0.f, 128.f), _vec2(128.f, 128.f), _vec2(128.f, 0.f), 0.12f, 7);

    m_tInfo.pName = L"음유시인";
    m_tInfo.pContent = L"저 문을 열고 들어오다니..! 당신이 용사님이시군요!!!!!!! 용사님을 위해 만든 노래가 있어요!";

    m_eState = CAT_IDLE;

    return S_OK;

}

void CBardCat::LateReady_GameObject()
{
    CQuestNPC::LateReady_GameObject();
}

_int CBardCat::Update_GameObject(const _float& fTimeDelta)
{
    CQuestNPC::Update_GameObject(fTimeDelta);

    _int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

    Add_RenderGroup(RENDER_ALPHA, this);

    return iExit;

}

void CBardCat::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CBardCat::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

    switch (m_eState)
    {
    case CAT_IDLE:
        m_pAnimatorCom->Play(L"BardCatIdle", true);
        break;

    case CAT_SONG:
        m_pAnimatorCom->Play(L"BardCatSong", true);
        break;
    }
    m_pGraphicDev->SetTexture(0, m_pNPCTex);
    m_pAnimatorCom->render();
    //m_pColliderCom->Render_Buffer();

}

void CBardCat::OnCollision(CGameObject* _pOther)
{
    if (_pOther->GetObjectKey() != L"Player")
        return;

    // [S]버튼 출력.
    // [S]버튼 클릭 시 텍스트박스 출력 + 텍스트 출력
    if (Engine::GetKeyDown(DIK_S))
    {
        m_bConversation = m_bConversation ? false : true;
        // 대화중일때 S를 누르면 대화종료
        // 대화중이 아닐때 S를 누르면 대화시작

        if (m_bConversation)
        {
            m_pInterButton->CallButton(false); // 대화 중일 경우 버튼 출력이 필요 없음!!!!

            m_pTextBox->Set_Text(m_tInfo); //대화창 텍스트 세팅
            m_pTextBox->CallTextBox(true); //대화창 호출
            //if (m_eState == CAT_SONG)
            //{

            //}
        }
        if (!m_bConversation)
        {
            m_eState = CAT_SONG;
            Engine::StopSound(SOUND_BGM);
            Engine::PlayBGM(L"BGM_81_BardSongWithVocal.wav", 0.8f);
            return;
        }
    }

    if (!m_bConversation)
    {
        m_pInterButton->CallButton(true); // 대화중이 아닐 때 버튼 활성화
        // 대화하기[S]
    }
}

void CBardCat::OnCollisionEnter(CGameObject* _pOther)
{
}

void CBardCat::OnCollisionExit(CGameObject* _pOther)
{
    if (m_eState == CAT_SONG)
    {
        m_eState = CAT_IDLE;
        Engine::StopSound(SOUND_BGM);
        Engine::PlayBGM(L"BGM_3_JungleAreaField1.wav", 0.8f);
    }
    m_pInterButton->CallButton(false);
}

HRESULT CBardCat::Add_Component()
{
    CComponent* pComponent = NULL;

    pComponent = m_pAnimatorCom = dynamic_cast<CAnimator2*>(Engine::Clone_Proto(L"Proto_Animator"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Ani_Buffer", pComponent });

    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });
    m_pTransformCom->m_vScale = { 25.f, 25.f, 30.f };
    m_pTransformCom->Set_Pos(880.f, 30.f, 1880.f);

    pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_pColliderCom->SetGameObjectPtr(this);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", pComponent });


    return S_OK;
}

CBardCat* CBardCat::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CBardCat* pNPC = new CBardCat(pGraphicDev);

    if (FAILED(pNPC->Ready_GameObject()))
    {
        Safe_Release(pNPC);
        MSG_BOX("CBardCat Create Failed");
        return nullptr;
    }

    return pNPC;
}

void CBardCat::Free()
{
    Engine::CGameObject::Free();
}
