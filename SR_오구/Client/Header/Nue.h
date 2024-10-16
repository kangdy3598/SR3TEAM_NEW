#pragma once
#include "QuestNPC.h"
#include "TextBox.h"

class CNue : public CQuestNPC
{
private:
	explicit CNue(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual	 ~CNue();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual	  void		LateReady_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

	virtual   void		OnCollision(CGameObject* _pOther);
	virtual   void		OnCollisionExit(CGameObject* _pOther);

	void Set_QuestClear() { m_bQuestSucess = true; }
private:
	HRESULT    Add_Component();

public:
	static CNue* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();

};

