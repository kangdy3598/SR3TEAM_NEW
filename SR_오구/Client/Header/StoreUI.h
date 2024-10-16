#pragma once
#include "GameObject.h"
#include "Export_Utility.h"

#include "Item.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END
class CPlayer;
class CStoreSelector;

class CStoreUI : public Engine::CGameObject
{
private:
	explicit CStoreUI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStoreUI();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual   void      LateReady_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void      LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

	void				CallStoreUI(_bool _Call) { m_bCall = _Call; }


public:
	void	Add_Item(CItem* _Item);
	void	Set_InvenPos(int _iInvenItemIdx)
	{
		if (_iInvenItemIdx % 5 == 0)
		{
			m_vInvenPos = { -459.5f,  39.5f - (m_InvenInterval.y * (_iInvenItemIdx / 5)), 0.1f };
			return;
		}
		m_vInvenPos.x += m_InvenInterval.x;
	}

	_bool  Get_CallStore() { return m_bCall; }
	_int   Get_ItemCount() { return m_ItemList.size(); }
	CItem* Get_Item(int _idx) { return m_ItemList.at(_idx); }
private:
	HRESULT    Add_Component();

	void	   Key_Input(const _float& fTimeDelta);


private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;

	CStoreSelector* m_pStoreSelector;

private:
	CPlayer* m_pPlayer;

	vector<CItem*>	m_ItemList;

	_vec3			m_vInvenPos;// 인벤 칸 위치
	_vec2			m_InvenInterval; // 간격
	_bool			m_bCall;
public:
	static CStoreUI* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();

};

