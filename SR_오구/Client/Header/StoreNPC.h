#pragma once
#include "GameObject.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "Player.h"
#include "InteractionButton.h"
#include "TextBox.h"
#include "QuestNPC.h"
#include "StoreUI.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTriCol;
class CTransform;
class CCollider;
class CAnimator2;
class CAnimation2;

END

class CStoreNPC : public Engine::CGameObject
{
protected:
	explicit CStoreNPC(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStoreNPC();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual	  void		LateReady_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

	virtual   void		OnCollision(CGameObject* _pOther) {}
	virtual   void		OnCollisionEnter(CGameObject* _pOther) {}
	virtual   void		OnCollisionExit(CGameObject* _pOther) {}

public:
	CQuestNPC::NPC_INFO	Get_Info() { return m_tInfo; }

protected:
	HRESULT    Add_Component();

protected:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pShopTransformCom;


	Engine::CAnimator2* m_pAnimatorCom; //추가
	Engine::CTransform* m_pTransformCom;
	Engine::CCollider* m_pColliderCom;

	CQuestNPC::NPC_INFO				m_tInfo;

	_bool					m_bConversation; // 대화 중
	_bool					m_bStoreOn;

	CInteractionButton* m_pInterButton;
	CTextBox* m_pTextBox;
	CPlayer* m_pPlayer;
	CStoreUI* m_pStoreUI;

	//CInvenUI* m_pInven;

	vector<CItem*>  m_ItemArray;
protected:
	virtual void Free();

	IDirect3DTexture9* m_pTex;
};

