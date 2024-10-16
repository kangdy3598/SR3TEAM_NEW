#pragma once
#include "GameObject.h"

class CBackGround;

BEGIN(Engine)

class CTriCol;
class CTransform;

END

class CTriAngle : public Engine::CGameObject
{
	explicit CTriAngle(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTriAngle();


public:
	virtual    HRESULT    Ready_GameObject();
	virtual    _int       Update_GameObject(const _float& fTimeDelta);
	virtual    void		  LateUpdate_GameObject();
	virtual    void       Render_GameObject();


private:
	HRESULT		Add_Component();
	void        Key_Input(const _float& fTimeDelta);


private:
	Engine::CTriCol* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;

public:
	static CTriAngle* Create(LPDIRECT3DDEVICE9 pGraphicDev);


private:
	virtual void Free();

private:
	CBackGround* m_pPlayer;

public:

	CBackGround* SetPlayerPtr(CBackGround* _ptr) { return m_pPlayer = _ptr; }

private:
	_vec3	 vUp;
	_vec3    vRight;
	_vec3    vLook;
	_vec3    vPosition;


};

