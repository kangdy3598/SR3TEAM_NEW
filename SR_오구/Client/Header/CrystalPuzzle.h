#pragma once
#include "GameObject.h"
#include "Crystal.h"
#include "PressBlock.h"

class CCrystalPuzzle :public Engine::CGameObject
{
private:
	explicit CCrystalPuzzle(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCrystalPuzzle();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

public:
	static CCrystalPuzzle* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void Add_Crystal(CGameObject* _pCrystal) { m_vecCrystals.push_back(_pCrystal); }
	void Add_PressBlock(CGameObject* _pBlock) { m_vecPressBlocks.push_back(_pBlock); }
	void Check_Matched();	

private:
	_bool m_bIsMatched;
	vector<CGameObject*> m_vecCrystals;
	vector<CGameObject*> m_vecPressBlocks;

private:
	virtual void Free();
};