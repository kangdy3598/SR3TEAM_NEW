#include "TerrainTex.h"
CTerrainTex::CTerrainTex()
{
}

CTerrainTex::CTerrainTex(LPDIRECT3DDEVICE9 pGraphicDev)
    :CVIBuffer(pGraphicDev)
    ,m_pPos(nullptr)
{
}

CTerrainTex::CTerrainTex(const CTerrainTex& rhs)
    :CVIBuffer(rhs)
    ,m_fH(rhs.m_fH)
    ,m_iH(rhs.m_iH)
    ,m_hFile(rhs.m_hFile)
    ,m_pPos(rhs.m_pPos)
{
}

CTerrainTex::~CTerrainTex()
{
}

HRESULT CTerrainTex::Ready_Buffer(const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv)
{
     
    m_dwVtxCnt = dwCntX * dwCntZ; // dwCntX 는 가로 행의 점의 개수  ,dwCntY는 세로 열의 점의 개수 , dwVtxIty는 점과 점 사이 간격 
    m_dwTriCnt = (dwCntX - 1) * (dwCntZ - 1) * 2;

    m_dwVtxSize = sizeof(VTXTEX);
    m_dwFVF = FVF_TEX;

    m_dwIdxSize = sizeof(INDEX32);
    m_IdxFmt = D3DFMT_INDEX32;

    m_pPos = new _vec3[m_dwVtxCnt];

    FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);


    //==========================================================
    // Height 맵을 위한 코드  ( height 즉 y값 얻기 위해 ) 



    // 높이맵 적용

    m_hFile = CreateFile(L"../Bin/Resource/Texture/Terrain/Height.bmp", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (INVALID_HANDLE_VALUE == m_hFile)
    {
        MSG_BOX("높이맵 텍스처 개방 실패");
    }

    _ulong	dwByte(0);

    ReadFile(m_hFile, &m_fH, sizeof(BITMAPFILEHEADER), &dwByte, NULL);
    ReadFile(m_hFile, &m_iH, sizeof(BITMAPINFOHEADER), &dwByte, NULL);

    _ulong* pPixel = new _ulong[m_iH.biWidth * m_iH.biHeight];

    ReadFile(m_hFile, pPixel, sizeof(_ulong) * m_iH.biWidth * m_iH.biHeight, &dwByte, NULL);

    CloseHandle(m_hFile);

    //=================================================================


    VTXTEX* pVertex = NULL; 
    
    _ulong  dwIndex(0);

    m_pVB->Lock(0, 0, (void**)&pVertex, 0);

    for(_ulong i = 0; i < dwCntX; ++i)
    {
        for(_ulong j = 0 ; j< dwCntZ; ++j)
        {
            dwIndex = i * dwCntX + j;
            
            pVertex[dwIndex].vPosition =
                _vec3(_float(j) * dwVtxItv , _float(pPixel[dwIndex] & 0x000000ff) / 20.f , _float(i) * dwVtxItv);

            //pVertex[dwIndex].vTexUV = _vec2((_float(j) / (dwCntX - 1)),  
            //    (_float(i) / (dwCntZ - 1))); 

            pVertex[dwIndex].vTexUV = _vec2((_float(j) / _float(dwCntX - 1)), (_float(i) / _float(dwCntZ - 1)));    

            m_pPos[dwIndex] = pVertex[dwIndex].vPosition;   
        }
    } // uv 좌표계를 0~1 로 설정해줌 

    Safe_Delete_Array(pPixel);

    INDEX32* pIndex = nullptr;
    _ulong		dwTriCnt(0);

    _vec3		vDst, vSrc, vNormal;

    m_pIB->Lock(0, 0, (void**)&pIndex, 0);

    for (_ulong i = 0; i < dwCntZ - 1; ++i)
    {
        for (_ulong j = 0; j < dwCntX - 1; ++j)
        {
            dwIndex = i * dwCntX + j;

            // 오른쪽 위
            pIndex[dwTriCnt]._0 = dwIndex + dwCntX;
            pIndex[dwTriCnt]._1 = dwIndex + dwCntX + 1;
            pIndex[dwTriCnt]._2 = dwIndex + 1;

            vDst = pVertex[pIndex[dwTriCnt]._1].vPosition - pVertex[pIndex[dwTriCnt]._0].vPosition;
            vSrc = pVertex[pIndex[dwTriCnt]._2].vPosition - pVertex[pIndex[dwTriCnt]._1].vPosition;
            D3DXVec3Cross(&vNormal, &vDst, &vSrc);

            pVertex[pIndex[dwTriCnt]._0].vNormal += vNormal;
            pVertex[pIndex[dwTriCnt]._1].vNormal += vNormal;
            pVertex[pIndex[dwTriCnt]._2].vNormal += vNormal;

            dwTriCnt++;

            // 왼쪽 아래
            pIndex[dwTriCnt]._0 = dwIndex + dwCntX;
            pIndex[dwTriCnt]._1 = dwIndex + 1;
            pIndex[dwTriCnt]._2 = dwIndex;

            vDst = pVertex[pIndex[dwTriCnt]._1].vPosition - pVertex[pIndex[dwTriCnt]._0].vPosition;
            vSrc = pVertex[pIndex[dwTriCnt]._2].vPosition - pVertex[pIndex[dwTriCnt]._1].vPosition;
            D3DXVec3Cross(&vNormal, &vDst, &vSrc);

            pVertex[pIndex[dwTriCnt]._0].vNormal += vNormal;
            pVertex[pIndex[dwTriCnt]._1].vNormal += vNormal;
            pVertex[pIndex[dwTriCnt]._2].vNormal += vNormal;

            dwTriCnt++;
        }
    }

    for (_ulong i = 0; i < m_dwVtxCnt; ++i)
        D3DXVec3Normalize(&pVertex[i].vNormal, &pVertex[i].vNormal);

    m_pVB->Unlock();
    m_pIB->Unlock();

    return S_OK;
}

void CTerrainTex::Render_Buffer()
{
    CVIBuffer::Render_Buffer();
}

CTerrainTex* CTerrainTex::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv)
{
    CTerrainTex* pInstance = new CTerrainTex(pGraphicDev);

    if (FAILED(pInstance->Ready_Buffer(dwCntX, dwCntZ, dwVtxItv)))
    {
        Safe_Release(pInstance);
        MSG_BOX("TerrainTex Create Failed");
        return nullptr;
    }

    return pInstance;
}

CComponent* CTerrainTex::Clone()
{
    return new CTerrainTex(*this);
}

void CTerrainTex::Free()
{
    CVIBuffer::Free();
}


