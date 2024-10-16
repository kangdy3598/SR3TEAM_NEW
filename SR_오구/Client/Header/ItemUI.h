#pragma once
#include "GameObject.h"
#include "Item.h"
#include "Player.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END
class CItemUI : public Engine::CGameObject
{
private:
	explicit CItemUI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CItemUI();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

	void	  Key_Input(const _float& fTimeDelta);
	void	  CallItemUI(_bool _Call)
	{
		m_bCall = _Call;
		Engine::Play_Sound(L"SFX_147_OguItemAdd.wav", SOUND_EFFECT, 0.7);
	}
	void	  Set_Text(CItem::ITEM_INFO _tInfo) { m_tCopyInfo = _tInfo; }
	void	  Set_Texture(CTexture* _pTexture)
	{
		m_pItemTextureCom = _pTexture;
		m_pItemTextureCom->Set_Texture();
	}
	void	  Render_Text();

private:
	HRESULT    Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;

	Engine::CTexture* m_pItemTextureCom;
	Engine::CTransform* m_pItemTransformCom;

	_bool	m_bCall;

	CItem::ITEM_INFO		m_tCopyInfo;

	CPlayer* m_pPlayer;
public:
	static CItemUI* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();

};

