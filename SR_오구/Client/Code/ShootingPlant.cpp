#include "pch.h"
#include "ShootingPlant.h"
#include "PlantOrb.h"
#include "Export_Utility.h"

CShootingPlant::CShootingPlant(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_iImageID(0), m_isFire(false), m_fFireRate(1), m_bIsRight(false), m_fTime(0), m_iTargetID(0), m_iAnimID(0)
{
}

CShootingPlant::~CShootingPlant()
{
}

HRESULT CShootingPlant::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->m_vScale = { 150.f, 20.f, 150.f };
	m_pTexTransformCom->m_vScale = { 25.f, 25.f, 25.f };
	m_pTexTransformCom->Rotation(ROT_X, 30.f * 3.14159265359f / 180.f);
	m_eTag = TAG_NONE;
	
	m_vecAnim[0].resize(10);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/ShootingPlant_Anim/Sprite_PlantCannon_Cryan_00.png", &m_vecAnim[0][0]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/ShootingPlant_Anim/Sprite_PlantCannon_Cryan_01.png", &m_vecAnim[0][1]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/ShootingPlant_Anim/Sprite_PlantCannon_Cryan_02.png", &m_vecAnim[0][2]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/ShootingPlant_Anim/Sprite_PlantCannon_Cryan_03.png", &m_vecAnim[0][3]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/ShootingPlant_Anim/Sprite_PlantCannon_Cryan_04.png", &m_vecAnim[0][4]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/ShootingPlant_Anim/Sprite_PlantCannon_Cryan_05.png", &m_vecAnim[0][5]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/ShootingPlant_Anim/Sprite_PlantCannon_Cryan_06.png", &m_vecAnim[0][6]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/ShootingPlant_Anim/Sprite_PlantCannon_Cryan_07.png", &m_vecAnim[0][7]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/ShootingPlant_Anim/Sprite_PlantCannon_Cryan_08.png", &m_vecAnim[0][8]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/ShootingPlant_Anim/Sprite_PlantCannon_Cryan_09.png", &m_vecAnim[0][9]);

	m_vecAnim[1].resize(10);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/ShootingPlant_Anim/Sprite_PlantCannon_Yellow_00.png", &m_vecAnim[1][0]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/ShootingPlant_Anim/Sprite_PlantCannon_Yellow_01.png", &m_vecAnim[1][1]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/ShootingPlant_Anim/Sprite_PlantCannon_Yellow_02.png", &m_vecAnim[1][2]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/ShootingPlant_Anim/Sprite_PlantCannon_Yellow_03.png", &m_vecAnim[1][3]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/ShootingPlant_Anim/Sprite_PlantCannon_Yellow_04.png", &m_vecAnim[1][4]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/ShootingPlant_Anim/Sprite_PlantCannon_Yellow_05.png", &m_vecAnim[1][5]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/ShootingPlant_Anim/Sprite_PlantCannon_Yellow_06.png", &m_vecAnim[1][6]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/ShootingPlant_Anim/Sprite_PlantCannon_Yellow_07.png", &m_vecAnim[1][7]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/ShootingPlant_Anim/Sprite_PlantCannon_Yellow_08.png", &m_vecAnim[1][8]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/ShootingPlant_Anim/Sprite_PlantCannon_Yellow_09.png", &m_vecAnim[1][9]);

	m_vecAnim[4].resize(10);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/ShootingPlant_Anim/Sprite_PlantCannon_Blue_00.png", &m_vecAnim[4][0]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/ShootingPlant_Anim/Sprite_PlantCannon_Blue_01.png", &m_vecAnim[4][1]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/ShootingPlant_Anim/Sprite_PlantCannon_Blue_02.png", &m_vecAnim[4][2]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/ShootingPlant_Anim/Sprite_PlantCannon_Blue_03.png", &m_vecAnim[4][3]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/ShootingPlant_Anim/Sprite_PlantCannon_Blue_04.png", &m_vecAnim[4][4]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/ShootingPlant_Anim/Sprite_PlantCannon_Blue_05.png", &m_vecAnim[4][5]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/ShootingPlant_Anim/Sprite_PlantCannon_Blue_06.png", &m_vecAnim[4][6]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/ShootingPlant_Anim/Sprite_PlantCannon_Blue_07.png", &m_vecAnim[4][7]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/ShootingPlant_Anim/Sprite_PlantCannon_Blue_08.png", &m_vecAnim[4][8]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/ShootingPlant_Anim/Sprite_PlantCannon_Blue_09.png", &m_vecAnim[4][9]);
	return S_OK;
}

void CShootingPlant::LateReady_GameObject()
{

}

void CShootingPlant::OnCollision(CGameObject* _pOther)
{
}

void CShootingPlant::OnCollisionEnter(CGameObject* _pOther)
{
	if (_pOther->Get_Tag() == TAG_PLAYER) {
		m_iTargetID = 9;
		m_isFire = true;
	}
}

void CShootingPlant::OnCollisionExit(CGameObject* _pOther)	
{
	if (_pOther->Get_Tag() == TAG_PLAYER) {
		m_isFire = false;
		m_iTargetID = 0;
		m_iImageID = 3;
		m_fFireRate = 0.5f;
	}
}

_int CShootingPlant::Update_GameObject(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);

	if (m_iImageID != m_iTargetID) {
		m_fTime += fTimeDelta;

		if (m_fTime >= 0.05f) {
			if (m_iTargetID < m_iImageID)
				m_iImageID--;
			else if (m_iTargetID > m_iImageID)
				m_iImageID++;

			m_fTime = 0;

			if(m_isFire && m_iImageID == 8)
				Shot();

			if (m_isFire && m_iTargetID <= m_iImageID) 
				m_iImageID = 3;		
		}
	}

	for (auto& iter : m_vecBullet)	
		iter->Update_GameObject(fTimeDelta);	

	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	return iExit;
}

void CShootingPlant::LateUpdate_GameObject(const _float& fTimeDelta)
{
	for (auto& iter : m_vecBullet)
		iter->LateUpdate_GameObject(fTimeDelta);

	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CShootingPlant::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTexTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetTexture(0, m_vecAnim[m_iAnimID][m_iImageID]);
	m_pBufferCom->Render_Buffer();

	//m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	////m_pBoundBox->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	for (auto& iter : m_vecBullet)
		iter->Render_GameObject();
}

void CShootingPlant::Init_Pos(_float _fX, _float _fZ)
{
	m_pTransformCom->Set_Pos(_fX, 20.f, _fZ);
	m_pTexTransformCom->Set_Pos(_fX, 15.f, _fZ);	
}

void CShootingPlant::Set_Dir(_bool _bIsRight)
{
	if (_bIsRight) {
		m_pTexTransformCom->Rotation(ROT_Y, 180.f * 3.14159265359f / 180.f);
		m_pTexTransformCom->Rotation(ROT_X, -60.f * 3.14159265359f / 180.f);
		m_bIsRight = true;
	}
}

HRESULT CShootingPlant::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_PipeTex"));
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

CShootingPlant* CShootingPlant::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CShootingPlant* pCrystal = new CShootingPlant(pGraphicDev);

	if (FAILED(pCrystal->Ready_GameObject()))
	{
		Safe_Release(pCrystal);
		MSG_BOX("pPipeBoard Create Failed");
		return nullptr;
	}

	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::OBJECT, pCrystal);
	return pCrystal;
}

void CShootingPlant::Shot()
{
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);	
	Play_Sound(L"SFX_96_MonsterPlantCannon_Shoot.wav", SOUND_PUZZLE_SUB, 1.f);

	for (int i = 0; i < m_vecBullet.size(); ++i)
	{
		if (m_vecBullet[i]->Is_Active())
			continue;				

		float tempX =  m_bIsRight ? 1.f : -1.f;		
		static_cast<CPlantOrb*>(m_vecBullet[i])->Init_Pos(vPos.x, vPos.z, tempX);
		m_vecBullet[i]->Set_Active(true);
		return;
	}

	Create_Bullet();
}

void CShootingPlant::Create_Bullet()
{
	_vec3 vPos;
	m_pTexTransformCom->Get_Info(INFO_POS, &vPos);	
	float tempX = m_bIsRight ? 1.f : -1.f;	
	CGameObject* pOb = CPlantOrb::Create(m_pGraphicDev);
	static_cast<CPlantOrb*>(pOb)->Init_Pos(vPos.x + tempX, vPos.z, tempX);
	static_cast<CPlantOrb*>(pOb)->Set_Speed(m_bIsRight ? 35.f : -35.f);	
	static_cast<CPlantOrb*>(pOb)->Set_ImageID(m_iAnimID);
	pOb->Set_Active(true);

	m_vecBullet.push_back(pOb);
}

void CShootingPlant::Free()
{
	Engine::CGameObject::Free();
}