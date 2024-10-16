#pragma once
#include "GameObject.h"

class CRay : public CGameObject
{
	explicit CRay(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CRay();

public:
	virtual			HRESULT			Ready_GameObject();
	virtual			_int			Update_GameObject(const _float& fTimeDelta);
	virtual			void			LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void			Render_GameObject();

private:
	HRESULT			Add_Component();
	void			Key_Input(const _float& fTimeDelta);

public:
	static CRay* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();



//추가 코드
public:
	// 광선 구조체 
	typedef struct tagRay
	{
		D3DXVECTOR3 Orign;  // 광선의 원점
		D3DXVECTOR3 direction; // 광선의 방향벡터
	}RAY,*LPRAY;

private:
	LPRAY m_pRay; // 광선 정보

public:
	LPRAY initRay(D3DXMATRIXA16* matProj, _D3DXMATRIXA16* matView);

	BOOL  destoryRay(VOID);

public:
	LPRAY  getRay(VOID) { return m_pRay; }


private:
	VOID   transformRay(LPRAY pRay, D3DXMATRIXA16* pMatrix);

};

