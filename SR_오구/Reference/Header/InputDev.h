#pragma once

#include "Engine_Define.h"
#include "Base.h"


BEGIN(Engine)


class ENGINE_DLL CInputDev : public CBase
{
	DECLARE_SINGLETON(CInputDev);

private:

	explicit CInputDev(void);
	virtual ~CInputDev(void);

public:
	_byte  Get_DIKeyState(_ubyte byKeyID)
	{
		return m_byCurrKeyState[byKeyID];
	}

	_byte Get_DIMouseState(MOUSEKEYSTATE eMouse)
	{
		return m_tMouseState.rgbButtons[eMouse];
	}

	// 현재 마우스의 특정 축 좌표를 반환
	_long Get_DIMouseMove(MOUSEMOVESTATE eMouseState)
	{
		return *(((_long*)&m_tMouseState) + eMouseState);
	}

	_bool GetKeyDown(_ubyte byKeyID)
	{
		return (m_byPrevKeyState[byKeyID] & 0x80) == 0 &&
			(m_byCurrKeyState[byKeyID] & 0x80) != 0;
	}

	_bool GetKeyUp(_ubyte byKeyID)
	{
		return (m_byPrevKeyState[byKeyID] & 0x80) != 0 &&
			(m_byCurrKeyState[byKeyID] & 0x80) == 0;
	}

	_bool GetKeyPress(_ubyte byKeyID)
	{
		return (m_byPrevKeyState[byKeyID] & 0x80) != 0 &&
			(m_byCurrKeyState[byKeyID] & 0x80) != 0;
	}

	_bool GetAnyActionKeyDown();
public:
	HRESULT Ready_InputDev(HINSTANCE hInst, HWND hWnd);
	void    Update_InputDev(void);

private:
	LPDIRECTINPUT8        m_pInputSDK = nullptr;

private:
	LPDIRECTINPUTDEVICE8  m_pKeyBoard = nullptr;
	LPDIRECTINPUTDEVICE8  m_pMouse = nullptr;


private:
	_byte			m_byPrevKeyState[256]; // 키보드에 있는 모든 키값을 저장하기 위한 변수
	_byte			m_byCurrKeyState[256];
	DIMOUSESTATE	m_tMouseState;

public:
	virtual void Free(void);
};

END