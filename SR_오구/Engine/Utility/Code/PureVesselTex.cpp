#include "PureVesselTex.h"

CPureVesselTex::CPureVesselTex()
{
}

CPureVesselTex::CPureVesselTex(LPDIRECT3DDEVICE9 pGraphicDev)
    :CVIBuffer(pGraphicDev)
    ,m_pPos(nullptr)
{
}

CPureVesselTex::CPureVesselTex(const CPureVesselTex& rhs)
    :CVIBuffer(rhs)
    , m_fH(rhs.m_fH)
    , m_iH(rhs.m_iH)
    , m_hFile(rhs.m_hFile)
    , m_pPos(rhs.m_pPos)
{
}

CPureVesselTex::~CPureVesselTex()
{
}

HRESULT CPureVesselTex::Ready_Buffer(const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv)
{
    return E_NOTIMPL;
}

void CPureVesselTex::Render_Buffer()
{
}

CPureVesselTex* CPureVesselTex::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv)
{
    return nullptr;
}

CComponent* CPureVesselTex::Clone()
{
    return nullptr;
}

void CPureVesselTex::Free()
{
}
