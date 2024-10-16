#include "pch.h"
#include "Audience.h"

CAudience::CAudience(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
{
}

CAudience::~CAudience()
{
}

HRESULT CAudience::Ready_GameObject()
{
	return S_OK;
}

void CAudience::LateReady_GameObject()
{
}

_int CAudience::Update_GameObject(const _float& fTimeDelta)
{
	return 0;
}

void CAudience::LateUpdate_GameObject(const _float& fTimeDelta)
{

}

void CAudience::Render_GameObject()
{
}

HRESULT CAudience::Add_Component()
{
	return S_OK;

}

void CAudience::Free()
{
    Engine::CGameObject::Free();
}
