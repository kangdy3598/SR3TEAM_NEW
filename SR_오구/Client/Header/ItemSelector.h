#pragma once
#include "GameObject.h"
#include "Item.h"
#include "QuickSlot.h"

BEGIN(Engine)

class CTexture;
class CRcTex;
class CTransform;
class CAnimator2;
class CAnimation2;

END
class CInvenUI;

class CItemSelector : public Engine::CGameObject
{
private:
	explicit CItemSelector(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CItemSelector();

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
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;

	Engine::CTexture* m_pTexButtonCom;
	Engine::CTransform* m_pTransButtonCom;


	CInvenUI* m_pInven;
	CQuickSlot* m_pQuickSlot;//0925

	_int		m_iCurIdx;
	_int		m_iPrevIdx;

	CItem* m_pItem;
	CItem::ITEM_INFO	m_tCopyInfo;
	_bool				m_bWaitQuickNum;
public:
	static CItemSelector* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();
	IDirect3DTexture9* m_pTex;

};

