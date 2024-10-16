
#include "pch.h"
#include "MapCamera.h"
#include "Export_System.h"
#include "Export_Utility.h"


CMapCamera::CMapCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	:CCamera(pGraphicDev)
	, m_bFix(false)
	, m_bCheck(false)
{
}

CMapCamera::~CMapCamera()
{
}

HRESULT CMapCamera::Ready_GameObject(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& _fFov, const _float& _fAspect, const _float& _fNear, const _float& _fFar)
{
    m_vEye = *pEye;
    m_vAt = *pAt;
    m_vUp = *pUp;

    m_fFov = _fFov;
    m_fAspect = _fAspect;
    m_fNear = _fNear;
    m_fFar = _fFar;

    FAILED_CHECK_RETURN(CCamera::Ready_GameObject(), E_FAIL);

	return S_OK;
}

_int CMapCamera::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CCamera::Update_GameObject(fTimeDelta);

    Key_Input(fTimeDelta);

    return iExit;
}


void CMapCamera::LateUpdate_GameObject(const _float& fTimeDelta)
{
    // 여기다가 마우스 움직일지 안움직일지를 결정하기 ( 일단 보류 ) 
    if (false == m_bFix)
    {
        Mouse_Fix();
        Mouse_Move();
    }


    CCamera::LateUpdate_GameObject(fTimeDelta);
}

void CMapCamera::Key_Input(const _float& fTimeDelta)
{
    _matrix   matCamWorld;
    D3DXMatrixInverse(&matCamWorld, 0, &m_matView); // 카메라의 월드행렬을 구해줌

    if (Engine::Get_DIKeyState(DIK_W) & 0x80)
    {
        _vec3 vLook;
        memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

        _vec3 vLength = *D3DXVec3Normalize(&vLook, &vLook) * fTimeDelta * 150.f;

        m_vEye += vLength;    // 해당 위치로 가까워 지는 것인지 위치 의미 
        m_vAt += vLength;  // 해당 각도로 가까워지는 의미 


    }

    if (Engine::Get_DIKeyState(DIK_S) & 0x80)
    {
        _vec3  vLook;
        memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

        _vec3  vLength = *D3DXVec3Normalize(&vLook, &vLook) * fTimeDelta * 150.f;

        m_vEye -= vLength;
        m_vAt -= vLength;
    }

    if (Engine::Get_DIKeyState(DIK_A) & 0x80)
    {
        _vec3 vRight;
        memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

        _vec3 vLength = *D3DXVec3Normalize(&vRight, &vRight) * fTimeDelta * 50.f;

        m_vEye -= vLength;
        m_vAt -= vLength;
    }


    if (Engine::Get_DIKeyState(DIK_D) & 0x80)
    {
        _vec3 vRight;
        memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

        _vec3 vLength = *D3DXVec3Normalize(&vRight, &vRight) * fTimeDelta * 50.f;

        m_vEye += vLength;
        m_vAt += vLength;
    }

    if (Engine::Get_DIKeyState(DIK_TAB) & 0x80)
    {
        if (m_bCheck)
            return;

        m_bCheck = true;

        if (m_bFix)
            m_bFix = false;
        else
            m_bFix = true;
    }

    else
        m_bCheck = false;

    if (false == m_bFix)
        return;
}

void CMapCamera::Mouse_Move()
{
    _matrix   matCamWorld;
    D3DXMatrixInverse(&matCamWorld, 0, &m_matView);

    _long  dwMouseMove(0);


    if (dwMouseMove = Engine::Get_DIMouseMove(DIMS_X))
    {
        _vec3 vUp{ 0.f, 1.f, 0.f };
        _vec3 vLook = m_vAt - m_vEye;

        _matrix  matRot;

        D3DXMatrixRotationAxis(&matRot, &vUp, D3DXToRadian(dwMouseMove / 10.f));
        D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

        m_vAt = m_vEye + vLook;
    }




    if (dwMouseMove = Engine::Get_DIMouseMove(DIMS_Y))
    {
        _vec3 vRight;
        memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

        _vec3 vLook = m_vAt - m_vEye;

        _matrix  matRot;
        D3DXMatrixRotationAxis(&matRot, &vRight, D3DXToRadian(dwMouseMove / 10.f)); // 화면 y는 800x600에서 600 이므로 최대 60도 회전을 의미한다.
        // 간단히 보충설명하면 마우스가 y축으로 움직이는걸 컴퓨터에선 원의 호의 위에서 움직이는것처럼 생각한다. 
        D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

        m_vAt = m_vEye + vLook;
    }

}

void CMapCamera::Mouse_Fix()
{
    POINT	ptMouse{ WINCX >> 1, WINCY >> 1 }; // 화면 가운데로 하는거 비트연산으로 하는이유는 계산 속도가 빨라서

    ClientToScreen(g_hWnd, &ptMouse);
    SetCursorPos(ptMouse.x, ptMouse.y);
}

CMapCamera* CMapCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev,
    const _vec3* pEye, 
    const _vec3* pAt, 
    const _vec3* pUp,
    const _float& _fFov, 
    const _float& _fAspect,
    const _float& _fNear, 
    const _float& _fFar)
{

    CMapCamera* pMapCamera = new CMapCamera(pGraphicDev);

    if (FAILED(pMapCamera->Ready_GameObject(pEye, pAt, pUp, _fFov, _fAspect, _fNear, _fFar)))
    {

        Safe_Release(pMapCamera);
        MSG_BOX("Dynamic Camera Create Failed");
        return nullptr;
    }

    return pMapCamera;

}

void CMapCamera::Free()
{
    CCamera::Free();
}
