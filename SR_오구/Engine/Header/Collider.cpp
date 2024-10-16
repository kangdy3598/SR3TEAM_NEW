#include "Collider.h"
#include "GameObject.h"
#include "Transform.h"

UINT CCollider::g_iNextID = 0;


CCollider::CCollider()
    : m_pMesh(nullptr)
    , m_VecMin(0.f, 0.f, 0.f)
    , m_VecMax(0.f, 0.f, 0.f)
    , m_pGameObjPtr(nullptr)
    , m_bisCollision(false)
    , m_iID(0)
{

}

CCollider::CCollider(LPDIRECT3DDEVICE9 pGraphicDev)
    :CComponent(pGraphicDev)
    , m_pMesh(nullptr)
    , m_VecMin(0.f, 0.f, 0.f)
    , m_VecMax(0.f, 0.f, 0.f)
    , m_pGameObjPtr(nullptr)
    , m_bisCollision(false)
    , m_iID(0)
{

}

CCollider::CCollider(const CCollider& rhs)
    :CComponent(rhs)
    , m_pMesh(rhs.m_pMesh)
    , m_VecMin(rhs.m_VecMin)
    , m_VecMax(rhs.m_VecMax)
    , m_bisCollision(rhs.m_bisCollision)
    , m_iID(g_iNextID++)
{
}

CCollider::~CCollider()
{
}

HRESULT CCollider::Ready_Buffer()
{
    if (FAILED(D3DXCreateBox(m_pGraphicDev, 1.5f, 1.5f, 1.5f, &m_pMesh, NULL))) {
        // 박스 생성 실패 처리
        int a = 4;
    }

    //D3DXVECTOR3* pVertices;

    m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVertices);
    D3DXComputeBoundingBox(pVertices, m_pMesh->GetNumVertices(), m_pMesh->GetNumBytesPerVertex(), &m_VecMin, &m_VecMax);
    m_pMesh->UnlockVertexBuffer();

    return S_OK;
}

void CCollider::Render_Buffer()
{
    if (m_bisCollision == true)
    {
        D3DMATERIAL9 redMaterial;
        ZeroMemory(&redMaterial, sizeof(D3DMATERIAL9));

        // 빨간색으로 설정
        redMaterial.Diffuse.r = 1.0f;
        redMaterial.Diffuse.g = 0.0f;
        redMaterial.Diffuse.b = 0.0f;
        redMaterial.Diffuse.a = 1.0f;

        redMaterial.Ambient = redMaterial.Diffuse;
        redMaterial.Specular = redMaterial.Diffuse;

        // 빨간색 재질 설정
        m_pGraphicDev->SetMaterial(&redMaterial);
        m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

    }

    else
    {
        D3DMATERIAL9 WhiteMaterial;
        ZeroMemory(&WhiteMaterial, sizeof(D3DMATERIAL9));

        // 하얀색으로 설정
        WhiteMaterial.Diffuse.r = 1.0f;
        WhiteMaterial.Diffuse.g = 1.0f;
        WhiteMaterial.Diffuse.b = 1.0f;
        WhiteMaterial.Diffuse.a = 1.0f;

        WhiteMaterial.Ambient = WhiteMaterial.Diffuse;
        WhiteMaterial.Specular = WhiteMaterial.Diffuse;

        // 하얀색 재질 설정
        m_pGraphicDev->SetMaterial(&WhiteMaterial);
        m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
    }

    m_bisCollision = false;


    m_pGraphicDev->SetTexture(0, 0);
    m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
    m_pMesh->DrawSubset(0);
    m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);


}

_int CCollider::Update_Component(const _float& fTimeDelta)
{
    D3DXMATRIX worldmatirx;

    //즉 해당 오브젝트의 Trasnform을 가져와야한다.
    //m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

    //m_pGraphicDev->GetTransform(D3DTS_WORLD, &worldmatirx);
    CTransform* pTransformCom = dynamic_cast<CTransform*>(m_pGameObjPtr->Find_Component(ID_DYNAMIC, L"Com_Transform"));


    m_vWolrdVecMin = { 0.f,0.f,0.f };
    m_vWolrdVecMin = { 0.f,0.f,0.f };

    D3DXVec3TransformCoord(&m_vWolrdVecMin, &m_VecMin, pTransformCom->Get_WorldMatrix());
    D3DXVec3TransformCoord(&m_vWolrdVecMax, &m_VecMax, pTransformCom->Get_WorldMatrix());



    return 0;
}

void CCollider::LateUpdate_Component()
{
}

CCollider* CCollider::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CCollider* pBoundBox = new CCollider(pGraphicDev);

    if (FAILED(pBoundBox->Ready_Buffer()))
    {
        Safe_Release(pBoundBox);
        MSG_BOX("CBoundBox Create Failed");
        return nullptr;
    }

    return pBoundBox;
}

CComponent* CCollider::Clone()
{
    return new CCollider(*this);
}

void CCollider::Free()
{
    CComponent::Free();
}



