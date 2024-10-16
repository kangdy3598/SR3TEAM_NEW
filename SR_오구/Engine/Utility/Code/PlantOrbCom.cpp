#include "PlantOrbCom.h"

CPlantOrbCom::CPlantOrbCom()
{
}

CPlantOrbCom::CPlantOrbCom(LPDIRECT3DDEVICE9 pGraphicDev)    
{
}

CPlantOrbCom::CPlantOrbCom(const CPlantOrbCom& rhs)    
{
}

CPlantOrbCom::~CPlantOrbCom()
{
}

HRESULT CPlantOrbCom::Ready_Com()
{
    return S_OK;
}

_int CPlantOrbCom::Update_Component(const _float& fTimeDelta)
{
    return 0;
}

void CPlantOrbCom::LateUpdate_Component()
{
}

CPlantOrbCom* CPlantOrbCom::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CPlantOrbCom* pPipe = new CPlantOrbCom(pGraphicDev);

    if (FAILED(pPipe->Ready_Com()))
    {
        Safe_Release(pPipe);
        MSG_BOX("PipeBoardCom Create Failed");
        return nullptr;
    }

    return pPipe;
}

CComponent* CPlantOrbCom::Clone()
{
    return new CPlantOrbCom(*this);
}
void CPlantOrbCom::Free()
{
    CComponent::Free();
}