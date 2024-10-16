#include "pch.h"
#include "RhinoStone.h"
#include "ctime"
#include "Rhino.h"
#include "Player.h"
#include "StoneEffect.h"

Engine::_uint CRhinoStone::m_iStaticID = 0;

CRhinoStone::CRhinoStone(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
	, m_bPosSet(false), m_bRotation(false)
	, m_fTime(0), m_bDead(false), m_iNum(0)
{
}

CRhinoStone::~CRhinoStone()
{
}

HRESULT CRhinoStone::Ready_GameObject()
{
	//temp
	m_iIndex = m_iStaticID;
	m_iStaticID++;

	Engine::CGameObject::Ready_GameObject();

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_eState = STONE_DEAD;

	//리노에도 만들어야하륻ㅅ
	srand(unsigned(time(NULL))); //난수 만들기

	SetObjectType(OBJ_TYPE::DESTROY_ABLE);

	_float	fFrameTime = 1.f / 7.f; // 1초에 6번 돌리기 
	D3DXCreateTextureFromFile(m_pGraphicDev, L"../Bin/Resource/Texture/ArenaMonster/RhinoStone.png", &m_pStoneTex);
	m_pAnimatorCom->CreateAnimation(L"RhinoStoneAppear", m_pStoneTex, _vec2(0.f, 0.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), fFrameTime, 6);
	m_pAnimatorCom->CreateAnimation(L"RhinoStoneDestroy", m_pStoneTex, _vec2(0.f, 256.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), fFrameTime, 6);
	m_pAnimatorCom->CreateAnimation(L"RhinoStoneSurvive", m_pStoneTex, _vec2(1536.f, 0.f), _vec2(256.f, 256.f), _vec2(0.f, 0.f), 0.5f, 0);

	return S_OK;
}

void CRhinoStone::LateReady_GameObject()
{
	Engine::CGameObject::LateReady_GameObject();

	m_pRhino = dynamic_cast<CRhino*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Rhino"));
	NULL_CHECK_RETURN(m_pRhino);

	m_pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
	NULL_CHECK_RETURN(m_pPlayer);
}

_int CRhinoStone::Update_GameObject(const _float& fTimeDelta)
{
	if (m_eState == STONE_DEAD)
		return 0;

	m_fTime += fTimeDelta;

	Add_RenderGroup(RENDER_ALPHA, this);

	if (!m_bPosSet)
	{
		m_bPosSet = true;

		_int iRandomX = 350 + (rand() % 300);
		_int iRandomZ = 220 + (rand() % 350);
		m_pTransformCom->m_vInfo[INFO_POS].x = iRandomX;
		m_pTransformCom->m_vInfo[INFO_POS].z = iRandomZ;

		//_float fX = 350 + m_iIndex * 40.f;
		//_float fZ = 220 + m_iIndex * 40.f;
		//m_pTransformCom->m_vInfo[INFO_POS].x = fX;
		//m_pTransformCom->m_vInfo[INFO_POS].z = fZ;
	}

	map<const _tchar*, CLayer*>& pMapLayer = Engine::Get_CurScenePtr()->GetLayerMapPtr();

	switch (m_eState)
	{
	case STONE_TARGET: // 타겟 지정
		if (!m_bRotation) // 회전 안했다면.
		{
			m_pTransformCom->Rotation(ROT_X, 80.f);
			m_pTransformCom->m_vInfo[INFO_POS].y = 10.f;
			m_bRotation = true;
		}
		if (m_fTime >= 2.4f)
		{
			m_eState = STONE_APPEAR; // 돌생성애니 
			m_fTime = 0.f; // 시간 리셋

			Engine::Play_Sound(L"SFX_332_StoneSpike_Ready.wav", SOUND_STONE, 2.f);

			//이펙
			wstring* Effect_Name = new wstring;
			*Effect_Name = L"Stone_Effect" + to_wstring(10 + m_iNum);

			CStoneEffect* pStoneEffect = CStoneEffect::Create(m_pGraphicDev);
			pMapLayer[L"Layer_GameLogic"]->Add_GameObject((*Effect_Name).c_str(), pStoneEffect);
			pStoneEffect->name = Effect_Name->c_str();

			_vec3 vEffectPos = m_pTransformCom->m_vInfo[INFO_POS];
			vEffectPos.y -= 2.f;
			vEffectPos.z -= 1.f;
			dynamic_cast<CTransform*>(pStoneEffect->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(vEffectPos);
		}
		break;

	case STONE_APPEAR: // 돌생성
		if (m_bRotation) // 회전 했다면 원래대로 돌려줌.
		{
			m_pTransformCom->Rotation(ROT_X, -80.f);
			m_pTransformCom->m_vInfo[INFO_POS].y = 47.f;
			m_bRotation = false;
		}
		m_pAnimatorCom->Play(L"RhinoStoneAppear", false); // 애니 실행
		if (m_pAnimatorCom->GetAnimation()->IsFinish())
		{
			//이펙
			wstring* Effect_Name = new wstring;
			*Effect_Name = L"Stone_Effect" + to_wstring(20 + m_iNum);

			CStoneEffect* pStoneEffect = CStoneEffect::Create(m_pGraphicDev);
			pMapLayer[L"Layer_GameLogic"]->Add_GameObject((*Effect_Name).c_str(), pStoneEffect);
			pStoneEffect->name = Effect_Name->c_str();

			_vec3 vEffectPos = m_pTransformCom->m_vInfo[INFO_POS];
			vEffectPos.y -= 2.f;
			vEffectPos.z -= 1.f;
			dynamic_cast<CTransform*>(pStoneEffect->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(vEffectPos);

			m_eState = STONE_SURVIVE; // 살아남은 돌
			m_fTime = 0.f; // 시간 리쏐
			m_pAnimatorCom->GetAnimation()->SetFrame(0);
		}
		break;

	case STONE_BROKEN:
	{
		if (m_pAnimatorCom->GetAnimation()->IsFinish())
		{
			m_pAnimatorCom->GetAnimation()->SetFrame(0);
			m_eState = STONE_DEAD; // 상태를 죽음으로...
			m_fTime = 0.f;
		}
		break;

	}
	}

	return Engine::CGameObject::Update_GameObject(fTimeDelta);
}

void CRhinoStone::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CRhinoStone::Render_GameObject()
{
	if (m_eState == STONE_DEAD)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	switch (m_eState)
	{
	case STONE_TARGET: // 타겟 지정
		//얘는 애니가 아니고 텍스쳐라서 일케 해줘야함.
		m_pTextureCom->Set_Texture();
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE); // 컬링 ㄴㄴ
		m_pBufferCom->Render_Buffer();
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		//m_pColliderCom->Render_Buffer();
		return;
	case STONE_APPEAR: // 돌생성
		m_pGraphicDev->SetTexture(0, m_pStoneTex);
		m_pAnimatorCom->Play(L"RhinoStoneAppear", false); // 애니 실행

		break;

	case STONE_SURVIVE:
		m_pGraphicDev->SetTexture(0, m_pStoneTex);
		m_pAnimatorCom->Play(L"RhinoStoneSurvive", true); // 그냥 하나만 출력임
		m_fTime = 0.f;
		break;

	case STONE_BROKEN:
		m_pGraphicDev->SetTexture(0, m_pStoneTex);
		m_pAnimatorCom->Play(L"RhinoStoneDestroy", false); // 충돌 브로큰 할거

		break;

	}
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE); // 컬링 ㄴㄴ
	m_pAnimatorCom->render();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW); // 컬링다시 ㅇㅇ
	//m_pColliderCom->Render_Buffer();
}

void CRhinoStone::OnCollision(CGameObject* _pOther)
{
	if (m_bDead)
		m_bIsActive = false;

	if (m_eState != STONE_SURVIVE)
		return;

	if (_pOther->GetObjectKey() == L"Rhino")
	{
		//Set_Dead(); // 타임 : 0, State : 브로큰
		m_pRhino->Set_CurHP(-100);
		m_pRhino->Set_Stun();
		m_pRhino->Set_Time(); // 0초

		// 여기에 상태를 명확하게 BROKEN으로 설정해부기 ㅠ
		m_fTime = 0.f;
		m_eState = STONE_BROKEN;
		m_pAnimatorCom->Play(L"RhinoStoneDestroy", false); // 충돌 브로큰 할거
	}

}

HRESULT CRhinoStone::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_StoneCircle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_TextureCircle", pComponent });

	pComponent = m_pAnimatorCom = dynamic_cast<CAnimator2*>(Engine::Clone_Proto(L"Proto_Animator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Ani_Buffer", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });
	m_pTransformCom->m_vScale = { 60.f, 60.f, 10.f };
	m_pTransformCom->m_vInfo[INFO_POS] = { 0.f, 50.f, 0.1f };

	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pColliderCom->SetGameObjectPtr(this);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", pComponent });

	return S_OK;
}

CRhinoStone* CRhinoStone::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRhinoStone* pStone = new CRhinoStone(pGraphicDev);

	if (FAILED(pStone->Ready_GameObject()))
	{
		Safe_Release(pStone);
		MSG_BOX("RhinoStone Create Failed");
		return nullptr;
	}

	return pStone;
}

void CRhinoStone::Free()
{
	Engine::CGameObject::Free();
}
