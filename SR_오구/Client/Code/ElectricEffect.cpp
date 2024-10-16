#include "pch.h"
#include "ElectricEffect.h"
#include "Export_Utility.h"
#include "ResMgr.h" 

CElectricEffect::CElectricEffect(LPDIRECT3DDEVICE9 pGraphicDev)
    :Engine::CGameObject(pGraphicDev)
    , name(L"")
    , m_bEffect(false)
    , m_iCount(0)
{
    
}

CElectricEffect::~CElectricEffect()
{
}

HRESULT CElectricEffect::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    IDirect3DTexture9* pEffectTexture1 = CResMgr::GetInstance()->GetEffectTexture()[0];
   
    
    m_pAnimatorCom->CreateAnimation(L"Lighting_Y", pEffectTexture1, _vec2(0.f, 0.f), _vec2(512.f, 1024.f), _vec2(512.f, 0.f), 0.1f, 7);

    SetObjectType(OBJ_TYPE::HURT_ABLE2);     

    //m_pAnimatorCom->Play(L"Lighting_Y",false);

    //m_pTransformCom->Set_Pos(500.f, 120.f, 500.f);
    //m_pTransformCom->m_vScale = { 35.f, 100.f, 20.f }; 

   
    dwtime = GetTickCount64();  
    return S_OK;
}

_int CElectricEffect::Update_GameObject(const _float& fTimeDelta)
{
    if (dwtime + 1000 < GetTickCount64())
    {
        Play_Sound(L"SFX_499_ThunderAttack.wav", SOUND_EFFECT, 1.f);
        m_pAnimatorCom->Play(L"Lighting_Y", false); 
        m_bEffect = true;
        dwtime = GetTickCount64();
    }

    else
    {
        //m_pAnimatorCom->Play(L"Attack_Area", true);
    }

    Add_RenderGroup(RENDER_ALPHA, this);    
    return Engine::CGameObject::Update_GameObject(fTimeDelta);
 
}

void CElectricEffect::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
   
}

void CElectricEffect::Render_GameObject()
{
    
    if (m_bEffect == false) 
    {
        if (m_iCount == 0)
        {
            m_pTransformCom->Rotation(ROT_X, 90 * 3.14f / 180.f);
            m_pTransformCom->m_vScale = { 35.f, 10.f, 20.f };
            m_iCount++;
            
        }

        m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
        m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

        m_pTextureCom->Set_Texture(0);  
        m_pBufferCom->Render_Buffer();  

        //¸ÊÅø ÀÛ¼º½Ã ²ô±â 
        ////m_pBoundBox->Render_Buffer();   
        m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW); 
        return;
    }


    if(m_pAnimatorCom->GetAnimation()->IsFinish())
    {
        
        Engine::CGameObject::Free();
        map<const _tchar*, CLayer*>& mapLayer = CManagement::GetInstance()->GetCurScenePtr()->GetLayerMapPtr();
        //±×´Ï±ñ ·¹ÀÌ¾î¿¡¼­ »èÁ¦ÇØÁà¾ßÇÑ´Ù´Â°Í. 
        CLayer* pLayer = nullptr;

        for (auto iter = mapLayer.begin(); iter != mapLayer.end(); ++iter)
        {
            const _tchar* layerKey = iter->first;

            if (_tcscmp(layerKey, _T("Layer_GameLogic")) == 0)
            {
                pLayer = iter->second;
            }
        }
        map<const _tchar*, CGameObject*>& pMap = pLayer->GetLayerGameObjectPtr();

        auto iter = find_if(pMap.begin(), pMap.end(), CTag_Finder(name.c_str()));
       
        if (iter != pMap.end())
        {
            pMap.erase(iter);
        }


    }
    
    else
    {
        if (m_iCount == 1)
        {
            _vec3 test;
            m_pTransformCom->Get_Info(INFO_POS, &test);
            m_pTransformCom->Set_Pos(test.x, test.y + 99, test.z);
            m_pTransformCom->Rotation(ROT_X, -90 * 3.14f / 180.f);
            m_pTransformCom->m_vScale = { 35.f, 100.f, 20.f };
            m_iCount++;
        }

        m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
        m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


        m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetEffectTexture()[0]);    
        m_pAnimatorCom->render();

        //¸ÊÅø ÀÛ¼º½Ã ²ô±â 
        ////m_pBoundBox->Render_Buffer();
        m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
    }

}


HRESULT CElectricEffect::Add_Component()
{
    CComponent* pComponent = NULL;  

    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex")); 
    NULL_CHECK_RETURN(pComponent, E_FAIL);  
    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });    

    pComponent = m_pAnimatorCom = dynamic_cast<CAnimator2*>(Engine::Clone_Proto(L"Proto_Animator"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

    pComponent = m_pBoundBox = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_pBoundBox->SetGameObjectPtr(this);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", pComponent });

    pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Attack_Area"));        
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });   

  

    return S_OK;
}

CElectricEffect* CElectricEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CElectricEffect* pElectricEffect = new CElectricEffect(pGraphicDev);    

    if (FAILED(pElectricEffect->Ready_GameObject()))
    {
        Safe_Release(pElectricEffect);
        MSG_BOX("pObject Create Failed");
        return nullptr;
    }

    return pElectricEffect; 
}

void CElectricEffect::Free()
{
    Engine::CGameObject::Free();
}

void CElectricEffect::OnCollision(CGameObject* _pOther)
{
    int a = 4;
}
