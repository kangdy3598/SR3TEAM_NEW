#pragma once
#include "GameObject.h"
#include "BossHPBar.h"
enum class Electriceel_STATE
{
	OUT_9_WATERFALL,
	OUT_11_SHOT,
	OUT_12_SHOT,	
	OUT_1_SHOT,	
	OUT_3_WATERFALL,	

	IDLE_12_SHOT,
	IDLE_1_SHOT,
	IDLE_3_WATERFALL,
	IDLE_9_WATERFALL,
	IDLE_11_SHOT,

	PROJECT_SHOT_12,
	PROJECT_SHOT_1,
	PROJECT_WATERFALL_3,		
	PROJECT_WATERFALL_9,	
	PROJECT_SHOT_11,

	IN_9,	
	IN_11,	
	IN_12,	
	IN_1,	
	IN_3,	

	IN_12_END,
	
	OUT_12_MAGIC,	
	OUT_9_MAGIC,	
	OUT_11_MAGIC,	
	OUT_1_MAGIC,	
	OUT_3_MAGIC,

	IDLE_12_MAGIC,
	IDLE_1_MAGIC,
	IDLE_3_MAGIC,
	IDLE_9_MAGIC,
	IDLE_11_MAGIC,

	PROJECT_MAGIC_12,
	PROJECT_MAGIC_1,
	PROJECT_MAGIC_3,
	PROJECT_MAGIC_9,
	PROJECT_MAGIC_11,




	ATTACK,

};


BEGIN(Engine)

class CAnimator2;
class CTransform;
class CCollider;

class CElectriceelBoss  : public Engine::CGameObject
{
private:
	explicit CElectriceelBoss(LPDIRECT3DDEVICE9 pGraphicDev);	
	virtual ~CElectriceelBoss();

public:
	void update_move();
	void update_state();
	void update_sound();
	void update_animation();


public:
	virtual  HRESULT  Ready_GameObject();
	virtual  _int     Update_GameObject(const _float& fTimeDelta);
	virtual  void     LateUpdate_GameObject(const _float& fTimeDelta);
	virtual  void     Render_GameObject();

private:
	HRESULT  Add_Component();
	HRESULT  SetUp_Material();
	void     Key_Input(const _float& fTimeDelta);

private:

	Electriceel_STATE m_eCurState;
	Electriceel_STATE m_ePreState;

	ULONGLONG dwtime;	
	ULONGLONG dwtimeShot;
	Engine::CAnimator2* m_pAnimatorCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CCollider* m_pBoundBox;

	CBossHPBar* m_pBossHPBar;
	CBossHPBar::BOSS_INFO m_tInfo;	

public:
	static CElectriceelBoss* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();

private:
	vector<IDirect3DTexture9*> m_vecTexture;
	IDirect3DTexture9* m_pComeTex;

public:
	virtual void OnCollisionEnter(CGameObject* _pOther);
	virtual void OnCollision(CGameObject* _pOther);

private:

	bool LoadTextureFromFile(LPDIRECT3DDEVICE9 d3dDevice, const char* filePath, IDirect3DTexture9** outTexture)
	{
		HRESULT hr = D3DXCreateTextureFromFileA(d3dDevice, filePath, outTexture);
		return SUCCEEDED(hr);
	}

	int  m_iCount;
	int  m_iBulletCount; 
	int  m_iWaterFallCount;

};

END