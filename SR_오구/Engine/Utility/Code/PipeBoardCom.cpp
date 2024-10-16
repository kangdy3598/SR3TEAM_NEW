#include "PipeBoardCom.h"

CPipeBoardCom::CPipeBoardCom()
    : m_iCurPipeCount(0), m_pCursor(nullptr), m_pStatingPipe(nullptr), m_bIsPipeSpawned(false), m_iGridSizeX(0), m_iGridSizeZ(0), m_pPickedPipe(nullptr)
{
   
}

CPipeBoardCom::CPipeBoardCom(LPDIRECT3DDEVICE9 pGraphicDev)
    :CComponent(pGraphicDev), m_iCurPipeCount(m_iCurPipeCount), m_pCursor(m_pCursor), m_pPickedPipe(nullptr),
    m_pStatingPipe(m_pStatingPipe), m_bIsPipeSpawned(false), m_iGridSizeX(0), m_iGridSizeZ(0)
{
}

CPipeBoardCom::CPipeBoardCom(const CPipeBoardCom& rhs)
    :CComponent(rhs), m_iCurPipeCount(rhs.m_iCurPipeCount), m_pCursor(rhs.m_pCursor), m_pPickedPipe(rhs.m_pPickedPipe),
    m_pStatingPipe(rhs.m_pStatingPipe), m_bIsPipeSpawned(false), m_iGridSizeX(rhs.m_iGridSizeX), m_iGridSizeZ(rhs.m_iGridSizeZ)
{
}

CPipeBoardCom::~CPipeBoardCom()
{
}

HRESULT CPipeBoardCom::Ready_PipeBoard()
{
	return S_OK;
}

_int CPipeBoardCom::Update_Component(const _float& fTimeDelta)
{
    return 0;
}

void CPipeBoardCom::LateUpdate_Component()
{
}

CPipeBoardCom* CPipeBoardCom::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CPipeBoardCom* pPipeBoard = new CPipeBoardCom(pGraphicDev);

    if (FAILED(pPipeBoard->Ready_PipeBoard()))
    {
        Safe_Release(pPipeBoard);
        MSG_BOX("PipeBoardCom Create Failed");
        return nullptr;
    }

    return pPipeBoard;
}

CComponent* CPipeBoardCom::Clone()
{
    return new CPipeBoardCom(*this);
}

void CPipeBoardCom::Free()
{
    CComponent::Free();
}
