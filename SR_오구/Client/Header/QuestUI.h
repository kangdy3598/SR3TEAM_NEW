#pragma once
#include "GameObject.h"
#include "Export_System.h"
#include "Export_Utility.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END
class CQuestCursor;
class CPlayer;

class CQuestUI : public Engine::CGameObject
{
private:
	explicit CQuestUI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CQuestUI();

public:
	struct QUEST_INFO
	{
		const _tchar* pQuestTitle;
		const _tchar* pQuestContent;
	};

	virtual   HRESULT   Ready_GameObject();
	virtual   void      LateReady_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void      LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

	void					Add_Quest(QUEST_INFO _Quest) { m_QuestArray.push_back(_Quest); }
	QUEST_INFO				Get_Quest(_int _iIdx) { return m_QuestArray[_iIdx]; }
	vector<QUEST_INFO>*		Get_QuestArray() { return &m_QuestArray; }
private:
	HRESULT    Add_Component();

	//void	   Key_Input(const _float& fTimeDelta);

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;

	//_bool					m_bCursorCreate;

private:
	vector<QUEST_INFO>		m_QuestArray;

	CPlayer*		m_pPlayer;
	CQuestCursor*	m_pCursor;

	_vec2			m_vQuestInterval; // АЃАн
	_vec2			m_vQuestPos;

public:
	static CQuestUI* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();

};

