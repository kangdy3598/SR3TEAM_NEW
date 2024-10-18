#include "pch.h"
#include "MonsterMothMage.h"

#include "Define.h"
#include "Export_System.h"
#include "MCRabbit.h"

CMonsterMothMage::CMonsterMothMage(LPDIRECT3DDEVICE9 pGraphicDev)
    :CMonster(pGraphicDev)
    , m_HPBar(nullptr)

{
}

CMonsterMothMage::~CMonsterMothMage()
{
}

HRESULT CMonsterMothMage::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    m_pTransformCom->m_vScale = { 20.f, 20.f, 20.f };

    int type = OBJ_TYPE::DESTROY_ABLE + OBJ_TYPE::HURT_ABLE;
    SetObjectType(type);

    SetIdleAnimation();
    SetAttackAnimation();


    m_tMonsterHP.iCurHP = 3;
    m_tMonsterHP.iMaxHP = 3;
    m_fMoveSpeed = 30.f;


    return S_OK;
}

void CMonsterMothMage::LateReady_GameObject()
{
    Engine::CGameObject::LateReady_GameObject();

    m_CPlayer = dynamic_cast<CPlayer*>(
        Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));

    CGameObject* HPBar = CMonsterHPUI::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(HPBar, );

    _tchar* objectName = new _tchar[32];
    swprintf(objectName, 32, L"MothMonsterHP_UI%d", testNum);
    FAILED_CHECK_RETURN(m_pLayer->Add_GameObject(objectName, HPBar), );
    static_cast<CMonsterHPUI*>(HPBar)->SetMonster(this);

    /////////////////////////////////////////////////////////////////////////////
    m_pMothMageOrb = CMonsterMothMageOrb::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(m_pMothMageOrb, );
    objectName = new _tchar[32];
    swprintf(objectName, 32, L"MonsterMothMageOrb%d", testNum);
    FAILED_CHECK_RETURN(m_pLayer->Add_GameObject(objectName, m_pMothMageOrb), );

    (dynamic_cast<CMonsterMothMageOrb*>(m_pMothMageOrb))->SetMothMage(this);
    (dynamic_cast<CMonsterMothMageOrb*>(m_pMothMageOrb))->SetTextureCom(m_pTextureCom);
    m_pMothMageOrb->LateReady_GameObject();

}

_int CMonsterMothMage::Update_GameObject(const _float& fTimeDelta)
{
    if (!m_activation)
        return 0;

    if (Get_Layer(L"Layer_GameLogic")->GetGameState() == GAMESTATE_NONE)
    {
        if (m_tMonsterHP.iCurHP == 0)
        {
            Engine::Play_Sound(L"SFX_92_MonsterMothMage_Death.wav", SOUND_EFFECT, 0.7);
            if (m_pMCRabbit) m_pMCRabbit->AddDefeatCount();
            m_activation = false;
            m_pMothMageOrb->SetActivation(false);
            return 0;
        }

        DurationInvincible(fTimeDelta);



        Engine::CGameObject::Update_GameObject(fTimeDelta);
    }

    Add_RenderGroup(RENDER_ALPHA, this);

    return 0;
}

void CMonsterMothMage::LateUpdate_GameObject(const _float& fTimeDelta)
{
    if (!m_activation)
        return;
    if (Get_Layer(L"Layer_GameLogic")->GetGameState() == GAMESTATE_NONE)
    {
        if (CheckPlayerDistance())
            AttackPlayer();

        else if(CheckPlayerChase())
            MoveToPlayer(fTimeDelta);

        KnockBack(fTimeDelta, m_vKnockBackDir);
        Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
    }
}

void CMonsterMothMage::Render_GameObject()
{
    if (!m_activation)
        return;

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    if (m_bIsAttack)
        m_pTextureCom->Set_Texture(1);
    else
        m_pTextureCom->Set_Texture(0);

    switch (m_iDirIndex)
    {
    case 0:
        if (m_bIsAttack)
            m_pAnimatorCom->Play(L"AttackFront", false);
        else
            m_pAnimatorCom->Play(L"MoveFront", true);
        break;
    case 1:
        if (m_bIsAttack)
            m_pAnimatorCom->Play(L"AttackFrontRight", false);
        else
            m_pAnimatorCom->Play(L"MoveFrontRight", true);
        break;
    case 2:
        if (m_bIsAttack)
            m_pAnimatorCom->Play(L"AttackRight", false);
        else
            m_pAnimatorCom->Play(L"MoveRight", true);
        break;
    case 3:
        if (m_bIsAttack) m_pAnimatorCom->Play(L"AttackBackRight", false);
        else m_pAnimatorCom->Play(L"MoveBackRight", true);
        break;
    case 4:
        if (m_bIsAttack) m_pAnimatorCom->Play(L"AttackBack", false);
        else m_pAnimatorCom->Play(L"MoveBack", true);
        break;
    case 5:
        if (m_bIsAttack) m_pAnimatorCom->Play(L"AttackBackLeft", false);
        else m_pAnimatorCom->Play(L"MoveBackLeft", true);
        break;
    case 6:
        if (m_bIsAttack) m_pAnimatorCom->Play(L"AttackLeft", false);
        else m_pAnimatorCom->Play(L"MoveLeft", true);
        break;
    case 7:
        if (m_bIsAttack) m_pAnimatorCom->Play(L"AttackFrontLeft", false);
        else m_pAnimatorCom->Play(L"MoveFrontLeft", true);
        break;
    default:
        break;
    }
    m_pAnimatorCom->render();

    //m_pTextureCom->Set_Texture(MONSTER);
    //m_pAnimationCom->Render_Buffer();
    if (!m_bInvincible)
        //m_pBoundBox->Render_Buffer();

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);  // 이거 설정안해주면 안됨 전역적으로 장치세팅이 저장되기 때문에
    m_pGraphicDev->SetTexture(0, NULL);  // 이거 설정안해주면 그대로 텍스처 나옴 이것도 마찬가지로 전역적으로 장치세팅이 되므로

}

void CMonsterMothMage::OnCollision(CGameObject* _pOther)
{
    if (!m_activation)
        return;

    if (_pOther->Get_Tag() == TAG_PLAYERATTACK)
    {
        SetMonsterCurHP(-1);
    }

    if (_pOther->GetObjectKey() != L"PlayerInteractionBox")
        return;

    m_CPlayer = dynamic_cast<CPlayerInteractionBox*>(_pOther)->GetPlayer();
    if (m_CPlayer->GetSwingTrigger() && !m_bInvincible)
    {
        m_vKnockBackDir = m_CPlayer->GetPlayerDirVector2();
        m_bKnockBackTrigger = true;
        m_bInvincible = true;
        SetMonsterCurHP(-1);

        Engine::Play_Sound(L"SFX_92_MonsterMothMage_Hit.wav", SOUND_EFFECT, 0.7);
    }


}

HRESULT CMonsterMothMage::Add_Component()
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

    /*pComponent = m_pAnimationCom = dynamic_cast<CAnimation*>(Engine::Clone_Proto(L"Proto_PlayerAnim"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Animation", pComponent });*/

    pComponent = m_pAnimatorCom = dynamic_cast<CAnimator2*>(Engine::Clone_Proto(L"Proto_Animator"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Ani_Buffer", pComponent });

    pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_MonsterMothMage"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Texture", pComponent });
}

void CMonsterMothMage::SetIdleAnimation()
{
    vector<IDirect3DBaseTexture9*> textComp = m_pTextureCom->Get_Texture();

    m_pAnimatorCom->CreateAnimation(L"MoveFront",
        (IDirect3DTexture9*)textComp[0], _vec2(0.f, 0.f),
        _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.2f, 5);

    m_pAnimatorCom->CreateAnimation(L"MoveFrontLeft",
        (IDirect3DTexture9*)textComp[0], _vec2(0.f, 256.f),
        _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.2f, 5);

    m_pAnimatorCom->CreateAnimation(L"MoveLeft",
        (IDirect3DTexture9*)textComp[0], _vec2(0.f, 256.f * 2),
        _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.2f, 5);

    m_pAnimatorCom->CreateAnimation(L"MoveBackLeft",
        (IDirect3DTexture9*)textComp[0], _vec2(0.f, 256.f * 3),
        _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.2f, 5);

    m_pAnimatorCom->CreateAnimation(L"MoveBack",
        (IDirect3DTexture9*)textComp[0], _vec2(0.f, 256.f * 4),
        _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.2f, 5);

    m_pAnimatorCom->CreateAnimation(L"MoveBackRight",
        (IDirect3DTexture9*)textComp[0], _vec2(0.f, 256.f * 5),
        _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.2f, 5);

    m_pAnimatorCom->CreateAnimation(L"MoveRight",
        (IDirect3DTexture9*)textComp[0], _vec2(0.f, 256.f * 6),
        _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.2f, 5);

    m_pAnimatorCom->CreateAnimation(L"MoveFrontRight",
        (IDirect3DTexture9*)textComp[0], _vec2(0.f, 256.f * 7),
        _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.2f, 5);

}

void CMonsterMothMage::SetAttackAnimation()
{
    vector<IDirect3DBaseTexture9*> textComp = m_pTextureCom->Get_Texture();

    m_pAnimatorCom->CreateAnimation(L"AttackFront",
        (IDirect3DTexture9*)textComp[0], _vec2(0.f, 0.f),
        _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.2f, 5);

    m_pAnimatorCom->CreateAnimation(L"AttackFrontLeft",
        (IDirect3DTexture9*)textComp[0], _vec2(0.f, 256.f),
        _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.2f, 5);

    m_pAnimatorCom->CreateAnimation(L"AttackLeft",
        (IDirect3DTexture9*)textComp[0], _vec2(0.f, 256.f * 2),
        _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.2f, 5);

    m_pAnimatorCom->CreateAnimation(L"AttackBackLeft",
        (IDirect3DTexture9*)textComp[0], _vec2(0.f, 256.f * 3),
        _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.2f, 5);

    m_pAnimatorCom->CreateAnimation(L"AttackBack",
        (IDirect3DTexture9*)textComp[0], _vec2(0.f, 256.f * 4),
        _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.2f, 5);

    m_pAnimatorCom->CreateAnimation(L"AttackBackRight",
        (IDirect3DTexture9*)textComp[0], _vec2(0.f, 256.f * 5),
        _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.2f, 5);

    m_pAnimatorCom->CreateAnimation(L"AttackRight",
        (IDirect3DTexture9*)textComp[0], _vec2(0.f, 256.f * 6),
        _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.2f, 5);

    m_pAnimatorCom->CreateAnimation(L"AttackFrontRight",
        (IDirect3DTexture9*)textComp[0], _vec2(0.f, 256.f * 7),
        _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.2f, 5);

}

_bool CMonsterMothMage::CheckPlayerDistance()
{
    if (m_bIsAttack)
        return true;

    _vec3 monsterPos;
    m_pTransformCom->Get_Info(INFO_POS, &monsterPos);

    _vec3 playerPos;
    dynamic_cast<CTransform*>(
        m_CPlayer->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Get_Info(INFO_POS, &playerPos);

    _vec3 dir = playerPos - monsterPos;
    _float distance = D3DXVec3Length(&dir);

    if (distance < 50)
        m_bIsAttack = true;
    else
        m_bIsAttack = false;

    return m_bIsAttack;
}

_bool CMonsterMothMage::CheckPlayerChase()
{
    _bool bReturn = false;
    _vec3 monsterPos;
    m_pTransformCom->Get_Info(INFO_POS, &monsterPos);

    _vec3 playerPos;
    dynamic_cast<CTransform*>(
        m_CPlayer->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Get_Info(INFO_POS, &playerPos);

    _vec3 dir = playerPos - monsterPos;
    _float distance = D3DXVec3Length(&dir);

    if (distance < 400)
        return true;
    else
        return false;

    return false;

}

void CMonsterMothMage::AttackPlayer()
{
    if (m_pAnimatorCom->GetAnimation()->IsFinish())
    {
        m_bIsAttack = false;
        m_bIsShot = false;
        m_pAnimatorCom->GetAnimation()->SetFrame(0);
        return;
    }
    if (m_pAnimatorCom->GetAnimation()->GetCurrentFrm() == 4 && !m_bIsShot)
    {
        Engine::Play_Sound(L"SFX_92_MonsterMothMage_Shoot.wav", SOUND_EFFECT, 0.7);

        m_bIsShot = true;
        bulletCount++;
        _tchar* objectName = new _tchar[32];
        swprintf(objectName, 32, L"MonsterMothMageProj%d", bulletCount);

        CGameObject* pGameObject = CMonsterMothMageBullet::Create(m_pGraphicDev);
        NULL_CHECK_RETURN(pGameObject, );
        FAILED_CHECK_RETURN(m_pLayer->Add_GameObject(objectName, pGameObject), );

        _vec3 m_pMothMageOrbPos;
        dynamic_cast<CTransform*>(
            m_pMothMageOrb->Get_Component(ID_DYNAMIC, L"Com_Transform")
            )->Get_Info(INFO_POS, &m_pMothMageOrbPos);

        dynamic_cast<CTransform*>(
            pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform")
            )->Set_Pos(m_pMothMageOrbPos);

        _vec3 playerPos;
        dynamic_cast<CTransform*>(
            m_CPlayer->Get_Component(ID_DYNAMIC, L"Com_Transform")
            )->Get_Info(INFO_POS, &playerPos);

        dynamic_cast<CMonsterMothMageBullet*>(pGameObject)->SetShotDir(playerPos - m_pMothMageOrbPos);
        (dynamic_cast<CMonsterMothMageBullet*>(pGameObject))->SetTextureCom(m_pTextureCom);
        pGameObject->LateReady_GameObject();
    }
}

CMonsterMothMage* CMonsterMothMage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CMonsterMothMage* pMonster = new CMonsterMothMage(pGraphicDev);

    if (FAILED(pMonster->Ready_GameObject()))
    {
        Safe_Release(pMonster);
        MSG_BOX("Monster Create Failed");
        return nullptr;
    }

    return pMonster;
}

void CMonsterMothMage::Free()
{
    Engine::CGameObject::Free();
}
