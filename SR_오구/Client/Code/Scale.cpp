#include "pch.h"
#include "Scale.h"
#include "StonePedestal.h"
#include "Export_Utility.h"

CScale::CScale(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CPuzzleObject(pGraphicDev), m_iImageID(6), m_iTargetID(6), m_fTime(0)
{

}

CScale::~CScale()
{
}

HRESULT CScale::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->m_vScale = { 80.f, 60.f, 0.f };
	m_pLeftCompTransformCom->m_vScale = { 25.f, 25.f, 15.f };
	m_pRightCompTransformCom->m_vScale = { 25.f, 25.f, 15.f };
	m_vecTexture.resize(13);	

	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Scale_Anim/Scale_Left_Anim05.png", &m_vecTexture[0]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Scale_Anim/Scale_Left_Anim04.png", &m_vecTexture[1]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Scale_Anim/Scale_Left_Anim03.png", &m_vecTexture[2]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Scale_Anim/Scale_Left_Anim02.png", &m_vecTexture[3]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Scale_Anim/Scale_Left_Anim01.png", &m_vecTexture[4]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Scale_Anim/Scale_Left_Anim00.png", &m_vecTexture[5]);

	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Scale_Anim/Scale_Balanced.png", &m_vecTexture[6]);

	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Scale_Anim/Scale_Right_Anim00.png", &m_vecTexture[7]);	
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Scale_Anim/Scale_Right_Anim01.png", &m_vecTexture[8]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Scale_Anim/Scale_Right_Anim02.png", &m_vecTexture[9]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Scale_Anim/Scale_Right_Anim03.png", &m_vecTexture[10]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Scale_Anim/Scale_Right_Anim04.png", &m_vecTexture[11]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Scale_Anim/Scale_Right_Anim05.png", &m_vecTexture[12]);

	return S_OK;
}

_int CScale::Update_GameObject(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);	
	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	m_pLeftPedestal->Update_GameObject(fTimeDelta);
	m_pRightPedestal->Update_GameObject(fTimeDelta);

	//m_fTime += fTimeDelta;
	//if (m_fTime > 0.5f )
	//{
	//	m_iImageID++;
	//	m_iImageID %= 12;
	//	m_fTime = 0;
	//}

	if (m_iImageID != m_iTargetID) {
		m_fTime += fTimeDelta;

		if (m_fTime >= 0.01f) {
			if (m_iTargetID < m_iImageID)
				m_iImageID--;
			else if (m_iTargetID > m_iImageID)
				m_iImageID++;

			m_fTime = 0;
		}
	}

	return iExit;
}

void CScale::LateUpdate_GameObject(const _float& fTimeDelta)
{
	m_pLeftPedestal->LateUpdate_GameObject(fTimeDelta);
	m_pRightPedestal->LateUpdate_GameObject(fTimeDelta);

	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CScale::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetTexture(0, m_vecTexture[m_iImageID]);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pLeftCompTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pLeftCompTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pRightCompTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pRightCompTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pLeftPedestal->Render_GameObject();
	m_pRightPedestal->Render_GameObject();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CScale::Init_Position(float _fX, float _fZ)
{
	m_pTransformCom->Set_Pos(_fX, 60.f, _fZ);

	m_pLeftCompTransformCom->Set_Pos(_fX - 60.f, 0.1f, _fZ - 40.f);
	m_pLeftCompTransformCom->Rotation(ROT_X, 90.f * 3.14159265359f / 180.f);

	m_pRightCompTransformCom->Set_Pos(_fX + 60.f, 0.1f, _fZ - 40.f);
	m_pRightCompTransformCom->Rotation(ROT_X, 90.f * 3.14159265359f / 180.f);
	
	CGameObject* pGameObject = CStonePedestal::Create(m_pGraphicDev);
	static_cast<CStonePedestal*>(pGameObject)->Init(_fX - 60.f, _fZ - 40.f);
	static_cast<CStonePedestal*>(pGameObject)->Set_Group(this);
	m_pLeftPedestal = pGameObject;

	pGameObject = CStonePedestal::Create(m_pGraphicDev);
	static_cast<CStonePedestal*>(pGameObject)->Init(_fX + 60.f, _fZ - 40.f);
	static_cast<CStonePedestal*>(pGameObject)->Set_Group(this);
	m_pRightPedestal = pGameObject;
}

HRESULT CScale::Add_Component()
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

	pComponent = m_pLeftCompTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_StonePlaceFrame"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_CLeftompTexture", pComponent });

	pComponent = m_pLeftCompTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_LeftCompTransform", pComponent });	

	pComponent = m_pRightCompTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_StonePlaceFrame"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_RightCompTexture", pComponent });

	pComponent = m_pRightCompTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_RightCompTransform", pComponent });

	return S_OK;
}

CScale* CScale::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CScale* pCrystal = new CScale(pGraphicDev);

	if (FAILED(pCrystal->Ready_GameObject()))
	{
		Safe_Release(pCrystal);
		MSG_BOX("pPipeBoard Create Failed");
		return nullptr;
	}

	return pCrystal;
}

void CScale::Free()
{
	Engine::CGameObject::Free();
}

void CScale::Match_Puzzle()
{	
	int iTemp = static_cast<CStonePedestal*>(m_pLeftPedestal)->Get_StoneID();
	int iSour = static_cast<CStonePedestal*>(m_pRightPedestal)->Get_StoneID();

	if (iTemp == -1 || iSour == -1) {
		m_iTargetID = 6;
		return;
	}		
	
	m_iTargetID = iTemp > iSour ? 0 : 12;
	Play_Sound(L"SFX_77_StoneUnload.wav", SOUND_PUZZLE_SUB, 1.f);
}
