#pragma once
#include "Component.h"
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CPipeBoardCom : public CComponent
{
private:
	explicit CPipeBoardCom();
	explicit CPipeBoardCom(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPipeBoardCom(const CPipeBoardCom& rhs);
	virtual ~CPipeBoardCom();

public:
	HRESULT					Ready_PipeBoard();
	virtual  _int			Update_Component(const _float& fTimeDelta);
	virtual  void			LateUpdate_Component();

public:
	_bool m_bIsPipeSpawned;
	_int m_iCurPipeCount;
	_int m_iGridSizeX;
	_int m_iGridSizeZ;
	_int m_iCursorID;
	CGameObject* m_pCursor;
	CGameObject* m_pStatingPipe;
	CGameObject* m_pPickedPipe;
	vector<CGameObject*> m_vecBoardGrid;
	
public:
	static CPipeBoardCom* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone();

private:
	virtual void			Free();

};

END