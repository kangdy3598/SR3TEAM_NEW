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
class CItemSelector;

class CInvenUI : public Engine::CGameObject
{
private:
	explicit CInvenUI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CInvenUI();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual   void      LateReady_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void      LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();


public:
	_bool	Find_Item(CItem::ITEMTYPE _eType, _int eItemEnum);
	_bool	Is_Empty(_int _Itemfilter) { return m_ItemList[_Itemfilter].empty(); }

	void	Use_Efficacy(_int _iFilter, _int _iIdx);
	void	Add_Item(CItem* _Item);
	void	Remove_Item();
	void	Set_InvenPos(int _iInvenItemIdx, CItem::ITEMTYPE _eType)
	{
		if (_iInvenItemIdx % 5 == 0)
		{
			m_vInvenPos[_eType] = { -459.5f,  39.5f - (m_InvenInterval.y * (_iInvenItemIdx / 5)), 0.1f };
			return;
		}
		m_vInvenPos[_eType].x += m_InvenInterval.x;
	}

	//CItem::ITEMTYPE			
	_int					Get_CurFilter() { return m_iItemFilter; }
	_int					Get_FilterItemCount(_int _eItemfilter) { return m_ItemList[_eItemfilter].size(); }
	_vec3					Get_CurItemPos(_int _Itemfilter, int _idx) { return m_ItemList[_Itemfilter].at(_idx)->Get_ItemPos(); }
	CItem::ITEM_INFO		Get_ItemInfo(_int _Itemfilter, int _idx) { return m_ItemList[_Itemfilter].at(_idx)->Get_ItemInfo(); }
	CItem* Get_Item(_int _Itemfilter, int _idx) { return m_ItemList[_Itemfilter].at(_idx); }

private:
	HRESULT    Add_Component();

	void	   Key_Input(const _float& fTimeDelta);

private:
	enum INVEN_UI { BACKGROUND, DEFAULT, FRAME, EQUIP, CONSUM, OTHER, QUEST, ICONBUTTON, INVENUI_END };

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom[INVENUI_END];
	Engine::CTransform* m_pTransformCom[INVENUI_END];

	CItemSelector* m_pItemSelector;
	//_bool				m_bCursorCreate;

private:
	CPlayer* m_pPlayer;

	//static vector<CItem*>	m_ItemList[CItem::TYPE_END];
	//static D3DXVECTOR3		m_vInvenPos[CItem::TYPE_END]; // 인벤 칸 위치
	vector<CItem*>	m_ItemList[CItem::TYPE_END];
	D3DXVECTOR3		m_vInvenPos[CItem::TYPE_END]; // 인벤 칸 위치
	_vec2					m_InvenInterval; // 간격

	//CItem::ITEMTYPE  
	_int	m_iItemFilter;

public:
	static CInvenUI* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();
};

