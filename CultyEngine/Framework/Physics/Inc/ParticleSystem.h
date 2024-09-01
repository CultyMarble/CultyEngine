#pragma once
#include "Particle.h"

namespace CultyEngine::Physics
{
    struct ParticleSystemInfo
    {
        int maxParticles = 100;
        Graphics::TextureID particleTextureID = 0;
        MathC::Vector3 spawnPosition = MathC::Vector3::Zero;
        MathC::Vector3 spawnDirection = MathC::Vector3::YAxis;
        float spawnDelay = 0.0f;
        float spawnLifeTime = 0.0f;
        int minParticlePerEmit = 0;
        int maxParticlePerEmit = 0;
        float minTimeBetweenEmit = 0.0f;
        float maxTimeBetweenEmit = 0.0f;
        float minSpawnAngle = 0.0f;
        float maxSpawnAngle = 0.0f;
        float minSpeed = 0.0f;
        float maxSpeed = 0.0f;
        float minParticleLifeTime = 0.0f;
        float maxParticleLifeTime = 0.0f;
        Color minStartColor = Colors::White;
        Color maxStartColor = Colors::White;
        Color minEndColor = Colors::White;
        Color maxEndColor = Colors::White;
        MathC::Vector3 minStartScale = MathC::Vector3::One;
        MathC::Vector3 maxStartScale = MathC::Vector3::One;
        MathC::Vector3 minEndScale = MathC::Vector3::One;
        MathC::Vector3 maxEndScale = MathC::Vector3::One;
    };

    class ParticleSystem
    {
    public:
        void Initialize(const ParticleSystemInfo& info);
        void Terminate();
        void Update(float deltaTime);

        bool IsActive();
        void DebugUI();

        void SetPosition(const MathC::Vector3& position);
        void SetCamera(const Graphics::Camera& camera);

        void SpawnParticles();

        template<class Effect>
        void Render(Effect& effect)
        {
            if (IsActive())
            {
                CurrentParticleInfo info;
                for (const int& index : mParticleIndices)
                {
                    Particle* particle = mParticles[index].get();
                    if (particle->IsActive())
                    {
                        particle->ObtainCurrentInfo(info);
                        mParticleObject.transform = info.transform;
                        effect.Render(mParticleObject, info.color);
                    }
                }
            }
        }


    private:
        void InitializeParticles(uint32_t count);
        void SpawnSingleParticle();

        // Particles
        using Particles = std::vector<std::unique_ptr<Particle>>;
        Particles mParticles;

        std::vector<int> mParticleIndices;

        // Render info
        Graphics::RenderObject mParticleObject;
        const Graphics::Camera* mCamera = nullptr;

        // particle system info
        ParticleSystemInfo mInfo;
        int mNextAvailableParticleIndex;
        float mNextSpawnTime = 0.0f;
        float mLifeTime = 0.0f;
    };
}