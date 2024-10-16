#pragma once

#include "GameObject.h"


BEGIN(Engine)

class CStartMap; 
class CTransform;
class CTexture; 

END

class CTestMap : public Engine::CGameObject
{
private:
	explicit CTestMap(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTestMap();


public:
	virtual  HRESULT  Ready_GameObject();
	virtual  _int     Update_GameObject(const _float& fTimeDelta);
	virtual  void     LateUpdate_GameObject(const _float& fTimeDelta);
	virtual  void     Render_GameObject();

private:
	HRESULT  Add_Component();
	void     Key_Input(const _float& fTimeDelta);

private:
	Engine::CStartMap*		m_pBufferCom;
	Engine::CTransform*		m_pTransformCom;
	Engine::CTexture*		m_pTextureCom;


public:
	static CTestMap* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();



};