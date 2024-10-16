#pragma once
#include "GameObject.h"
#include "PlayerIdle.h"
#include "PlayerMove.h"
#include "PlayerDash.h"
#include "PlayerSwing.h"
#include "PlayerLift.h"
#include "PlayerLiftMove.h"
#include "PlayerPush.h"
#include "PlayerDance.h"
#include "PlayerSmash.h"
#include "PlayerRolling.h"
#include "PlayerHurt.h"
#include "PlayerPickUp.h"
#include "PlayerBalloonFly.h"
#include "EquipHat.h"
#include "Export_Utility.h"

//민지
#pragma region 민지
#include "PowerUI.h"
#include "SpeedUI.h"

#include "ExploreHat.h"
#include "PartyHat.h"
#include "Mohican.h"
#include "EnkyHat.h"
#include "AppleHat.h"
#include "Penguin.h"
#include "PoopHat.h"

#include "SmallFruit.h"
#include "MiddleFruit.h"
#include "BigFruit.h"
#include "SmallHerb.h"
#include "MiddleHerb.h"
#include "BigHerb.h"
#include "SmallRoot.h"
#include "MiddleRoot.h"
#include "BigRoot.h"
#include "CookSmallFish.h"
#include "CookMiddleFish.h"
#include "CookBigFish.h"

#include "Branch.h"
#include "Leaf.h"

#include "ExpressTicket.h"
#include "Net.h"

#include "QuestUI.h"
#include "QuickSlot.h"
#include "InvenUI.h"

#pragma endregion 민지


BEGIN(Engine)

class CTexture;
class CRcTex;
class CTransform;
class CCamera;
class CCalculator;
class CStateController;
class CCollider;

END

class CBuffUI;//1003
class CPlayer : public Engine::CGameObject
{
private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPlayer();

public:
	struct PLAYERHP
	{
		_int	iCurHP;
		_int	iMaxHP;
	};

	virtual			HRESULT			Ready_GameObject();
	virtual			void			LateReady_GameObject();
	virtual			_int			Update_GameObject(const _float& fTimeDelta);
	virtual			void			LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void			Render_GameObject();

	virtual			void			OnCollisionEnter(CGameObject* _pOther);
	virtual			void			OnCollisionExit(CGameObject* _pOther);

public:
	// 애니메이션 관련 /////////////////////////////////////////////////////////
	CAnimation* GetAnimationComp() { return m_pAnimationCom; }

	// 플레이어 현재 행동을 리턴합니다.
	PLAYERSTATE		GetPlayerState() { return m_ePlayerState; }
	void			SetPlayerState(PLAYERSTATE _ePlayerState)
	{
		m_ePlayerState = _ePlayerState;
		m_pAnimationCom->SetAnimFrame(m_ePlayerState, m_bIsDiagonal);
	}
	//1010 민지
	_int Get_HonorScore() { return m_iHonorScore; }
	void Set_HonorScore(_int _Score) { m_iHonorScore += _Score; }
	void Set_Interacting(_bool _bInteract) { m_bIsInteracting = _bInteract; }
	// 공격중인지 판단합니다.
	_bool			GetSwingTrigger() { return m_bSwingTrigger; }
	void			SetSwingTrigger(_bool bSwing) { m_bSwingTrigger = bSwing; }
	// 밀 수 있는지 판단합니다.
	_bool			GetPushTrigger() { return m_bPushTrigger; }
	void			SetPushTrigger(_bool bPush) { m_bPushTrigger = bPush; }

	// 인터렉션박스와 인터렉션박스와 충돌중인 오브젝트를 가져오거나 리턴합니다.
	CGameObject* GetInteractionBox() { return m_objInteractionBox; }
	void			SetInteractionBox(CGameObject* _obj) { m_objInteractionBox = _obj; }
	CGameObject* GetInteractingObj() { return m_objInteracting; }
	void			SetInteractingObj(CGameObject* _obj) { m_objInteracting = _obj; }
	CGameObject* GetLiftObj() { return m_objLiftObject; }
	void			SetLiftObj(CGameObject* _obj) { m_objLiftObject = _obj; }

	// 플레이어가 지금 이동중인 방향을 리턴합니다.
	_vec3			GetPlayerDirVector() { return m_vPlayerDir; }
	// 플레이어에서 인터렉션박스로 향하는 방향을 리턴합니다.
	_vec3			GetPlayerDirVector2()
	{
		CTransform* BoxTransform = dynamic_cast<CTransform*>(
			m_objInteractionBox->Get_Component(ID_DYNAMIC, L"Com_Transform"));

		_vec3 vBoxPos;
		BoxTransform->Get_Info(INFO_POS, &vBoxPos);

		_vec3 vPlayerPos;
		m_pTransformCom->Get_Info(INFO_POS, &vPlayerPos);

		_vec3 returnPos = vBoxPos - vPlayerPos;
		D3DXVec3Normalize(&returnPos, &returnPos);
		return returnPos;

	}
	// 플레이어가 대각선 방향에 위치에 있는지 판단합니다.
	bool			IsPlayerDiagonal() { return m_bIsDiagonal; }
	// 키 입력으로 캐릭터의 방향을 조정합니다.
	void			SetPlayerDirection();
	// 해당 방향으로 플레이어를 고정 시킵니다.
	void			FixPlayerDir(int _fixDir)
	{
		m_vPlayerDir.x = 0; m_vPlayerDir.z = 1;
		DisableDiagonal();
		m_pAnimationCom->SetAnimDir(m_ePlayerState, m_vPlayerDir, m_bIsDiagonal);
	}
	// 현재 방향으로 플레이어를 고정 시킵니다.
	void			FixCurPlayerDir(_bool _value) { m_bFixPlayerDir = _value; }
	// 무조건 플레이어 방향을 대각선이 아닌 방향으로 바꿉니다.
	void			DisableDiagonal() { m_bIsDiagonal = false; }
	// 물체랑 부딪혀도 통과할 수 있는지 판단합니다. 리턴값이 false 플레이어가 움직이지 않습니다.
	_bool			GetPassAble() { return m_bPassAble; }
	void			SetPassAble(_bool value) { m_bPassAble = value; }
	// 플레이어 상태를 아이템 획득 모션으로 변경합니다.
	void			ChangePickUpState();
	// 플레이어 상태를 숭숭 익스프레스 모션으로 변경합니다.
	void			ChangeBalloonFlyState();
	// 플레이어 능력치 관련 //////////////////////////////////////////////////////
	float			GetMoveSpeed() { return m_fMoveSpeed; }
	void			SetMoveSpeed(float _fSpeed) { m_fMoveSpeed = _fSpeed; }
	float			GetItemMoveSpeed() { return m_fItemMoveSpeed; }
	void			SetItemMoveSpeed(float _itemSpeed) { m_fItemMoveSpeed += _itemSpeed; }

	int				GetItemAttackPower() { return m_iItemAttackPower; }
	void			SetItemAttackPower(int _itemPower) { m_iItemAttackPower += _itemPower; }
	//민지
	_bool           GetVisitingStore() { return m_bStoreVisit; }
	void            SetVisitingStore(_bool _bVisit) { m_bStoreVisit = _bVisit; }
	_int			GetPlayerCoin() { return m_iPlayerCoin; }
	void            SetPlayerCoin(_int _iCoin) { m_iPlayerCoin += _iCoin; }
	PLAYERHP		GetPlayerHP() { return m_tPlayerHP; }
	_bool			GetPlayerQuestUI() { return m_bQuest; }//0928
	_bool			GetPlayerInven() { return m_bInven; }
	CInvenUI*		GetPlayerInventory() { return m_pInven; }
	void			SetPlayerCurHP(_int _SetHP)
	{
		m_tPlayerHP.iCurHP += _SetHP;
		if (m_tPlayerHP.iCurHP > m_tPlayerHP.iMaxHP)
			m_tPlayerHP.iCurHP = m_tPlayerHP.iMaxHP;
		else if (m_tPlayerHP.iCurHP < 0)
			m_tPlayerHP.iCurHP = 0;
	}
	void			SetPlayerMaxHP(_int _SetHP)
	{
		m_tPlayerHP.iMaxHP += _SetHP;
		m_tPlayerHP.iCurHP = m_tPlayerHP.iMaxHP;
	}
	//1003
	void			SetPowerTime(_int _SetTime, _float _SetPower = 10.f)
	{
		m_BuffArray[0]->Set_BuffTime(_SetTime);
		m_iItemAttackPower = _SetPower;
	}
	void			SetSpeedTime(_int _SetTime, _float _SetSpeed = 20.f)
	{
		m_BuffArray[1]->Set_BuffTime(_SetTime);
		m_fItemMoveSpeed = _SetSpeed;
	}
	_vec3			GetPlayerPos() { return m_pTransformCom->m_vInfo[INFO_POS]; }
	_vec3			SetPlayerPos(_vec3 _vPos) { return m_pTransformCom->m_vInfo[INFO_POS] = _vPos; }


	void			SetInvincible(_bool value = true) { m_bInvincible = value; }
	bool			IsInvincible() { return m_bInvincible; }
	void			DurationInvincible(const _float& fTimeDelta);
	// 충돌 관련
	_vec3			GetColliderPos() { return m_vColliderPos; }
	_vec3			GetColPlayerPos() { return m_vColPlayerPos; }
	////////////////////////////////////////////////////////////////////////////
	CDynamicCamera* GetCamera() { return m_pCamera; }
	void			SetCamera(CDynamicCamera* _camera) { m_pCamera = _camera; }

	void			SetEquipHat(CEquipHat* _equipHat) { m_equipHat = _equipHat; }
	CEquipHat* GetEquipHat() { return m_equipHat; }

	void			SetNextSceneOn() { m_bNextStage = true; }
	_bool			IsNextSceneOn() { return m_bNextStage; }
	//조명 예시 
	HRESULT			SetUp_Material();



private:
	HRESULT			Add_Component();
	void			Key_Input(const _float& fTimeDelta);
	_vec3			Piking_OnTerrain();
	void			Print_PlayerState();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CAnimation* m_pAnimationCom;
	CDynamicCamera* m_pCamera;
	Engine::CStateController* m_pStateControlCom;
	Engine::CCollider* m_pBoundBox;

	PLAYERSTATE					m_ePlayerState;

	_bool						m_bIsDiagonal;
	_vec3						m_vPlayerDir;
	_bool						m_bFixPlayerDir;

	_bool						m_bSwingTrigger;
	_bool						m_bPushTrigger;

	_vec3						m_vColPlayerPos;
	_vec3						m_vColliderPos;

	_bool						m_bPassAble;
	_bool						m_bInvincible;

	_int						m_iAttackPower;
	_int						m_iItemAttackPower;
	_float						m_fMoveSpeed;
	_float						m_fItemMoveSpeed;


	CGameObject* m_objInteractionBox;  // 상호작용할 박스
	CGameObject* m_objInteracting;     // 그 박스와 충돌 중인 오브젝트
	CGameObject* m_objLiftObject;

	CEquipHat* m_equipHat;
	//민지
	CBuffUI* m_BuffArray[2];

	CInvenUI* m_pInven;
	CQuestUI* m_pQuestUI;
	CQuickSlot* m_pQuickSlot;

	//1010 민지
	_int		m_iHonorScore;
	_int		m_iPlayerCoin;
	PLAYERHP	m_tPlayerHP;
	_bool		m_bInven;
	_bool		m_bQuest;
	_bool		m_bStoreVisit;
	_bool		m_bIsInteracting;
	_bool		m_bNextStage = false;
public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();


	//김선환 긴급해서 퍼블릭사용함 10월 11일
public:
	bool  m_bSmashEnd;


};

