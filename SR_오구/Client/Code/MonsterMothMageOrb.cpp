#include "pch.h"
#include "MonsterMothMageOrb.h"
#include "Define.h"
#include "Export_System.h"

CMonsterMothMageOrb::CMonsterMothMageOrb(LPDIRECT3DDEVICE9 pGraphicDev)
    :CMonster(pGraphicDev)

{
}

CMonsterMothMageOrb::~CMonsterMothMageOrb()
{
}

HRESULT CMonsterMothMageOrb::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    m_pTransformCom->m_vScale = { 10.f, 10.f, 10.f };

    return S_OK;
}

void CMonsterMothMageOrb::LateReady_GameObject()
{
    Engine::CGameObject::LateReady_GameObject();

}

_int CMonsterMothMageOrb::Update_GameObject(const _float& fTimeDelta)
{
    if (!m_activation)
        return 0;

    if (Get_Layer(L"Layer_GameLogic")->GetGameState() == GAMESTATE_NONE)
    {
        CTransform* mothmageTrasform = static_cast<Engine::CTransform*>(
            m_pMothMage->Get_Component(ID_DYNAMIC, L"Com_Transform"));

        _vec3 mothmagePos;
        mothmageTrasform->Get_Info(INFO_POS, &mothmagePos);
        mothmagePos.y += 30;
        mothmagePos.z -= 5;

        m_pTransformCom->Set_Pos(mothmagePos);
        Engine::CGameObject::Update_GameObject(fTimeDelta);
    }

    Add_RenderGroup(RENDER_ALPHA, this);
    return 0;
}

void CMonsterMothMageOrb::LateUpdate_GameObject(const _float& fTimeDelta)
{
    if (!m_activation)
        return;
    if (Get_Layer(L"Layer_GameLogic")->GetGameState() == GAMESTATE_NONE)
    {
        Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
    }
}

void CMonsterMothMageOrb::Render_GameObject()
{
    if (!m_activation)
        return;

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pTextureCom->Set_Texture(2);
    m_pBufferCom->Render_Buffer();
    
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);  // 이거 설정안해주면 안됨 전역적으로 장치세팅이 저장되기 때문에
    m_pGraphicDev->SetTexture(0, NULL);  // 이거 설정안해주면 그대로 텍스처 나옴 이것도 마찬가지로 전역적으로 장치세팅이 되므로
}

HRESULT CMonsterMothMageOrb::Add_Component()
{
    CComponent* pComponent = NULL;

    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

    /*pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_MonsterMothMage"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Texture", pComponent });*/

}

CMonsterMothMageOrb* CMonsterMothMageOrb::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CMonsterMothMageOrb* pMonster = new CMonsterMothMageOrb(pGraphicDev);

    if (FAILED(pMonster->Ready_GameObject()))
    {
        Safe_Release(pMonster);
        MSG_BOX("Monster Create Failed");
        return nullptr;
    }

    return pMonster;
}

void CMonsterMothMageOrb::Free()
{
    Engine::CGameObject::Free();
}
