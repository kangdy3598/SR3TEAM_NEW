#pragma once
#include "TextBox.h"
#include "QuestNPC.h"
#include "Rhino.h"
#include "MonsterMothMage.h"

class CMCRabbit : public CQuestNPC
{
private:
	explicit CMCRabbit(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual	 ~CMCRabbit();

public:
	enum MONSTER_NUM
	{
		RHINO, MOTH_MAGE, NUM_END
	};

	virtual   HRESULT   Ready_GameObject();
	virtual	  void		LateReady_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

	virtual   void		OnCollision(CGameObject* _pOther);
	//virtual   void		OnCollisionEnter(CGameObject* _pOther);
	virtual   void		OnCollisionExit(CGameObject* _pOther);

	void	   Set_Monster(MONSTER_NUM _eChoice) { m_eMonster = _eChoice; }
	
	void	   AddDefeatCount() { m_iDefeatCount++; }

private:
	HRESULT    Add_Component();
	void	   Create_Monster();
	void	   Set_Victory();

private:
	MONSTER_NUM m_eMonster;

	CRhino* m_pRhino;
	int m_iDefeatCount = 0;

public:
	static CMCRabbit* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();

};

