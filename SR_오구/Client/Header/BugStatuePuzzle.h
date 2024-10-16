#pragma once
#include "PuzzleObject.h"

class CBugStatuePuzzle :public Engine::CPuzzleObject
{
private:
	explicit CBugStatuePuzzle(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBugStatuePuzzle();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();
	void Match_Puzzle() override;

public:
	static CBugStatuePuzzle* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void Add_Statue(CGameObject* _pStatue) { m_vecStatues.push_back(_pStatue);	 }
	void Add_StoneBlock(CGameObject* _pBlock) { m_vecStoneBlocks.push_back(_pBlock); }	

private:
	_bool m_bIsMatched;	
	vector<CGameObject*> m_vecStatues;
	vector<CGameObject*> m_vecStoneBlocks;
	vector<CGameObject*> m_vecStoneBlocksHoles;

private:
	virtual void Free();		
};