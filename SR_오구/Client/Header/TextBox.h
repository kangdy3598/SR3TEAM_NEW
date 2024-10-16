#pragma once
#include "GameObject.h"
#include "Export_Utility.h"
#include "Export_System.h"
#include "QuestNPC.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTriCol;
class CTransform;

END

class CTextBox : public Engine::CGameObject
{
private:
	explicit CTextBox(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTextBox();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

	void	  Key_Input(const _float& fTimeDelta);
	void	  Render_Text();
	void	  CallTextBox(_bool _Call) 
	{
		m_bCall = _Call; 
		Engine::Play_Sound(L"SFX_146_ChattingText.wav", SOUND_EFFECT, 1.f);
	}
	void	  Set_Text(CQuestNPC::NPC_INFO _pNPCInfo) { m_tCopyInfo = _pNPCInfo; }
private:
	HRESULT    Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;


	_bool	m_bCall;
	_int	m_iCurStrIndex;
	float	m_fElapsedTime;

	//CNPC*				m_pCopyNPC;
	CQuestNPC::NPC_INFO		m_tCopyInfo;
public:
	static CTextBox* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();
};

