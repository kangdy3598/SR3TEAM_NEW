#include "pch.h"
#include "MonsterMothMageBullet.h"
#include "Define.h"
#include "Export_System.h"

CMonsterMothMageBullet::CMonsterMothMageBullet(LPDIRECT3DDEVICE9 pGraphicDev)
    :CMonster(pGraphicDev)
    , m_pTexture(nullptr)
{
}

CMonsterMothMageBullet::~CMonsterMothMageBullet()
{
}

HRESULT CMonsterMothMageBullet::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    m_pTransformCom->m_vScale = { 10.f, 10.f, 10.f };

    int type = OBJ_TYPE::HURT_ABLE;
    SetObjectType(type);
    CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(
        GROUP_TYPE::MONSTER, this);


    m_fMoveSpeed = 0.5f;
    m_vShotDir = m_vToPlayerDir;

    return S_OK;
}

void CMonsterMothMageBullet::LateReady_GameObject()
{
    Engine::CGameObject::LateReady_GameObject();

    vector<IDirect3DBaseTexture9*> textComp = m_pTextureCom->Get_Texture();

    m_pAnimatorCom->CreateAnimation(L"Move1",
        (IDirect3DTexture9*)textComp[3], _vec2(0.f, 0.f),
        _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.2f, 3);

    m_pAnimatorCom->CreateAnimation(L"Move2",
        (IDirect3DTexture9*)textComp[3], _vec2(0.f, 0.f),
        _vec2(582.f, 582.f), _vec2(582.f, 0.f), 0.2f, 3);

    m_pAnimatorCom->CreateAnimation(L"Move3",
        (IDirect3DTexture9*)textComp[3], _vec2(0.f, 0.f),
        _vec2(582.f, 582.f), _vec2(582.f, 0.f), 0.2f, 3);
}

_int CMonsterMothMageBullet::Update_GameObject(const _float& fTimeDelta)
{
    if (!m_activation)
        return 0;

    if (Get_Layer(L"Layer_GameLogic")->GetGameState() == GAMESTATE_NONE)
    {

        CTransform* transform =
            dynamic_cast<CTransform*>(Get_Component(ID_DYNAMIC, L"Com_Transform"));
        transform->Move_Pos(&m_vShotDir, fTimeDelta, m_fMoveSpeed);

        Engine::CGameObject::Update_GameObject(fTimeDelta);
    }

    Add_RenderGroup(RENDER_ALPHA, this);
    return 0;
}

void CMonsterMothMageBullet::LateUpdate_GameObject(const _float& fTimeDelta)
{
    if (!m_activation)
        return;
    if (Get_Layer(L"Layer_GameLogic")->GetGameState() == GAMESTATE_NONE)
    {
        Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
    }
}

void CMonsterMothMageBullet::Render_GameObject()
{
    if (!m_activation)
        return;

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pTextureCom->Set_Texture(3);

    m_pAnimatorCom->Play(L"Move1", true);
    m_pAnimatorCom->render();

    //m_pTextureCom->Set_Texture(MONSTER);
    //m_pAnimationCom->Render_Buffer();
    //if (!m_bInvincible)
    //    ////m_pBoundBox->Render_Buffer();

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);  // 이거 설정안해주면 안됨 전역적으로 장치세팅이 저장되기 때문에
    m_pGraphicDev->SetTexture(0, NULL);  // 이거 설정안해주면 그대로 텍스처 나옴 이것도 마찬가지로 전역적으로 장치세팅이 되므로

}

void CMonsterMothMageBullet::OnCollision(CGameObject* _pOther)
{
    if (!m_activation)
        return;

    if (_pOther->GetObjectKey() != L"PlayerInteractionBox")
        return;

    m_CPlayer = dynamic_cast<CPlayerInteractionBox*>(_pOther)->GetPlayer();
    if (m_CPlayer->GetSwingTrigger() && !m_bInvincible)
    {
        //   int a = 10;
    }
}

HRESULT CMonsterMothMageBullet::Add_Component()
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

    pComponent = m_pAnimatorCom = dynamic_cast<CAnimator2*>(Engine::Clone_Proto(L"Proto_Animator"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Ani_Buffer", pComponent });

    /*pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_MonsterMothMageProj"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Texture", pComponent });*/

}

CMonsterMothMageBullet* CMonsterMothMageBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CMonsterMothMageBullet* pMonster = new CMonsterMothMageBullet(pGraphicDev);

    if (FAILED(pMonster->Ready_GameObject()))
    {
        Safe_Release(pMonster);
        MSG_BOX("Monster Create Failed");
        return nullptr;
    }

    return pMonster;
}

void CMonsterMothMageBullet::Free()
{
    Engine::CGameObject::Free();
}
