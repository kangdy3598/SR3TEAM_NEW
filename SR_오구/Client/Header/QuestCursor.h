#pragma once
#include "GameObject.h"
#include "QuestUI.h"

BEGIN(Engine)

class CTexture;
class CRcTex;
class CTransform;

END

class CQuestCursor : public Engine::CGameObject
{
private:
	explicit CQuestCursor(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CQuestCursor();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual   void		LateReady_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

	void	   ResetIdx() { m_iCurIdx = -5; }
private:
	HRESULT    Add_Component();
	void	   Key_Input(const _float& fTimeDelta);

	void	   Update_QuestInfo();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;

	CQuestUI*				m_pQuestUI;
	CQuestUI::QUEST_INFO	m_tCopyInfo;

	_int		m_iCurIdx;
	_int		m_iPrevIdx;

	_vec3		m_vCursorPos;
	float		m_fYInterval;
public:
	static CQuestCursor* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();

};

