#include "pch.h"
#include "StonePedestal.h"
#include "PuzzleObject.h"
#include "Export_Utility.h"

CStonePedestal::CStonePedestal(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_pCollided(nullptr), m_fPullDuration(0)
{
}

CStonePedestal::~CStonePedestal()
{
}

HRESULT CStonePedestal::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->m_vScale = { 22.f, 22.f, 22.f };
	
	m_pTexTransformCom->m_vScale = { 22.f, 22.f, 22.f };
	m_pTexTransformCom->Rotation(ROT_X, 90.f * 3.14159265359f / 180.f);

	return S_OK;
}

_int CStonePedestal::Update_GameObject(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);

	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);
	
	if (m_pCollided != nullptr && m_fPullDuration < 1.f) {
		m_fPullDuration += fTimeDelta;

		_vec3 vPos, vMovePos, vTarget;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);	
		Engine::CTransform* StoneTrasnform = static_cast<Engine::CTransform*>(m_pCollided->Get_Component(ID_DYNAMIC, L"Com_Transform"));		
		StoneTrasnform->Get_Info(INFO_POS, &vTarget);
		vPos.z -= 10.f;
		vMovePos = vPos - vTarget;
		vMovePos.y = 0.f;
		StoneTrasnform->Move_Pos(&vMovePos, fTimeDelta, 2.f);

		//Play_Sound(L"SFX_119_MazePuzzlePieceAdd.wav", SOUND_EFFECT, 1.f);
	}

	return iExit;
}

void CStonePedestal::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CStonePedestal::Render_GameObject()
{	
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTexTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();

	//m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	//////m_pBoundBox->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CStonePedestal::OnCollision(CGameObject* _pOther)
{
}

void CStonePedestal::OnCollisionEnter(CGameObject* _pOther)
{
	if (_pOther->Get_Tag() == TAG_STONE) {
		if (m_pCollided != nullptr)
			return;

		m_pCollided = _pOther;		
		static_cast<CPuzzleObject*>(m_pGroup)->Match_Puzzle();
	}
}

void CStonePedestal::OnCollisionExit(CGameObject* _pOther)
{
	if (_pOther->Get_Tag() == TAG_STONE) {
		if (m_pCollided == nullptr)
			return;

		if (m_pCollided == _pOther) {
			m_pCollided = nullptr;
			m_fPullDuration = 0;
		}

		static_cast<CPuzzleObject*>(m_pGroup)->Match_Puzzle();
	}
}

void CStonePedestal::Init(_float _fX, _float _fZ)
{
	m_pTransformCom->Set_Pos(_fX, 20.f, _fZ);
	m_pTexTransformCom->Set_Pos(_fX, .2f, _fZ);
}

HRESULT CStonePedestal::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_StonePedestal"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	pComponent = m_pTexTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_TexTransform", pComponent });

	pComponent = m_pBoundBox = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pBoundBox->SetGameObjectPtr(this);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", pComponent });

	return S_OK;
}

CStonePedestal* CStonePedestal::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStonePedestal* pCrystal = new CStonePedestal(pGraphicDev);

	if (FAILED(pCrystal->Ready_GameObject()))
	{
		Safe_Release(pCrystal);
		MSG_BOX("pPipeBoard Create Failed");
		return nullptr;
	}
	
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::PUZZLE, pCrystal);

	return pCrystal;
}

void CStonePedestal::Free()
{
	Engine::CGameObject::Free();
}