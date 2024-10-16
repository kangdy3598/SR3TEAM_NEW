#pragma once
#include "GameObject.h"


BEGIN(Engine)

class CTexture;
class CMapTex;
class CTransform;

class CMoonTempleMap:public Engine::CGameObject
{
private:
	explicit CMoonTempleMap(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMoonTempleMap();


public:
	virtual HRESULT Ready_GameObject();
	virtual _int    Update_GameObject(const float& fTimeDelta);
	virtual void    LateUpdate_GameObject(const float& fTimeDelta);
	virtual void    Render_GameObject();


private:
	HRESULT   Add_Component();
	HRESULT   SetUp_Material();

private:
	Engine::CMapTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;


public:
	static CMoonTempleMap* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();
};

END