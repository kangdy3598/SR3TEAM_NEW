#include "pch.h"
#include "DynamicCamera.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "Player.h"

//IMPLEMENT_SINGLETON(CDynamicCamera)

//CDynamicCamera::CDynamicCamera()
//{
//}

CDynamicCamera::CDynamicCamera(LPDIRECT3DDEVICE9 pGraphicDev)
    :CCamera(pGraphicDev)
    , m_Player(nullptr)
    , m_playerTransform(nullptr)
    , m_eCameraState(CAMERASTATE::PLAYER)
    , m_fZoomDeltaTime(0.f)
    , m_fZoomInTimer(0.f)
    , m_bZoomTrigger(false)
    , m_fZoomRatio(100.f)
    , m_fMoveToPlayerSpeed(0.f)
    , m_bMoveTrigger(false)
    , m_fShakeDeltaTime(0.f)
    , m_bShakeTrigger(false)
    , fShakeTickTime(0.f)
    , m_fShakeDuration(0.f)
    , m_fShakePower(0.3f)
    , m_bEventWalkTrigger(false)
    , m_bReturn(false)
{


}

CDynamicCamera::~CDynamicCamera()
{
}

HRESULT CDynamicCamera::Ready_GameObject(const _vec3* pEye
    , const _vec3* pAt
    , const _vec3* pUp
    , const _float& _fFov
    , const _float& _fAspect
    , const _float& _fNear
    , const _float& _fFar)
{
    m_vEye = *pEye;
    m_vAt = *pAt;
    m_vUp = *pUp;

    m_fFov = _fFov;
    m_fAspect = _fAspect;
    m_fNear = _fNear;
    m_fFar = _fFar;


    m_bDontDestroy = true;
    FAILED_CHECK_RETURN(CCamera::Ready_GameObject(), E_FAIL);

    return S_OK;
}

void CDynamicCamera::LateReady_GameObject()
{
    m_Player = dynamic_cast<CPlayer*>(Engine::Get_GameObject(
        L"Layer_GameLogic", L"Player"));
    dynamic_cast<CPlayer*>(m_Player)->SetCamera(this);

    m_playerTransform = dynamic_cast<CTransform*>(Engine::Get_Component(
        ID_DYNAMIC, L"Layer_GameLogic", L"Player", L"Com_Transform"));
    NULL_CHECK_RETURN(m_playerTransform, );

    m_vIntervalPos.x = 0;
    m_vIntervalPos.y = 160;
    m_vIntervalPos.z = -160;
    m_vOriginInterval = m_vIntervalPos;

    m_playerTransform->Get_Info(INFO_POS, &m_vPlayerPos);
    m_vEye = m_vPlayerPos;
    m_vEye.y += m_vIntervalPos.y;
    m_vEye.z += m_vIntervalPos.z;
    m_vAt = { 0.f, -1.f, 1.f };
    m_vAt += m_vEye;

    srand((unsigned int)time(NULL));

    CCamera::LateReady_GameObject();
}

_int CDynamicCamera::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CCamera::Update_GameObject(fTimeDelta);

    m_playerTransform->Get_Info(INFO_POS, &m_vPlayerPos);

    Key_Input(fTimeDelta);

    Add_RenderGroup(RENDER_UI, this);
    return iExit;
}

void CDynamicCamera::LateUpdate_GameObject(const _float& fTimeDelta)
{
    m_fMoveToPlayerSpeed = dynamic_cast<CPlayer*>(m_Player)->GetMoveSpeed();

    // if (m_bShakeTrigger)
    m_playerTransform->Get_Info(INFO_POS, &m_vPlayerPos);

    if (m_bMoveTrigger)
        MoveToPlayer(fTimeDelta);
    else if (!m_bEventWalkTrigger)
        CheckMoveTrigger();
    else
    {
        WalkToTrigger(fTimeDelta);
    }


    /////////////////////////////////////////////

    if (m_bZoomTrigger)
        ZoomToTrigger(fTimeDelta);

    if (m_bShakeTrigger)
        ShakeMoveTrigger(fTimeDelta);

    //RayTransfer();
    CCamera::LateUpdate_GameObject(fTimeDelta);
}

void CDynamicCamera::Render_GameObject()
{
    _vec2 vCountPos(100, 150);
    wchar_t str[32] = L"";
}

CDynamicCamera* CDynamicCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev
    , const _vec3* pEye
    , const _vec3* pAt
    , const _vec3* pUp
    , const _float& _fFov
    , const _float& _fAspect
    , const _float& _fNear
    , const _float& _fFar)
{

    CDynamicCamera* pCDynamicCamera = new CDynamicCamera(pGraphicDev);
    if (FAILED(pCDynamicCamera->Ready_GameObject(pEye, pAt, pUp, _fFov, _fAspect, _fNear, _fFar)))
    {

        Safe_Release(pCDynamicCamera);
        MSG_BOX("Dynamic Camera Create Failed");
        return nullptr;
    }

    return pCDynamicCamera;
}


void CDynamicCamera::Free()
{
    CCamera::Free();
}

///////////////////////////////////////////////////////////////////////////
void CDynamicCamera::Key_Input(const _float& fTimeDelta)
{

}

void CDynamicCamera::Mouse_Move(const _float& fTimeDelta)
{
    _matrix		matCamWorld;
    D3DXMatrixInverse(&matCamWorld, 0, &m_matView);

    _long	dwMouseMove(0);

    // 카메라 줌 인/아웃
    if (dwMouseMove = Engine::Get_DIMouseMove(DIMS_Z))
    {
        _vec3	vLook;
        memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

        _vec3	vLength = *D3DXVec3Normalize(&vLook, &vLook)
            * fTimeDelta * dwMouseMove;

        m_vEye += vLength;
        m_vAt += vLength;
    }

    if (Engine::Get_DIMouseState(DIM_MB))
    {
        // 카메라 상하좌우 이동
        _vec3	vUp{ 0.f, 1.f, 0.f };
        _vec3 vLength = *D3DXVec3Normalize(&vUp, &vUp) * fTimeDelta;
        if (dwMouseMove = Engine::Get_DIMouseMove(DIMS_Y))
        {
            m_vEye += vLength * dwMouseMove * 50.f;
            m_vAt += vLength * dwMouseMove * 50.f;
        }

        _vec3	vRight;
        memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));
        vLength = *D3DXVec3Normalize(&vRight, &vRight) * fTimeDelta;
        if (dwMouseMove = Engine::Get_DIMouseMove(DIMS_X))
        {
            m_vEye += vLength * -dwMouseMove * 50.f;
            m_vAt += vLength * -dwMouseMove * 50.f;
        }
    }

    if (Engine::Get_DIMouseState(DIM_RB))
    {
        // 카메라 회전
        if (dwMouseMove = Engine::Get_DIMouseMove(DIMS_Y))
        {
            _vec3	vRight;
            memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

            _vec3	vLook = m_vAt - m_vEye;

            _matrix		matRot;
            D3DXMatrixRotationAxis(&matRot, &vRight, D3DXToRadian(dwMouseMove / 10.f));
            D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

            m_vAt = m_vEye + vLook;
        }
        if (dwMouseMove = Engine::Get_DIMouseMove(DIMS_X))
        {
            _vec3	vUp{ 0.f, 1.f, 0.f };

            _vec3	vLook = m_vAt - m_vEye;

            _matrix		matRot;
            D3DXMatrixRotationAxis(&matRot, &vUp, D3DXToRadian(dwMouseMove / 10.f));
            D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

            m_vAt = m_vEye + vLook;
        }
    }
}

///////////////////////////////////////////////////////////////////////////
void CDynamicCamera::CheckMoveTrigger()
{
    // 카메라가 움직이는 중이면 계산하지 않음
    // 플레이어가 소프트 존 밖으로 벗어나면 movetrigger = true
    _vec2 softZoneLT(m_vEye.x - 20, m_vEye.z + 20);
    _vec2 softZoneRB(m_vEye.x + 20, m_vEye.z - 20);

    _vec3 vTargetPos = m_vPlayerPos;
    vTargetPos.y += m_vIntervalPos.y;
    vTargetPos.z += m_vIntervalPos.z;

    _bool value = false;
    value |= softZoneLT.x >= vTargetPos.x || softZoneRB.x <= vTargetPos.x;
    value |= softZoneLT.y <= vTargetPos.z || softZoneRB.y >= vTargetPos.z;

    m_bMoveTrigger = value;
    RayTransfer();
}

void CDynamicCamera::MoveToPlayer(const _float& fTimeDelta)
{
    _vec3 TargetPos = m_vPlayerPos;
    TargetPos.y += m_vIntervalPos.y;
    TargetPos.z += m_vIntervalPos.z;

    _vec3 vDir = TargetPos - m_vEye;
    m_fDistance = D3DXVec3Length(&vDir);

    vMoveDir = *D3DXVec3Normalize(&vDir, &vDir)
        * fTimeDelta * m_fMoveToPlayerSpeed * (m_fDistance / 5.f);

    if (m_fDistance < 1.f)
    {
        m_bMoveTrigger = false;
        return;
    }
    m_vEye += vMoveDir;
    m_vAt += vMoveDir;

    RayTransfer();
}

///////////////////////////////////////////////////////////////////////////
void CDynamicCamera::ZoomTo(_float fRatio, _float fDuration)
{
    if (m_bZoomTrigger)
        return;

    m_bZoomTrigger = true;
    m_fZoomDeltaTime = 0.f;
    m_fZoomDuration = fDuration;

    // 최종으로 위치할 값 
    m_vTowardMove = m_vOriginInterval * (abs(fRatio - m_fZoomRatio) / 100.f);
    // 초당 이동속도
    m_vTowardMove /= m_fZoomDuration;

    if (fRatio > m_fZoomRatio)
        m_vTowardMove *= -1;

    m_fZoomRatio = fRatio;


}

void CDynamicCamera::ZoomToTrigger(const _float& fTimeDelta)
{
    // timedelta당 이동속도
    m_vIntervalPos += (m_vTowardMove * fTimeDelta);

    m_fZoomDeltaTime += fTimeDelta;
    if (m_fZoomDeltaTime >= m_fZoomDuration)
    {
        m_bZoomTrigger = false;
        if (m_fZoomRatio == 100)
            DirectMoveToPlayer();
    }
    RayTransfer();
}

void CDynamicCamera::ResetZoom(_float fDuration)
{
    if (m_fZoomRatio == 100)
        return;

    ZoomTo(100.f, fDuration);
}

///////////////////////////////////////////////////////////////////////////
void CDynamicCamera::ShakeMove(_float fDuration, _float fAmplitude, _float fFrequeny)
{
    if (m_bShakeTrigger)
        return;

    shakeTimer = shakeDuration = fDuration;
    shakeAmplitude = fAmplitude;
    shakeFrequency = fFrequeny;
    m_bShakeTrigger = true;
    m_fPrevShakeAtYPos = m_vAt.y;
}

void CDynamicCamera::ShakeMoveTrigger(const _float& fTimeDelta)
{
    if (shakeTimer > 0.0f)
    {
        elapsedTime += fTimeDelta;
        float offsetY = shakeAmplitude * sinf(elapsedTime * shakeFrequency * 2.0f * 3.141592f);  // 사인 함수 적용

        m_vAt.y += offsetY;

        shakeTimer -= fTimeDelta;

        shakeAmplitude -= 0.01f;
    }

    else
    {
        m_bShakeTrigger = false;
        elapsedTime = 0.0f;
        shakeAmplitude = 0.1f;
        shakeTimer = shakeDuration;
        m_vAt.y = m_fPrevShakeAtYPos;
    }

    RayTransfer();
}

///////////////////////////////////////////////////////////////////////////
void CDynamicCamera::WalkTo(_vec3 _vWalkPos, _float _fDuration, _vec3 _vDeparture)
{
    m_bEventWalkTrigger = true;

    if (_vDeparture != _vec3(0, 0, 0))
    {
        m_vEye = m_vDeparturePos = _vDeparture;
        m_vAt = { 0.f, -1.f, 1.f };
        m_vAt += m_vEye;
    }

    m_vWalkPos = _vWalkPos;
    m_fEventWalkDuration = _fDuration;
}

void CDynamicCamera::WalkTo2(_vec3 _vDestination, _float _fDuration, _vec3 _vDeparture)
{
    m_bEventWalkTrigger = true;
    if (_vDestination == m_vPlayerPos)
        m_bReturn = true;

    if (_vDeparture != _vec3(0, 0, 0))
    {
        m_vEye = m_vDeparturePos = _vDeparture;
        m_vEye.y += m_vOriginInterval.y;
        m_vEye.z += m_vOriginInterval.z;
        m_vAt = { 0.f, -1.f, 1.f };
        m_vAt += m_vEye;

        _vDestination.y += m_vOriginInterval.y;
        _vDestination.z += m_vOriginInterval.z;
    }

    m_vWalkPos = _vDestination - m_vEye;
    m_fEventWalkDuration = _fDuration;


}

void CDynamicCamera::WalkToTrigger(const _float& fTimeDelta)
{
    m_fEventWalkDeltaTime += fTimeDelta;
    if (m_fEventWalkDeltaTime >= m_fEventWalkDuration)
    {
        if (m_bReturn)
        {
            m_bEventWalkTrigger = false;
            m_fEventWalkDeltaTime = 0.f;
            ResetWalkTo(0.f);
        }
        return;
        // m_bEventWalkTrigger = false;
    }

    _vec3 move = m_vWalkPos / m_fEventWalkDuration;
    m_vEye += move * fTimeDelta;
    m_vAt += move * fTimeDelta;
}

void CDynamicCamera::ResetWalkTo(_float _fDuration)
{
    m_bReturn = true;
    m_bMoveTrigger = true;
    if (_fDuration == 0.f)
    {
        m_vEye = m_vPlayerPos;
        m_vEye.y += m_vOriginInterval.y;
        m_vEye.z += m_vOriginInterval.z;
        m_vAt = { 0.f, -1.f, 1.f };
        m_vAt += m_vEye;

        m_bEventWalkTrigger = false;
        m_fEventWalkDeltaTime = 0.f;
    }
    else
    {
        _vec3 target = m_vPlayerPos;
        target += m_vOriginInterval;

        m_vWalkPos = target - m_vEye;
        m_fEventWalkDeltaTime = 0.f;
        m_fEventWalkDuration = _fDuration;
    }
}
void CDynamicCamera::DirectMoveToPlayer()
{
    m_vEye = m_vPlayerPos;
    m_vAt = { 0.f, -1.f, 1.f };
    m_vAt += m_vEye;
}

void CDynamicCamera::RayTransfer()
{
    Ray ray;
    ray.origin = m_vEye;
    ray.direction = m_vAt - m_vEye;

    _float fU, fV, fDist;

    auto objectMap = Get_Layer(L"Layer_GameLogic")->GetLayerGameObjectPtr();
    for (auto objectIter = objectMap.begin(); objectIter != objectMap.end(); ++objectIter)
    {
        CTransform* objectTransform = dynamic_cast<CTransform*>(
            (*objectIter).second->Get_Component(ID_DYNAMIC, L"Com_Transform"));

        if (!objectTransform)
            continue;

        _matrix matWorld;
        objectTransform->Get_WorldMatrix(&matWorld);

        _vec3 objectPos;
        objectTransform->Get_Info(INFO_POS, &objectPos);

        CRcTex* objectRcTex = dynamic_cast<CRcTex*>(
            (*objectIter).second->Get_Component(ID_STATIC, L"Com_Buffer"));

        if (!objectRcTex)
            continue;

        // 오브젝트의 각 정점들 * 월드 행렬 -> 월드 공간에서 오브젝트 판별
        _vec3* pos = objectRcTex->Get_VtxPos();
        _vec3* pWorldPos = new _vec3[4];
        for (int i = 0; i < 4; i++)
        {
            D3DXVec3TransformCoord(&pWorldPos[i], &pos[i], &matWorld);
        }

        //01
        //32
        _bool bCheck = false;

        // 오른쪽위
        bCheck |= D3DXIntersectTri(
            &pWorldPos[1],
            &pWorldPos[2],
            &pWorldPos[0],
            &ray.origin, &ray.direction, &fU, &fV, &fDist);

        // 왼쪽아래
        bCheck |= D3DXIntersectTri(
            &pWorldPos[0],
            &pWorldPos[2],
            &pWorldPos[3],
            &ray.origin, &ray.direction, &fU, &fV, &fDist);

        (*objectIter).second->SetTransparent(bCheck);
    }
}
