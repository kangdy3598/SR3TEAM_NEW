#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform :public CComponent
{

private:
	explicit CTransform();
	explicit CTransform(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTransform(const CTransform& rhs);
	virtual ~CTransform();


public:
	HRESULT					Ready_Transform();
	virtual  void			LateReady_Component();
	virtual  _int			Update_Component(const _float& fTimeDelta);
	virtual  void			LateUpdate_Component();


public:
	_matrix&	   ForGetWorldMaxtrix() { return m_matWorld; }
	const _matrix* Get_WorldMatrix() { return &m_matWorld; }
	void           Get_WorldMatrix(_matrix* pWorld) { *pWorld = m_matWorld; }
	void		   Move_Pos(const _vec3* pDir, const _float& fTimeDelta, const _float& fSpeed)
	{
		m_vInfo[INFO_POS] += *pDir * fTimeDelta * fSpeed;
	}

	void  Get_Info(INFO eType, _vec3* pInfo)
	{
		memcpy(pInfo, &m_matWorld.m[eType][0], sizeof(_vec3));
	}

	void	Rotation(ROTATION eType, const _float& fAngle)
	{
		*(((_float*)&m_vAngle) + eType) += fAngle;
	}

	void	Set_Pos(_float _fX, _float _fY, _float _fZ)
	{
		m_vInfo[INFO_POS] = { _fX, _fY, _fZ };
		memcpy(&m_matWorld.m[INFO_POS], &m_vInfo[INFO_POS], sizeof(_vec3));
	}
	void	Set_Pos(_vec3 _vec)
	{
		m_vInfo[INFO_POS] = { _vec.x, _vec.y, _vec.z };
	}

	void				Chase_Target(const _vec3* pTargetPos, const _float& fSpeed);
	const _matrix*		Compute_LookAtTarget(const _vec3* pTargetPos);

public:
	_vec3 m_vInfo[INFO_END];
	_vec3 m_vScale;
	_vec3 m_vAngle;
	_matrix m_matWorld;

	float Rotation_x;
	float Rotation_y;
	float Rotation_z;

	float Scale_x;
	float Scale_y;
	float Scale_z;

public:
	static CTransform* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone();

private:
	virtual void			Free();


};

END