#include "pch.h"
#include "Catapult.h"
#include "Stone.h"
#include "Export_Utility.h"

CCatapult::CCatapult(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_pStone(nullptr), m_bIndicator(false), m_fPullDuration(0), m_iImageID(0), m_iTargetID(0), m_fTime(0)
{
}

CCatapult::~CCatapult()
{
}

HRESULT CCatapult::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	
	SetObjectType(PUSH_ABLE);
	m_pTransformCom->m_vScale = { 20.f, 20.f, 20.f };
	m_pTexTransformCom->m_vScale = { 15.f, 15.f, 15.f };
	m_pBucketTransformCom->m_vScale = { 21.f, 31.f, 0.f };
	m_pInTransformCom->m_vScale = { 17.f, 17.f, 0.f };	
	m_pTexTransformCom->Rotation(ROT_X, 90.f * 3.14159265359f / 180.f);
	m_pBucketTransformCom->Rotation(ROT_X, 90.f * 3.14159265359f / 180.f);
	m_pInTransformCom->Rotation(ROT_X, 90.f * 3.14159265359f / 180.f);

	m_vecTexture.resize(6);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Catapult_Anim/Sprite_Catapult_Anim00.png", &m_vecTexture[0]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Catapult_Anim/Sprite_Catapult_Anim01.png", &m_vecTexture[1]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Catapult_Anim/Sprite_Catapult_Anim02.png", &m_vecTexture[2]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Catapult_Anim/Sprite_Catapult_Anim03.png", &m_vecTexture[3]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Catapult_Anim/Sprite_Catapult_Anim04.png", &m_vecTexture[4]);	
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Catapult_Anim/Sprite_Catapult_Anim04.png", &m_vecTexture[5]);
	return S_OK;
}

_int CCatapult::Update_GameObject(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);
	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	if (m_pStone != nullptr && m_fPullDuration < .5f) {
		m_fPullDuration += fTimeDelta;

		_vec3 vPos, vMovePos, vTarget;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		Engine::CTransform* StoneTrasnform = static_cast<Engine::CTransform*>(m_pStone->Get_Component(ID_DYNAMIC, L"Com_Transform"));
		StoneTrasnform->Get_Info(INFO_POS, &vTarget);
		vPos.z -= 15.f;
		vPos.y = 25.f;
		vMovePos = vPos - vTarget;
		StoneTrasnform->Move_Pos(&vMovePos, fTimeDelta, 10.f);
	}

	if (m_iImageID != m_iTargetID) {
		m_fTime += fTimeDelta;

		if (m_fTime >= 0.05f) {
			if (m_iTargetID < m_iImageID)
				m_iImageID--;
			else if (m_iTargetID > m_iImageID)
				m_iImageID++;

			m_fTime = 0;
		}

		if (m_iImageID == 5)
			m_iTargetID = 0;
	}

	return iExit;
}

void CCatapult::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
	
	if (m_pStone != nullptr && m_fPullDuration > .5f) {
		_vec3 vPos;
		m_pBucketTransformCom->Get_Info(INFO_POS, &vPos);
		vPos.y = 21.f;
		vPos.z -= 23.f;
		static_cast<Engine::CTransform*>(m_pStone->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(vPos);
	}

	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	m_pTexTransformCom->Set_Pos(vPos.x, .2f, vPos.z);
	m_pBucketTransformCom->Set_Pos(vPos.x, 9.f, vPos.z + 5.f);
	m_pInTransformCom->Set_Pos(vPos.x, 0.2f, vPos.z + 245.f);

}

void CCatapult::Render_GameObject()
{
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	////m_pBoundBox->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTexTransformCom->Get_WorldMatrix());
	m_pTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pBucketTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetTexture(0, m_vecTexture[m_iImageID]);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pInTransformCom->Get_WorldMatrix());
	m_pInTextureCom->Set_Texture();	

	if (m_bIndicator) 
		m_pBufferCom->Render_Buffer();	

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CCatapult::OnCollision(CGameObject* _pOther)
{
	if (_pOther->GetObjectKey() != L"PlayerInteractionBox") {
		return;
	}		

	m_CPlayer = dynamic_cast<CPlayerInteractionBox*>(_pOther)->GetPlayer();	
	if (m_CPlayer->GetSwingTrigger() && !m_bIsActivate)
	{
		if (m_pStone == nullptr)
			return;

		int type = OBJ_TYPE::LIFT_ABLE + OBJ_TYPE::PUSH_ABLE;
		static_cast<CStone*>(m_pStone)->SetObjectType(type);
		static_cast<CStone*>(m_pStone)->Launch();
		m_pStone = nullptr;
		m_bIsActivate = true;
		m_bIndicator = false;
		Play_Sound(L"SFX_32_Catapult_Shoot.wav", SOUND_EFFECT, 1.f);

		m_iTargetID = 5;
	}

}

void CCatapult::OnCollisionEnter(CGameObject* _pOther)
{

	if (_pOther->Get_Tag() == TAG_STONE) {
		if (m_pStone != nullptr)
			return;

		m_pStone = _pOther;
		int type = OBJ_TYPE::TYPE_NONE;
		static_cast<CStone*>(m_pStone)->SetObjectType(type);
		m_fPullDuration = 0;
		m_bIndicator = true; 
		Play_Sound(L"SFX_31_Catapult_Load.wav", SOUND_EFFECT, 1.f);		
	}
}

void CCatapult::OnCollisionExit(CGameObject* _pOther)
{
	if (_pOther->GetObjectKey() != L"PlayerInteractionBox")
		return;

	m_bIsActivate = false;
}

void CCatapult::Init_Pos(_float _fX, _float _fZ)
{
	m_pTransformCom->Set_Pos(_fX, 15.f, _fZ + 10.f);
	m_pTexTransformCom->Set_Pos(_fX, .05f, _fZ);
	m_pBucketTransformCom->Set_Pos(_fX, 5.f, _fZ - 5.f);
	m_pInTransformCom->Set_Pos(_fX, 0.05f, _fZ + 40.f);
}

HRESULT CCatapult::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_CatapultBase"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

	pComponent = m_pInTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Catapult_Indicator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_InTexture", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	pComponent = m_pTexTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_TexTransform", pComponent });

	pComponent = m_pBucketTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_BucketTransform2", pComponent });

	pComponent = m_pInTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_InTransform2", pComponent });

	pComponent = m_pBoundBox = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pBoundBox->SetGameObjectPtr(this);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", pComponent });

	return S_OK;
}

CCatapult* CCatapult::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCatapult* pCrystal = new CCatapult(pGraphicDev);

	if (FAILED(pCrystal->Ready_GameObject()))
	{
		Safe_Release(pCrystal);
		MSG_BOX("pPipeBoard Create Failed");
		return nullptr;
	}

	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::OBJECT, pCrystal);
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::PUZZLE, pCrystal);
	return pCrystal;
}

void CCatapult::Free()
{
	Engine::CGameObject::Free();
}