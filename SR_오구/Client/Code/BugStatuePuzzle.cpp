#include "pch.h"
#include "BugStatuePuzzle.h"
#include "BugStatue.h"
#include "StoneBlock.h"
#include "StoneBlockHole.h"
#include "Export_Utility.h"

CBugStatuePuzzle::CBugStatuePuzzle(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CPuzzleObject(pGraphicDev)
{
}

CBugStatuePuzzle::~CBugStatuePuzzle()
{
}

HRESULT CBugStatuePuzzle::Ready_GameObject()
{
	return S_OK;
}

_int CBugStatuePuzzle::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	return iExit;
}

void CBugStatuePuzzle::LateUpdate_GameObject(const _float& fTimeDelta)
{
}

void CBugStatuePuzzle::Render_GameObject()
{
}

CBugStatuePuzzle* CBugStatuePuzzle::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBugStatuePuzzle* pStatuePuzzle = new CBugStatuePuzzle(pGraphicDev);

	if (FAILED(pStatuePuzzle->Ready_GameObject()))
	{
		Safe_Release(pStatuePuzzle);
		MSG_BOX("pPipeBoard Create Failed");
		return nullptr;
	}

	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::OBJECT, pStatuePuzzle);
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::PUZZLE, pStatuePuzzle);

	return pStatuePuzzle;
}


void CBugStatuePuzzle::Free()
{
	Engine::CGameObject::Free();
}

void CBugStatuePuzzle::Match_Puzzle()
{
	_bool bClear = true;
	for (int i = 0; i < m_vecStatues.size(); ++i)
	{	
		int temp = static_cast<CBugStatue*>(m_vecStatues[i])->Get_ImageID();

		if (temp != 3 && temp != 5) {
			return;
		}
		else
		{
			if (temp == 1)
				static_cast<CStoneBlock*>(m_vecStoneBlocks[i])->Set_ImageID(2);
			else if (temp == 3)
				static_cast<CStoneBlock*>(m_vecStoneBlocks[i])->Set_ImageID(6);
		}
			
	}

	for (int i = 0; i < m_vecStoneBlocks.size(); ++i)
	{
		static_cast<CStoneBlock*>(m_vecStoneBlocks[i])->Move_StoneBlock();
		Play_Sound(L"SFX_23_StoneGateLightOn.wav", SOUND_EFFECT, 1.f);
		Play_Sound(L"SFX_168_GateOnceOff.wav", SOUND_SURROUNDING, 1.f);
	}
}