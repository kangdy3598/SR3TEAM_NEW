#include "pch.h"
#include "MatchPuzzle.h"
#include "StonePedestal.h"
#include "ColorStone.h"
#include "StoneBlock.h"
#include "Export_Utility.h"

CMatchPuzzle::CMatchPuzzle(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CPuzzleObject(pGraphicDev)
{
}

CMatchPuzzle::~CMatchPuzzle()
{
}

HRESULT CMatchPuzzle::Ready_GameObject()
{	
	return S_OK;
}

_int CMatchPuzzle::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	return iExit;
}

void CMatchPuzzle::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CMatchPuzzle::Render_GameObject()
{
}

void CMatchPuzzle::Puzzle_Clear()
{
	for (auto& iter : m_vecStoneBlocks)
	{
		static_cast<CStoneBlock*>(iter)->Move_StoneBlock();
		Play_Sound(L"SFX_168_GateOnceOff.wav", SOUND_EFFECT, 1.f);
	}
}

CMatchPuzzle* CMatchPuzzle::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMatchPuzzle* pCrystal = new CMatchPuzzle(pGraphicDev);

	if (FAILED(pCrystal->Ready_GameObject()))
	{
		Safe_Release(pCrystal);
		MSG_BOX("pPipeBoard Create Failed");
		return nullptr;
	}

	return pCrystal;
}

void CMatchPuzzle::Free()
{
	Engine::CGameObject::Free();
}

void CMatchPuzzle::Match_Puzzle()
{
	for (int i = 0; i < m_vecPedestals.size(); ++i)
	{
		if (static_cast<CStonePedestal*>(m_vecPedestals[i])->Get_Stone() == nullptr)
			return;

		if (static_cast<CColorStone*>(static_cast<CStonePedestal*>(m_vecPedestals[i])->Get_Stone())->Get_ImageID() != m_vecCombinations[i]) {			
			return;
		}
	}

	Puzzle_Clear();
}
