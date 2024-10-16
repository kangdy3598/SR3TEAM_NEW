#pragma once
#include "QuestNPC.h"
#include "TextBox.h"

class CGod : public CQuestNPC
{
private:
	explicit CGod(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual	 ~CGod();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual	  void		LateReady_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

	virtual   void		OnCollision(CGameObject* _pOther);
	virtual   void		OnCollisionEnter(CGameObject* _pOther);
	virtual   void		OnCollisionExit(CGameObject* _pOther);

	void		Call_God() { m_bCall = true; }
private:
	enum GODSTATE { GOD_IN1, GOD_IN2, GOD_IN3, GOD_IDLE1, GOD_IDLE2, GOD_IDLE3 };

	HRESULT    Add_Component();

public:
	static CGod* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();
	IDirect3DTexture9* m_pComeTex;

	_bool	  m_bCall;
	GODSTATE  m_eState;
};

