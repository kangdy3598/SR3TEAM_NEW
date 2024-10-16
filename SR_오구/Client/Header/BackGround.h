#pragma once

#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CAnimationTex;

END


class CBackGround : public Engine::CGameObject
{
private:
	explicit CBackGround(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBackGround();


public:
	virtual    HRESULT    Ready_GameObject();
	virtual    _int       Update_GameObject(const _float& fTimeDelta);
	virtual    void		  LateUpdate_GameObject(const _float& fTimeDelta);
	virtual    void       Render_GameObject();


private:
	HRESULT		Add_Component();



private:
	Engine::CRcTex*			m_pBufferCom;	
	Engine::CTexture*		m_pTextureCom;	
	Engine::CAnimationTex*   m_pAnimationCom;

public:
	static CBackGround* Create(LPDIRECT3DDEVICE9 pGraphicDev);


private:
	virtual void Free();




};

