#include "pch.h"
#include "CrystalPuzzle.h"
#include "Export_Utility.h"

CCrystalPuzzle::CCrystalPuzzle(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CCrystalPuzzle::~CCrystalPuzzle()
{
}

HRESULT CCrystalPuzzle::Ready_GameObject()
{
	return S_OK;
}

_int CCrystalPuzzle::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	return iExit;
}

void CCrystalPuzzle::LateUpdate_GameObject(const _float& fTimeDeltsa)
{
}

void CCrystalPuzzle::Render_GameObject()
{
}

CCrystalPuzzle* CCrystalPuzzle::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCrystalPuzzle* pCrystal = new CCrystalPuzzle(pGraphicDev);

	if (FAILED(pCrystal->Ready_GameObject()))
	{
		Safe_Release(pCrystal);
		MSG_BOX("pPipeBoard Create Failed");
		return nullptr;
	}

	return pCrystal;
}

void CCrystalPuzzle::Check_Matched()
{
	int i = 0;
	for (auto& iter : m_vecPressBlocks)
	{
		if (static_cast<CPressBlock*>(iter)->Get_ImageID() != static_cast<CCrystal*>(m_vecCrystals[i])->Get_ImageID()) {			
			return;
		}

		i++;
	}

	for (auto& iter : m_vecCrystals) {
		static_cast<CCrystal*>(iter)->Break();
		Play_Sound(L"SFX_29_CrystalBreak.wav", SOUND_EFFECT, 1.f);
	}		
	for (auto& iter : m_vecPressBlocks) {
		static_cast<CPressBlock*>(iter)->Set_Clear(true);
		static_cast<CPressBlock*>(iter)->Set_ImageID(8);
		static_cast<CPressBlock*>(iter)->Set_Active(false);
	}
}

void CCrystalPuzzle::Free()
{
	Engine::CGameObject::Free();
}