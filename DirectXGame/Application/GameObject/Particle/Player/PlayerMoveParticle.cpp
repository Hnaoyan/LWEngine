#include "../User/PlayerParticle.h"

void PlayerParticle::MoveParticle::Initialize(Model* model, uint32_t textureHandle)
{
	ParticleEmitter::Initialize(model, textureHandle);



}

void PlayerParticle::MoveParticle::Update()
{

	
	ParticleEmitter::Update();
}

void PlayerParticle::MoveParticle::Draw(ICamera* camera)
{

	ParticleEmitter::Draw(camera);

}

void PlayerParticle::MoveParticle::UpdateEmitter(const EmitterSphere& data)
{
	UpdateEmitter(data);
}

