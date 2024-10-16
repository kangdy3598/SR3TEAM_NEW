#pragma once
#include "Component.h"

BEGIN(Engine)

class  ENGINE_DLL CPipeCom :public CComponent
{
private:
	explicit CPipeCom();
	explicit CPipeCom(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPipeCom(const CPipeCom& rhs);
	virtual ~CPipeCom();
	
public:
	HRESULT					Ready_Pipe();
	virtual  _int			Update_Component(const _float& fTimeDelta);
	virtual  void			LateUpdate_Component();

public:
	void Add_PipeFlow(PIPEFLOW _eType) { m_FlowList.push_back(_eType); }
	_bool Is_PipeType_Exist(PIPEFLOW _eType) {
		for (auto& iter : m_FlowList)
			if (iter == _eType)
				return true;
			
		return false;
	}

public:	
	_bool m_bIsFixed;
	_bool m_bIsConnected;
	list<PIPEFLOW> m_FlowList;

public:
	static CPipeCom* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone();

private:
	virtual void			Free();
};

END