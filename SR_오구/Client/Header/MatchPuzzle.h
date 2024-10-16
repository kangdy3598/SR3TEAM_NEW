#pragma once
#include "PuzzleObject.h"

class CMatchPuzzle :public Engine::CPuzzleObject
{
private:
	explicit CMatchPuzzle(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMatchPuzzle();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();
	void Match_Puzzle() override;

public:	
	void Push_Combination(_int _iCom) { m_vecCombinations.push_back(_iCom); }
	void Add_Pedestals(CGameObject* _pObj) { m_vecPedestals.push_back(_pObj); }
	void Add_StoneBlock(CGameObject* _pObj) { m_vecStoneBlocks.push_back(_pObj); }
	void Puzzle_Clear();

public:
	static CMatchPuzzle* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:	
	vector<_int> m_vecCombinations;	
	vector<CGameObject*> m_vecPedestals;
	vector<CGameObject*> m_vecStoneBlocks;

private:
	virtual void Free();		
};