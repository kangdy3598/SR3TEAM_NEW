#pragma once
#include "QuestNPC.h"
#include "TextBox.h"
#include "Export_Utility.h"

class CExpressMonkey : public CQuestNPC
{
private:
	explicit CExpressMonkey(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual	 ~CExpressMonkey();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual	  void		LateReady_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

	virtual   void		OnCollision(CGameObject* _pOther);
	virtual   void		OnCollisionEnter(CGameObject* _pOther);
	virtual   void		OnCollisionExit(CGameObject* _pOther);

	void		Set_Pos(_vec3 _vPos) { m_pTransformCom->m_vInfo[INFO_POS] = _vPos; }
private:
	HRESULT    Add_Component();
	Engine::CTransform* m_pBalloonTransformCom;

public:
	static CExpressMonkey* Create(LPDIRECT3DDEVICE9 pGraphicDev);

	static	_bool g_bQuestClear;
	static	_bool g_bQuestAccept;
private:
	virtual void Free();

	IDirect3DTexture9* m_pComeTex;
	_bool			   m_bEnter;

};

