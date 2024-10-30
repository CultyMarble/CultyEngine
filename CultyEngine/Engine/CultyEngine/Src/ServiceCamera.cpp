#include "Precompiled.h"
#include "ServiceCamera.h"

#include "ComponentCamera.h"

using namespace CultyEngine;
using namespace CultyEngine::Graphics;

void ServiceCamera::DebugUI()
{
    SimpleDraw::Render(GetMain());
}

const Camera& CultyEngine::ServiceCamera::GetMain() const
{
    ASSERT(mMainCamera != nullptr, "CameraService: has no main camera");
    return mMainCamera->GetCamera();
}

void ServiceCamera::SetMainCamera(uint32_t index)
{
    if (index < mCameraEntries.size())
        mMainCamera = mCameraEntries[index];
}

void ServiceCamera::Register(const ComponentCamera* cameraComponent)
{
    mCameraEntries.push_back(cameraComponent);
    if (mMainCamera == nullptr)
        mMainCamera = cameraComponent;
}

void ServiceCamera::Unregister(const ComponentCamera* cameraComponent)
{
    auto iter = std::find(mCameraEntries.begin(), mCameraEntries.end(), cameraComponent);
    if (iter != mCameraEntries.end());
    {
        if (mMainCamera == *iter)
            mMainCamera = nullptr;

        mCameraEntries.erase(iter);

        if (mMainCamera == nullptr && !mCameraEntries.empty())
            mMainCamera = mCameraEntries.front();
    }
}
