#pragma once
#include "QuestNPC.h"
#include "TextBox.h"

class CBardCat : public CQuestNPC
{
private:
	explicit CBardCat(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual	 ~CBardCat();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual	  void		LateReady_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

	virtual   void		OnCollision(CGameObject* _pOther);
	virtual   void		OnCollisionEnter(CGameObject* _pOther);
	virtual   void		OnCollisionExit(CGameObject* _pOther);

private:
	HRESULT    Add_Component();

	enum CAT_STATE { CAT_IDLE, CAT_SONG };
public:
	static CBardCat* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();
	CAT_STATE m_eState;
};

