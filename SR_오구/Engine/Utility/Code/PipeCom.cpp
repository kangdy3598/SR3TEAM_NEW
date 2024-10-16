#include "PipeCom.h"

CPipeCom::CPipeCom() : m_bIsFixed(false), m_bIsConnected(false)
{
}

CPipeCom::CPipeCom(LPDIRECT3DDEVICE9 pGraphicDev)
    : m_bIsFixed(false), m_bIsConnected(false)
{
}

CPipeCom::CPipeCom(const CPipeCom& rhs)
    : m_bIsFixed(rhs.m_bIsFixed), m_bIsConnected(rhs.m_bIsConnected)
{
}

CPipeCom::~CPipeCom()
{
}

HRESULT CPipeCom::Ready_Pipe()
{
	return S_OK;
}

_int CPipeCom::Update_Component(const _float& fTimeDelta)
{
	return 0;
}

void CPipeCom::LateUpdate_Component()
{
}

CPipeCom* CPipeCom::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CPipeCom* pPipe = new CPipeCom(pGraphicDev);

    if (FAILED(pPipe->Ready_Pipe()))
    {
        Safe_Release(pPipe);
        MSG_BOX("PipeBoardCom Create Failed");
        return nullptr;
    }

    return pPipe;
}

CComponent* CPipeCom::Clone()
{
    return new CPipeCom(*this);
}
void CPipeCom::Free()
{
    CComponent::Free();
}