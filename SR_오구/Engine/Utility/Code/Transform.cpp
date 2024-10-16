#include "Transform.h"

CTransform::CTransform()
    :m_vScale(1.f,1.f,1.f)
    ,m_vAngle(0.f,0.f,0.f)
    ,Rotation_x(0.f)
    ,Rotation_y(0.f)
    ,Rotation_z(0.f)
    ,Scale_x(1.f)
    ,Scale_y(1.f)
    ,Scale_z(1.f)
{
    ZeroMemory(m_vInfo, sizeof(m_vInfo));
    D3DXMatrixIdentity(&m_matWorld);
}

CTransform::CTransform(LPDIRECT3DDEVICE9 pGraphicDev)
    :CComponent(pGraphicDev)
    ,m_vScale(1.f,1.f,1.f)
    ,m_vAngle(0.f,0.f,0.f)
    , Rotation_x(0.f)
    , Rotation_y(0.f)
    , Rotation_z(0.f)
    , Scale_x(1.f)
    , Scale_y(1.f)
    , Scale_z(1.f)
{
    ZeroMemory(m_vInfo, sizeof(m_vInfo));
    D3DXMatrixIdentity(&m_matWorld);
}

CTransform::CTransform(const CTransform& rhs)
    :CComponent(rhs)
    ,m_vScale(rhs.m_vScale)
    ,m_vAngle(rhs.m_vAngle)
    , Rotation_x(rhs.Rotation_x)
    , Rotation_y(rhs.Rotation_y)
    , Rotation_z(rhs.Rotation_z)
    , Scale_x(rhs.Scale_x)
    , Scale_y(rhs.Scale_y)
    , Scale_z(rhs.Scale_z)
{
    for (size_t i = 0; i < INFO_END; ++i)
        m_vInfo[i] = rhs.m_vInfo[i];

    m_matWorld = rhs.m_matWorld;
}

CTransform::~CTransform()
{
}

HRESULT CTransform::Ready_Transform()
{
    
    D3DXMatrixIdentity(&m_matWorld);

    for(_int i=0; i<INFO_END; ++i)
    {
        memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));
    }

    return S_OK;
}

void CTransform::LateReady_Component()
{
    D3DXMatrixIdentity(&m_matWorld);

    for (_int i = 0; i < INFO_POS; ++i)
    {
        memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));
    }

    // 크기 

    for (_int i = 0; i < INFO_POS; ++i)
    {
        D3DXVec3Normalize(&m_vInfo[i], &m_vInfo[i]);
        m_vInfo[i] *= *(((_float*)&m_vScale) + i);
    }

    // 회전 

    _matrix     matRot[ROT_END];

    D3DXMatrixRotationX(&matRot[ROT_X], m_vAngle.x);
    D3DXMatrixRotationY(&matRot[ROT_Y], m_vAngle.y);
    D3DXMatrixRotationZ(&matRot[ROT_Z], m_vAngle.z);


    for (_int i = 0; i < INFO_POS; ++i)
    {
        for (_int j = 0; j < ROT_END; ++j)
        {
            D3DXVec3TransformNormal(&m_vInfo[i], &m_vInfo[i], &matRot[j]);
        }
    }


    for (_int i = 0; i < INFO_END; ++i)
    {
        memcpy(&m_matWorld.m[i][0], &m_vInfo[i], sizeof(_vec3));
    }

}

_int CTransform::Update_Component(const _float& fTimeDelta)
{
   

    D3DXMatrixIdentity(&m_matWorld);

    for(_int i =0; i<INFO_POS; ++i)
    {
        memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));
    }

    // 크기 

    for (_int i = 0; i < INFO_POS; ++i) 
    {
        D3DXVec3Normalize(&m_vInfo[i], &m_vInfo[i]);
        m_vInfo[i] *= *(((_float*)&m_vScale) + i);
    }

    // 회전 

    _matrix     matRot[ROT_END];

    D3DXMatrixRotationX(&matRot[ROT_X], m_vAngle.x);
    D3DXMatrixRotationY(&matRot[ROT_Y], m_vAngle.y);
    D3DXMatrixRotationZ(&matRot[ROT_Z], m_vAngle.z);


    for (_int i = 0; i < INFO_POS; ++i)
    {
        for (_int j = 0; j < ROT_END; ++j)
        {
            D3DXVec3TransformNormal(&m_vInfo[i], &m_vInfo[i], &matRot[j]);
        }
    }


    for (_int i = 0; i < INFO_END; ++i)
    {
        memcpy(&m_matWorld.m[i][0], &m_vInfo[i], sizeof(_vec3));
    }

    return 0;
}

void CTransform::LateUpdate_Component()
{
}


void CTransform::Chase_Target(const _vec3* pTargetPos, const _float& fSpeed)
{
    _vec3  vDir = *pTargetPos - m_vInfo[INFO_POS];

    m_vInfo[INFO_POS] += *D3DXVec3Normalize(&vDir, &vDir) * fSpeed; 

    _matrix  matRot = *Compute_LookAtTarget(pTargetPos);
    
    _matrix  matScale, matTrans; 

    D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
    D3DXMatrixTranslation(&matTrans, m_vInfo[INFO_POS].x, m_vInfo[INFO_POS].y, m_vInfo[INFO_POS].z);
    
    m_matWorld = matScale * matRot * matTrans; 


}

const _matrix* CTransform::Compute_LookAtTarget(const _vec3* pTargetPos)
{
    _vec3       vDir = *pTargetPos - m_vInfo[INFO_POS];
    _vec3       vAxis, vUp;
    _matrix     matRot;


    //RIGHT , UP ,Look 순서 

  /*  D3DXVec3Normalize(&vUp, &m_vInfo[INFO_UP]);
    D3DXVec3Normalize(&vDir, &vDir);

    _float fDot = D3DXVec3Dot(&vUp, &vDir);

    _float fRadian = acos(fDot);

    D3DXMatrixRotationAxis(&matRot, &vAxis, fRadian);*/


    return D3DXMatrixRotationAxis(&matRot
                                    ,D3DXVec3Cross(&vAxis, &m_vInfo[INFO_UP], &vDir)
                                    ,acos(D3DXVec3Dot(D3DXVec3Normalize(&vUp, &m_vInfo[INFO_UP])
                                    ,D3DXVec3Normalize(&vDir, &vDir))));
    
}



CTransform* CTransform::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CTransform* pTransform = new CTransform(pGraphicDev);

    if (FAILED(pTransform->Ready_Transform()))
    {
        Safe_Release(pTransform);
        MSG_BOX("Transform Create Failed");
        return nullptr;
    }

    return pTransform;
}

CComponent* CTransform::Clone()
{
    return new CTransform(*this);
}

void CTransform::Free()
{
    CComponent::Free();
}
