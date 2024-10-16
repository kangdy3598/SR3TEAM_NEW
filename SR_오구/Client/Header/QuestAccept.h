#pragma once
#include "GameObject.h"
#include "QuestUI.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CQuestAccept : public Engine::CGameObject
{
private:
	explicit CQuestAccept(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CQuestAccept();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

	void	  Key_Input(const _float& fTimeDelta);
	void	  CallQuestAcceptUI(_bool _Call) 
	{
		m_bCall = _Call; 
		Engine::Play_Sound(L"SFX_148_UINormalOpen.wav", SOUND_EFFECT, 0.8);
		//Engine::Play_Sound(L"SFX_446_QuestClear.wav", SOUND_EFFECT, 0.7);

	}
	void	  Set_Text(CQuestUI::QUEST_INFO _pNPCInfo) { m_tCopyInfo = _pNPCInfo; }
	void	  Render_Text();
private:
	HRESULT    Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;

	_bool	m_bCall;

	CQuestUI::QUEST_INFO		m_tCopyInfo;
public:
	static CQuestAccept* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();

};

