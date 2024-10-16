#pragma once
#include "Item.h"

class CMiddleRoot :  public CItem
{
private:
	explicit CMiddleRoot(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMiddleRoot();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual	  void		LateReady_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void      LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

	virtual	  void		OnCollision(CGameObject* _pOther);

	virtual	   void	    Use_Item();

private:
	HRESULT    Add_Component();

	CTexture* m_pCountRCTextureCom;
	CTransform* m_pCountRCTransformCom;

public:
	static CMiddleRoot* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static _bool	g_Acquired;
};

