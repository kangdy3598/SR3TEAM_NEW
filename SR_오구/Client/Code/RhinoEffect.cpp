#include "pch.h"
#include "RhinoEffect.h"
#include "ResMgr.h" 
#include "Export_Utility.h"

CRhinoEffect::CRhinoEffect(LPDIRECT3DDEVICE9 pGraphicDev)
    :Engine::CGameObject(pGraphicDev)
    , name(L"")
    , m_bEffect(false)
{
}

CRhinoEffect::~CRhinoEffect()
{
}

HRESULT CRhinoEffect::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    IDirect3DTexture9* pEffectTexture1 = CResMgr::GetInstance()->GetEffectTexture()[30];

    // D3DXCreateTextureFromFile 제거하고, 리소스 매니저에서 텍스처를 가져오는 방식으로 변경
    //m_pTex = CResMgr::GetInstance()->GetEffectTexture()[30];
    m_pTex = pEffectTexture1;

    // 애니메이션 추가
    m_pAnimatorCom->CreateAnimation(L"RhinoDust", m_pTex, _vec2(0.f, 0.f), _vec2(128.f, 128.f), _vec2(128.f, 128.f), 0.25f, 3);
    //m_pAnimatorCom->CreateAnimation(L"RhinoDust", pEffectTexture1, _vec2(0.f, 0.f), _vec2(128.f, 128.f), _vec2(128.f, 128.f), 0.1f, 7);
    m_pAnimatorCom->Play(L"RhinoDust", false);

    return S_OK;
}

_int CRhinoEffect::Update_GameObject(const _float& fTimeDelta)
{
    Add_RenderGroup(RENDER_TRANSLUCNET, this);
    return Engine::CGameObject::Update_GameObject(fTimeDelta);
}

void CRhinoEffect::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CRhinoEffect::Render_GameObject()
{

    //m_pGraphicDev->SetTexture(0, m_pTex);
    //m_pAnimatorCom->Play(L"RhinoDust", false);

    // 애니메이션이 끝났는지 확인하고, 끝나면 객체를 제거
    if (m_pAnimatorCom->GetAnimation()->IsFinish())
    {
        Engine::CGameObject::Free();
        map<const _tchar*, CLayer*>& mapLayer = CManagement::GetInstance()->GetCurScenePtr()->GetLayerMapPtr();

        CLayer* pLayer = nullptr;
        for (auto iter = mapLayer.begin(); iter != mapLayer.end(); ++iter)
        {
            const _tchar* layerKey = iter->first;

            //if (_tcscmp(layerKey, _T("Layer_GameLogic")) == 0)
            if (lstrcmp(layerKey, L"Layer_GameLogic") == 0)
            {
                pLayer = iter->second;
                //break;
            }
        }
        map<const _tchar*, CGameObject*>& pMap = pLayer->GetLayerGameObjectPtr();
        auto iter = find_if(pMap.begin(), pMap.end(), CTag_Finder(name.c_str()));

        if (iter != pMap.end())
        {
            pMap.erase(iter);
        }

        return;
    }
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    //m_pGraphicDev->SetTexture(0, m_pTex);
    m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetEffectTexture()[30]);
    m_pAnimatorCom->render();
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CRhinoEffect::Add_Component()
{
    CComponent* pComponent = NULL;

    // 기본적인 컴포넌트 추가
    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pAnimatorCom = dynamic_cast<CAnimator2*>(Engine::Clone_Proto(L"Proto_Animator"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_AniBuffer", pComponent });

    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });
    m_pTransformCom->m_vScale = { 50.f, 50.f, 10.f };

    pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Attack_Area"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

    return S_OK;
}

CRhinoEffect* CRhinoEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CRhinoEffect* pRhinoEffect = new CRhinoEffect(pGraphicDev);

    if (FAILED(pRhinoEffect->Ready_GameObject()))
    {
        Safe_Release(pRhinoEffect);
        MSG_BOX("RhinoEffect Create Failed");
        return nullptr;
    }

    return pRhinoEffect;
}

void CRhinoEffect::Free()
{
    Engine::CGameObject::Free();
}