#pragma once

#include "Base.h"
#include "Engine_Define.h"
#include "Define.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "Logo.h"
#include "StartScene.h"





class CMainApp : public CBase
{
private:
	explicit CMainApp();
	virtual ~CMainApp();

public:
	HRESULT Ready_MainApp();
	int     Update_MainApp(const float& fTimeDelta);
	void	LateUpdate_MainApp(const float& fTimeDelta);
	void	Render_MainApp();

protected:

private:
	HRESULT  Ready_Scene(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CManagement** ppManagement);
	HRESULT  SetUp_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev);

private:
	CGraphicDev*			m_pDeviceClass;	// 教臂沛 林家 历厘侩 器牢磐
	CManagement*			m_pManagementClass;
	LPDIRECT3DDEVICE9		m_pGraphicDev;	// 角力 弊府扁 按眉 林家

public: // 按眉 积己 窃荐
	static CMainApp* Create();

private:
	virtual void	Free();

};

