#pragma once
#include "Item.h"
class CWaterToken : public CItem
{
private:
	explicit CWaterToken(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CWaterToken();

public:

	virtual   HRESULT   Ready_GameObject();
	virtual	  void		LateReady_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void      LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

	virtual	  void		OnCollision(CGameObject* _pOther);

private:
	HRESULT    Add_Component();

	CTexture* m_pCountRCTextureCom;
	CTransform* m_pCountRCTransformCom;
public:
	static CWaterToken* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static _bool		g_Acquired;
};

