#pragma once
#include "Component.h"

BEGIN(Engine)

class  ENGINE_DLL CPlantOrbCom :public CComponent
{
private:
	explicit CPlantOrbCom();
	explicit CPlantOrbCom(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPlantOrbCom(const CPlantOrbCom& rhs);
	virtual ~CPlantOrbCom();

public:
	HRESULT					Ready_Com();
	virtual  _int			Update_Component(const _float& fTimeDelta);
	virtual  void			LateUpdate_Component();

public:

public:		

public:
	static CPlantOrbCom* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone();

private:
	virtual void			Free();
};

END