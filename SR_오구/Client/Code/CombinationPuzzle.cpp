#include "pch.h"
#include "CombinationPuzzle.h"
#include "CombinationStatue.h"
#include "StoneBlock.h"
#include "StoneBlockHole.h"
#include "Export_Utility.h"

CCombinationPuzzle::CCombinationPuzzle(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CCombinationPuzzle::~CCombinationPuzzle()
{
}

HRESULT CCombinationPuzzle::Ready_GameObject()
{
	m_iStatueCount = 0;
	return S_OK;
}

_int CCombinationPuzzle::Update_GameObject(const _float& fTimeDelta)	
{	
	m_vecCombination.push_back(1);
	m_vecCombination.push_back(0);
	m_vecCombination.push_back(2);

	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);	

	return iExit;
}

void CCombinationPuzzle::LateUpdate_GameObject(const _float& fTimeDelta)
{
}

void CCombinationPuzzle::Render_GameObject()
{
}

CCombinationPuzzle* CCombinationPuzzle::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCombinationPuzzle* pCombinationPuzzle = new CCombinationPuzzle(pGraphicDev);

	if (FAILED(pCombinationPuzzle->Ready_GameObject()))
	{
		Safe_Release(pCombinationPuzzle);
		MSG_BOX("pPipeBoard Create Failed");
		return nullptr;
	}

	return pCombinationPuzzle;
}

void CCombinationPuzzle::Check_Matched()
{
	for (int i = 0; i < m_iStatueCount; ++i) {
		if (static_cast<CCombinationStatue*>(m_vecStatues[i])->Get_ImageID() != m_vecCombination[i])
			return;
	}		

	for (int i = 0; i < m_vecStoneBlocks.size(); ++i)
	{
		static_cast<CStoneBlock*>(m_vecStoneBlocks[i])->Move_StoneBlock();	
		Play_Sound(L"SFX_168_GateOnceOff.wav", SOUND_EFFECT, 1.f);
	}
}

void CCombinationPuzzle::Free()
{
	Engine::CGameObject::Free();
}
