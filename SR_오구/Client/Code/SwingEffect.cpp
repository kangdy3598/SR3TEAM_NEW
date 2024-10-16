#include "pch.h"
#include "SwingEffect.h"
#include "Define.h"
#include "Export_System.h"

CSwingEffect::CSwingEffect(LPDIRECT3DDEVICE9 pGraphicDev)
    :Engine::CGameObject(pGraphicDev)
    , m_activation(true)
{
}

CSwingEffect::~CSwingEffect()
{
}

HRESULT CSwingEffect::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    m_pTransformCom->m_vScale = { 30.f, 30.f, 30.f };
    

    vector<IDirect3DBaseTexture9*> textComp = m_pTextureCom->Get_Texture();

    m_pAnimatorCom->CreateAnimation(L"MoveFront",
        (IDirect3DTexture9*)textComp[0], _vec2(0.f, 0.f),
        _vec2(512.f, 512.f), _vec2(512.f, 0.f), 0.2f, 3);

    return S_OK;
}

void CSwingEffect::LateReady_GameObject()
{
}

_int CSwingEffect::Update_GameObject(const _float& fTimeDelta)
{

   // Add_RenderGroup(RENDER_ALPHA, this);


    return Engine::CGameObject::Update_GameObject(fTimeDelta);
}

void CSwingEffect::LateUpdate_GameObject(const _float& fTimeDelta)
{
    _vec3 vPlayerDir = m_CPlayer->GetPlayerDirVector();
    if (vPlayerDir.x != 0 || vPlayerDir.z != 0)
    {
        CTransform* m_playerTransform = dynamic_cast<CTransform*>(Engine::Get_Component(
            ID_DYNAMIC, L"Layer_GameLogic", L"Player", L"Com_Transform"));

        _vec3 pos;
        m_playerTransform->Get_Info(INFO_POS, &pos);
        pos.x += vPlayerDir.x * 15;
        pos.z -= vPlayerDir.z * 15;

        D3DXMATRIX rotationMatrix;
        D3DXMatrixIdentity(&rotationMatrix);

        if (vPlayerDir.x < -0.85)
        {
            
            D3DXMatrixRotationY(&rotationMatrix, 90.f);

        }
        else if (vPlayerDir.x < -0.55)
        {
            if (vPlayerDir.z < 0) D3DXMatrixRotationY(&rotationMatrix, 135.f);
            else D3DXMatrixRotationY(&rotationMatrix, 45.f);
        }
        else if (vPlayerDir.x < 0.55)
        {
            if (vPlayerDir.z < 0) D3DXMatrixRotationY(&rotationMatrix, 180.f);
            else D3DXMatrixRotationY(&rotationMatrix, 0.f);
        }
        else if (vPlayerDir.x < 0.85)
        {
            if (vPlayerDir.z < 0) D3DXMatrixRotationY(&rotationMatrix, 225.f);
            else D3DXMatrixRotationY(&rotationMatrix, 315.f);
        }
        else
            D3DXMatrixRotationY(&rotationMatrix, 270.f);

        m_pTransformCom->Set_Pos(pos);
        m_pGraphicDev->SetTransform(D3DTS_WORLD, &rotationMatrix);
    }

    
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CSwingEffect::Render_GameObject()
{
    if (!m_activation)
        return;

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pTextureCom->Set_Texture();
    m_pAnimatorCom->Play(L"MoveFront", true);
    m_pAnimatorCom->render();
    //m_pBufferCom->Render_Buffer();

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);  // 이거 설정안해주면 안됨 전역적으로 장치세팅이 저장되기 때문에
    m_pGraphicDev->SetTexture(0, NULL);  // 이거 설정안해주면 그대로 텍스처 나옴 이것도 마찬가지로 전역적으로 장치세팅이 되므로

}

HRESULT CSwingEffect::Add_Component()
{
    CComponent* pComponent = NULL;

    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

    pComponent = m_pAnimatorCom = dynamic_cast<CAnimator2*>(Engine::Clone_Proto(L"Proto_Animator"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Ani_Buffer", pComponent });

    pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_SwingEffect1"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Texture", pComponent });
}

CSwingEffect* CSwingEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CSwingEffect* pEffect = new CSwingEffect(pGraphicDev);

    if (FAILED(pEffect->Ready_GameObject()))
    {
        Safe_Release(pEffect);
        MSG_BOX("Monster Create Failed");
        return nullptr;
    }

    return pEffect;
}

void CSwingEffect::Free()
{
    Engine::CGameObject::Free();
}
