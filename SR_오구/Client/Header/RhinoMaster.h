#pragma once
#include "QuestNPC.h"
#include "TextBox.h"
#include "MCRabbit.h"

class CRhinoMaster : public CQuestNPC
{
private:
	explicit CRhinoMaster(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual	 ~CRhinoMaster();

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

private:
	CMCRabbit* m_pRabbit;

public:
	static CRhinoMaster* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();
};

