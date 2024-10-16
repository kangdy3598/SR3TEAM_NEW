#include "pch.h"
#include "Object.h"
#include "Export_Utility.h"
#include "ImGuiManger.h"
#include "ResMgr.h"

CObject::CObject(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
{
}

CObject::~CObject()
{
}

HRESULT CObject::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}



_int CObject::Update_GameObject(const _float& fTimeDelta)
{

	if (GetTransparent())
		Add_RenderGroup(RENDER_TRANSLUCNET, this);

	else
		Add_RenderGroup(RENDER_ALPHA, this);

	return Engine::CGameObject::Update_GameObject(fTimeDelta);
}

void CObject::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CObject::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	if (CManagement::GetInstance()->m_imap_stage == 2)
	{
		m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	}

	if (m_strTextureKey == L"Wall")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[0]);
	}

	else if (m_strTextureKey == L"Super")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[1]);
	
	}
	
	else if (m_strTextureKey == L"TelephonePole")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[2]);
	
	}
	
	else if (m_strTextureKey == L"BusStation")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[3]);
	
	}
	
	else if (m_strTextureKey == L"BreadStore")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[4]);
	}
	
	else if (m_strTextureKey == L"Telephone2Pole")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[5]);
	}
	
	else if (m_strTextureKey == L"Bench")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[6]);
	}
	
	else if (m_strTextureKey == L"Carpet")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[7]);
	}
	
	else if (m_strTextureKey == L"Ogu_House_Floor")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[8]);
	}
	
	else if(m_strTextureKey == L"Ogu_House_Table")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[9]);
	}
	
	else if (m_strTextureKey == L"Flower_Pot")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[10]);
	}
	
	else if (m_strTextureKey == L"Street_Lamp")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[11]);
	}
	
	else if (m_strTextureKey == L"No_Flower_Pot")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[12]);
	}
	
	
	else if (m_strTextureKey == L"One_Flower_Pot")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[13]);
	}
	
	else if (m_strTextureKey == L"One_No_Flower_Pot")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[14]);
	}
	
	else if (m_strTextureKey == L"Forest_Tree")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[15]);
	}
	
	else if (m_strTextureKey == L"Long_Wall")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[16]);
	}
	
	else if (m_strTextureKey == L"Background_Wall")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[17]);
	}
	
	else if (m_strTextureKey == L"Cart")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[18]);
	}
	
	else if (m_strTextureKey == L"Tutorial_Store2")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[19]);
	}
	
	else if (m_strTextureKey == L"One_Tree")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[20]);
	}
	
	else if (m_strTextureKey == L"Background_Sky")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[21]);
	}
	
	else if (m_strTextureKey == L"Bucket")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[22]);
	}
	
	else if (m_strTextureKey == L"Trash_Can")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[23]);
	}
	
	else if (m_strTextureKey == L"bicycle")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[24]);
	}
	
	else if (m_strTextureKey == L"Mini_Chair")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[25]);
	}
	
	
	else if (m_strTextureKey == L"Fence")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[26]);
	}
	
	else if (m_strTextureKey == L"Grass_collection")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[27]);
	}
	
	else if (m_strTextureKey == L"Stone")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[28]);
	}
	
	else if (m_strTextureKey == L"Sky_Color_Tree")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[29]);
	}
	
	else if (m_strTextureKey == L"Cave_Entrance")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[30]);
	}
	
	
	else if (m_strTextureKey == L"Cave_Wall")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[31]);	
	}

	else if (m_strTextureKey == L"Moon_Tree_one")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[32]);
	}

	else if (m_strTextureKey == L"Moon_Tree_two")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[33]);
	}

	else if (m_strTextureKey == L"Mushroom")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[34]);
	}

	else if (m_strTextureKey == L"Tree_bottom")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[35]);
	}

	else if (m_strTextureKey == L"Bug_statue_one")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[36]);
	}


	else if (m_strTextureKey == L"Sign")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[37]);
	}

	else if (m_strTextureKey == L"Bug_statue_two")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[38]);
	}

	else if (m_strTextureKey == L"Moon_Forest_Entrance")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[39]);
	}


	else if (m_strTextureKey == L"Stone_statue")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[40]);
	}

	else if (m_strTextureKey == L"Moon_Grass")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[41]);
	}


	else if (m_strTextureKey == L"Bug_statue_three")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[42]);
	}


	else if (m_strTextureKey == L"Stone_statue_two")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[43]);
	}


	else if (m_strTextureKey == L"Moon_Flag")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[44]);
	}


	else if (m_strTextureKey == L"Moon_Wall")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[45]);
	}


	else if (m_strTextureKey == L"Wood_Wall")	
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[46]);
	}


	else if (m_strTextureKey == L"Wood_Door")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[47]);
	}

	else if (m_strTextureKey == L"Monkey_Flag_one")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[48]);
	}

	else if (m_strTextureKey == L"Monkey_Flag_two")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[49]);
	}

	else if (m_strTextureKey == L"Monkey_Flag_three")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[50]);
	}

	else if (m_strTextureKey == L"Monkey_Flag_Four")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[51]);
	}

	else if (m_strTextureKey == L"Rock")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[52]);
	}

	else if (m_strTextureKey == L"Tree_Collection")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[53]);
	}

	else if (m_strTextureKey == L"Big_Tree")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[54]);
	}

	else if (m_strTextureKey == L"Big_desk")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[55]);
	}

	else if (m_strTextureKey == L"Temple_Flower_one")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[56]);
	}

	else if (m_strTextureKey == L"Temple_Flower_two")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[57]);

	}

	else if (m_strTextureKey == L"Temple_Flower_three")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[58]);
	}

	else if (m_strTextureKey == L"Temple_Wall_one")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[59]);
	}

	else if (m_strTextureKey == L"Temple_Wall_two")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[60]);
	}

	else if (m_strTextureKey == L"Temple_statue")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[61]);
	}

	else if (m_strTextureKey == L"black_cell")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[62]);
	}

	else if (m_strTextureKey == L"Arena_Wall")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[63]);
	}

	else if (m_strTextureKey == L"blackBoard")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[64]);
	}

	else if (m_strTextureKey == L"ArenaMark")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[65]);
	}

	else if (m_strTextureKey == L"Caffet_One")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[66]);
	}

	else if (m_strTextureKey == L"Caffet_Two")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[67]);
	}

	else if (m_strTextureKey == L"Caffet_Three")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[68]);
	}

	else if (m_strTextureKey == L"Pillar")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[69]);
	}


	else if (m_strTextureKey == L"SunTemple_Entrance")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[70]);
	}

	else if (m_strTextureKey == L"Jungle_Wall")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[71]);
	}

	else if (m_strTextureKey == L"Jungle_Tile")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[72]);
	}

	else if (m_strTextureKey == L"Statue_3")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[73]);
	}

	else if (m_strTextureKey == L"Statue_5")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[74]);
	}

	else if (m_strTextureKey == L"Statue_7")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[75]);
	}

	else if (m_strTextureKey == L"Statue_11")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[76]);
	}

	else if (m_strTextureKey == L"Statue_9")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[77]);
	}

	else if (m_strTextureKey == L"Statue_1")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[78]);
	}

	else if (m_strTextureKey == L"MoonTempleTile")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[79]);
	}

	else if (m_strTextureKey == L"House1")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[80]);
	}

	else if (m_strTextureKey == L"House2")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[81]);
	}

	else if (m_strTextureKey == L"House3")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[82]);
	}

	else if (m_strTextureKey == L"WaterFall")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[83]);
	}

	else if (m_strTextureKey == L"Grass_Object")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[84]);
	}

	else if (m_strTextureKey == L"Green_Pillar")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[85]);
	}

	else if (m_strTextureKey == L"harbor_Statue")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[86]);
	}

	else if (m_strTextureKey == L"harbor_Statue_2")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[87]);
	}

	else if (m_strTextureKey == L"House4")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[88]);
	}

	else if (m_strTextureKey == L"Umbrella_one")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[89]);
	}


	else if (m_strTextureKey == L"Umbrella_two")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[90]);
	}

	else if (m_strTextureKey == L"Umbrella_three")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[91]);
	}

	else if (m_strTextureKey == L"Umbrella_Chair")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[92]);
	}

	else if (m_strTextureKey == L"Harbor_Wall")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[93]);
	}

	else if (m_strTextureKey == L"Light_House")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[94]);
	}

	else if (m_strTextureKey == L"Harbor_Wall_two")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[95]);
	}

	else if (m_strTextureKey == L"Ship")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[96]);
	}

	else if (m_strTextureKey == L"Arena_Entrance")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[97]);
	}

	else if (m_strTextureKey == L"Monkey_Staute_one")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[98]);
		}

	else if (m_strTextureKey == L"Monkey_Staute_two")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[99]);
		}

	else if (m_strTextureKey == L"Monkey_Staute_three")
	{
		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetTexture()[100]);
		}


	m_pBufferCom->Render_Buffer();



	//맵툴 작성시 끄기 
	////m_pBoundBox->Render_Buffer();
	// 
	
	if (CManagement::GetInstance()->m_imap_stage == 2)
	{
		m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, false);
	}


	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CObject::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });
	
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_PlayerTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Texture",pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	
	 
	// 9월 23일 추가 선환 충돌관련 박스 
	// 맵툴 작성시 끄기 
	//pComponent = m_pBoundBox = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_pBoundBox->SetGameObjectPtr(this);
	//m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", pComponent });

	return S_OK;	


}

HRESULT CObject::SetUp_Material()
{
	return S_OK;
}

void CObject::Key_Input(const _float& fTimeDelta)
{
}

CObject* CObject::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CObject* pObject = new CObject(pGraphicDev);

	if(FAILED(pObject->Ready_GameObject()))
	{
		Safe_Release(pObject);
		MSG_BOX("pObject Create Failed");
		return nullptr;
	}

	return pObject;
}

void CObject::Free()
{
	Engine::CGameObject::Free();
}

void CObject::OnCollision(CGameObject* _pOther)
{
	int a = 4; 
}
