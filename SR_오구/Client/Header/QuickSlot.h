#pragma once
#include "GameObject.h"
#include "Export_Utility.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END
class CItem;

class CQuickSlot : public Engine::CGameObject
{
private:

	explicit CQuickSlot(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CQuickSlot();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void      LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

public:
	void		Add_QuickSlotItem(_int _iIndex, CItem* _pItem);
	_vec3		Set_QuickSlotPos(int _iQuickItemIdx)
	{
		_vec3 vQuickSlotPos = m_vFirstItemPos;
		vQuickSlotPos.x = vQuickSlotPos.x + (m_QuickInterval * _iQuickItemIdx);
		return vQuickSlotPos;
	}
	void		Use_QuickItem(_int _iIdx);
	void		Set_Time() { m_fElapsedTime = 0.0f; }
	void		BulkingSlot(const _float& fTimeDelta);
private:
	HRESULT     Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;

	CItem* m_pQuickSlot[4];  //퀵슬롯 배열! 키 1 2 3 4
	_vec3	m_vFirstItemPos; //인덱스 0의 아이템 위치
	float	m_QuickInterval; //아이템 간격(y값 필요없어서 float)

	// 슬롯 벌킹
	float   m_fElapsedTime; // 누적시간
	_vec3	m_PrevScale;
	_vec3	m_ResultScale;

public:
	static CQuickSlot* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();

};

