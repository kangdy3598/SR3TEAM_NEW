#pragma once
#include "TextBox.h"
#include "QuestNPC.h"
#include "CGodEffect.h"
class CWhiteBird : public CQuestNPC
{
private:
	explicit CWhiteBird(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual	 ~CWhiteBird();

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

private:
	CGodEffect* m_pGodEffect;

public:
	static CWhiteBird* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static _bool	g_bQuestClear;
	static _bool	g_bQuestAccept;
private:
	virtual void Free();
	_bool			   m_bDead;
};

