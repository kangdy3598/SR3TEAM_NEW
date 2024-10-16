#include "MapTex.h"

CMapTex::CMapTex()
    :m_pPos(nullptr)
    
{
}

CMapTex::CMapTex(LPDIRECT3DDEVICE9 pGraphicDev)
    :CVIBuffer(pGraphicDev)
    ,m_pPos(nullptr)
   
{
}

CMapTex::CMapTex(const CMapTex& rhs)
    :CVIBuffer(rhs)
    , m_pPos(rhs.m_pPos)
    
{
}

CMapTex::~CMapTex()
{

 
}

HRESULT CMapTex::Ready_Buffer(const _ulong& dwCntX, const _ulong& dwCntZ, const float& dwVtxItvX ,const float& dwVtxItvY)
{
    m_dwVtxCnt = dwCntX * dwCntZ; // dwCntX는 가로 행의 점의 개수, dwCntZ는 세로 열의 점의 개수, dwVtxIty는 점과 점 사이 간격
    m_dwTriCnt = (dwCntX - 1) * (dwCntZ - 1) * 2;

    m_dwVtxSize = sizeof(VTXTEX2);
    m_dwFVF = FVF_TEX2;

    m_dwIdxSize = sizeof(INDEX32);
    m_IdxFmt = D3DFMT_INDEX32;

    m_pPos = new _vec3[m_dwVtxCnt];

    FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);


    //=======================================================
    // 버텍스의 인덱스 값 설정 

    //VTXTEX2* pVertex = NULL;
    m_pVertex = NULL;
    _ulong  dwIndex(0); 

    m_pVB->Lock(0, 0, (void**)&m_pVertex, 0);

    for(_ulong i = 0; i< dwCntX; ++i)
    {
        for(_ulong j =0; j<dwCntZ; ++j)
        {
            dwIndex = i * dwCntX + j;

            m_pVertex[dwIndex].vPosition = _vec3(_float(j) * dwVtxItvX, 0, _float(i) * dwVtxItvY);
            m_pVertex[dwIndex].vTexUV = _vec2((_float(j) / (dwCntX - 1)),
                (_float(i) / (dwCntZ - 1)));

            m_pPos[dwIndex] = m_pVertex[dwIndex].vPosition;
        }
    }

    m_Vec3 = { 1.f,1.f,1.f };
    m_pVB->Unlock();


    // ==================================================
    // 인덱스 버퍼 설정
    INDEX32* pIndex = nullptr; 
    _ulong dwTriCnt(0);

    m_pIB->Lock(0, 0, (void**)&pIndex, 0);
    
    for(_ulong i =0; i < dwCntZ -1; ++i)
    {
        for(_ulong j = 0; j< dwCntX -1 ; ++j)
        {
            dwIndex = i * dwCntX + j; 

            //오른쪽 위 
            pIndex[dwTriCnt]._0 = dwIndex + dwCntX;
            pIndex[dwTriCnt]._1 = dwIndex + dwCntX + 1;
            pIndex[dwTriCnt]._2 = dwIndex + 1; 

            dwTriCnt++; 

            // 왼쪽 아래
            pIndex[dwTriCnt]._0 = dwIndex + dwCntX; 
            pIndex[dwTriCnt]._1 = dwIndex + 1;  
            pIndex[dwTriCnt]._2 = dwIndex;

            dwTriCnt++; 
        }
    }

    m_pIB->Unlock();
   


    return S_OK; 
}

void CMapTex::Render_Buffer()
{
    CVIBuffer::Render_Buffer();
}

CMapTex* CMapTex::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwCntX, const _ulong& dwCntZ, const float& dwVtxItvX, const float& dwVtxItvY)
{
    CMapTex* pInstance = new CMapTex(pGraphicDev);

    if (FAILED(pInstance->Ready_Buffer(dwCntX, dwCntZ, dwVtxItvX,dwVtxItvY)))             
    {
        Safe_Release(pInstance);
        MSG_BOX("TerrainTex Create Failed");
        return nullptr;
    }

    return pInstance;
}

CComponent* CMapTex::Clone()
{
    return new CMapTex(*this);
}

void CMapTex::Free()
{
    CVIBuffer::Free();
}
