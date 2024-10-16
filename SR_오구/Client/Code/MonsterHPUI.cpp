#include "pch.h"
#include "Monster.h"
#include "MonsterHPUI.h"

CMonsterHPUI::CMonsterHPUI(LPDIRECT3DDEVICE9 pGraphicDev)
    :Engine::CGameObject(pGraphicDev)
    , m_pPlayer(nullptr)
    , m_pMonster(nullptr)
    , m_bRenderHP(false)
{
}

CMonsterHPUI::~CMonsterHPUI()
{
}

HRESULT CMonsterHPUI::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    LateReady_GameObject();
    return S_OK;

}

void CMonsterHPUI::LateReady_GameObject()
{
    m_pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
    NULL_CHECK_RETURN(m_pPlayer, );

  /*  m_pTransformCom[HPFRAME] = static_cast<Engine::CTransform*>(
        Get_Component(ID_DYNAMIC, L"Com_TransformFrame"));

    m_pTransformCom[HPBAR] = static_cast<Engine::CTransform*>(
        Get_Component(ID_DYNAMIC, L"Com_TransformHPBar"));

    m_pTransformCom[HPBACK] = static_cast<Engine::CTransform*>(
        Get_Component(ID_DYNAMIC, L"Com_TransformHPBack"));*/

    m_HPBarGaugeScaleX = m_pTransformCom[HPBAR]->m_vScale.x;
}

_int CMonsterHPUI::Update_GameObject(const _float& fTimeDelta)
{
    if (!m_pMonster->GetActivation())
        return 0;
    _int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

    UpdateHPGauge();
    
    Engine::Add_RenderGroup(RENDER_ALPHA, this);

    return iExit;
}

void CMonsterHPUI::LateUpdate_GameObject(const _float& fTimeDelta)
{
    if (!m_pMonster->GetActivation())
        return;

    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CMonsterHPUI::Render_GameObject()
{
    if (!m_pMonster->GetActivation())
        return;

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[HPBACK]->Get_WorldMatrix());
    m_pTextureCom[HPBACK]->Set_Texture();
    m_pBufferCom->Render_Buffer();

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[HPBAR]->Get_WorldMatrix());
    m_pTextureCom[HPBAR]->Set_Texture();
    m_pBufferCom->Render_Buffer();

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[HPFRAME]->Get_WorldMatrix());
    m_pTextureCom[HPFRAME]->Set_Texture();
    m_pBufferCom->Render_Buffer();

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
    m_pGraphicDev->SetTexture(0, NULL);
}

HRESULT CMonsterHPUI::Add_Component()
{
    CComponent* pComponent = NULL;

    // 렉트텍스쳐
    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });


    //HPFRAME
    pComponent = m_pTextureCom[HPFRAME] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_HPFrame"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TextureFrame", pComponent });

    pComponent = m_pTransformCom[HPFRAME] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TransformFrame", pComponent });
    m_pTransformCom[HPFRAME]->m_vScale = { 20.f, 5.f, 1.f }; 


    //HPBAR
    pComponent = m_pTextureCom[HPBAR] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_HPBar"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TextureHPBar", pComponent });

    pComponent = m_pTransformCom[HPBAR] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TransformHPBar", pComponent });
    m_pTransformCom[HPBAR]->m_vScale = { 15.f, 4.f, 1.f };


    //HPBACK
    pComponent = m_pTextureCom[HPBACK] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_HPBarBack"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TextureHPBack", pComponent });

    pComponent = m_pTransformCom[HPBACK] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TransformHPBack", pComponent });
    m_pTransformCom[HPBACK]->m_vScale = { 15.f, 4.f, 1.f };

    return S_OK;
}

void CMonsterHPUI::UpdateHPGauge()
{
    _vec3 monsterPos;
    dynamic_cast<CTransform*>(
        m_pMonster->Get_Component(ID_DYNAMIC, L"Com_Transform")
        )->Get_Info(INFO_POS, &monsterPos);
    monsterPos.y += 20;
    m_pTransformCom[HPFRAME]->Set_Pos(monsterPos);

    _vec3 FramePos;
    m_pTransformCom[HPFRAME]->Get_Info(INFO_POS, &FramePos);
    FramePos.x += 3.6f;
    FramePos.z += 0.1f;
    m_pTransformCom[HPBACK]->Set_Pos(FramePos);

    int m_num = dynamic_cast<CMonster*>(m_pMonster)->GetMonsterHP().iMaxHP;
    int c_num = dynamic_cast<CMonster*>(m_pMonster)->GetMonsterHP().iCurHP;

    m_pTransformCom[HPFRAME]->Get_Info(INFO_POS, &FramePos);
    m_pTransformCom[HPBAR]->m_vScale.x = m_HPBarGaugeScaleX * ((float)c_num / m_num);
    FramePos.x -= (30 * (1 - (float)c_num / m_num)) / 2;

    FramePos.x += 3.6f;
    FramePos.z += 0.1f;
    m_pTransformCom[HPBAR]->Set_Pos(FramePos);
}

CMonsterHPUI* CMonsterHPUI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CMonsterHPUI* pUI = new CMonsterHPUI(pGraphicDev);

    if (FAILED(pUI->Ready_GameObject()))
    {
        Safe_Release(pUI);
        MSG_BOX("UI Create Failed");
        return nullptr;
    }

    return pUI;
}

void CMonsterHPUI::Free()
{
    Engine::CGameObject::Free();
}
