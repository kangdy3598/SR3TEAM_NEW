#pragma once
#include "GameObject.h"
#include "Player.h"
#include "InvenUI.h"

BEGIN(Engine)

class CTexture;
class CRcTex;
class CTransform;
class CAnimator2;
class CAnimation2;

END
class CStoreUI;
class CPlayer;

class CStoreSelector : public Engine::CGameObject
{
private:
	explicit CStoreSelector(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStoreSelector();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual	  void		LateReady_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

	void	   ResetIdx() { m_iCurIdx = -5; }
private:
	HRESULT    Add_Component();
	void	   Key_Input(const _float& fTimeDelta);

	void	   Update_ItemInfo();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CAnimator2* m_pAnimatorCom; //Ãß°¡
	Engine::CTransform* m_pTransformCom;

	Engine::CTexture* m_pTextureCom;
	Engine::CTexture* m_pTexButtonCom;
	Engine::CTransform* m_pTransButtonCom;

	CPlayer*	m_pPlayer;
	CInvenUI*	m_pInven;
	CStoreUI*	m_pStore;
	_int		m_iCurIdx;
	_int		m_iPrevIdx;

	CItem*				m_pItem;
	CItem::ITEM_INFO	m_tCopyInfo;

public:
	static CStoreSelector* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();

	IDirect3DTexture9* m_pTex;
};

