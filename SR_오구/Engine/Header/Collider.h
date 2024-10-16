#pragma once
#include "Component.h"

BEGIN(Engine)

class CGameObject;

class ENGINE_DLL CCollider : public CComponent
{
	


private:
	explicit CCollider();
	explicit CCollider(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCollider(const CCollider& rhs);
	virtual  ~CCollider();


public:
	virtual  HRESULT  Ready_Buffer();
	virtual  void     Render_Buffer();
	virtual  _int	  Update_Component(const _float& fTimeDelta);
	virtual  void	  LateUpdate_Component();


public:
	static CCollider* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	CComponent* Clone();
	void SetGameObjectPtr(CGameObject* _pGameObj)
	{
		m_pGameObjPtr = _pGameObj;
	}

	_vec3 GetWorldVecMin() { return m_vWolrdVecMin; }
	_vec3 GetWorldVecMax() { return m_vWolrdVecMax; }
private:
	virtual void Free();


private:
	LPD3DXMESH	 m_pMesh;
				 
	_vec3		 m_VecMin;
	_vec3		 m_VecMax;
	_vec3		 m_vWolrdVecMin;
	_vec3		 m_vWolrdVecMax;

	D3DXVECTOR3* pVertices;
	CGameObject* m_pGameObjPtr;


	static UINT  g_iNextID; //Á¤Àû¸É¹ö¶ó °´Ã¼³»¿ë¿¡ Æ÷ÇÔ¾ÈµÊ 
	UINT         m_iID; 


public:
	bool  m_bisCollision;

	UINT GetID() { return m_iID; }
	
	

};

END