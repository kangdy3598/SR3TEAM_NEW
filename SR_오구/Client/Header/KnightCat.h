#pragma once
#include "TextBox.h"
#include "QuestNPC.h"
class CKnightCat : public CQuestNPC
{
private:
	explicit CKnightCat(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual	 ~CKnightCat();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual	  void		LateReady_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

	virtual   void		OnCollision(CGameObject* _pOther);
	virtual   void		OnCollisionExit(CGameObject* _pOther);

private:
	HRESULT    Add_Component();

public:
	static CKnightCat* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();
};

