#include "PuzzleObject.h"

CPuzzleObject::CPuzzleObject(LPDIRECT3DDEVICE9 pGraphicDev)
    :CGameObject(pGraphicDev)
{
}

CPuzzleObject::~CPuzzleObject()
{
}

HRESULT CPuzzleObject::Ready_GameObject()
{
    return E_NOTIMPL;
}

_int CPuzzleObject::Update_GameObject(const _float& fTimeDelta)
{
    return _int();
}

void CPuzzleObject::LateUpdate_GameObject(const _float& fTimeDelta)
{
}

void CPuzzleObject::Match_Puzzle()
{
}

void CPuzzleObject::Free()
{
}

void CPuzzleObject::Render_GameObject()
{
}
