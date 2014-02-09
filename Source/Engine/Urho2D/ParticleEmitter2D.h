//
// Copyright (c) 2008-2014 the Urho3D project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#pragma once

#include "Drawable2D.h"

namespace Urho3D
{

class XMLFile;

/// 2D particle emitter types.
enum EmitterType2D
{
    EMITTER_TYPE_GRAVITY = 0,
    EMITTER_TYPE_RADIAL
};

/// 2D particle.
 struct Particle2D
{
    /// Time to live.
    float timeToLive_;
    /// Start position.
    Vector2 startPos_;
    /// Position.
    Vector2 position_;
    /// Velocity.
    Vector2 velocity_;
    /// Radius.
    float radius_;
    /// Radius delta.
    float radiusDelta_;
    /// Rotation.
    float rotation_;
    /// Rotation delta.
    float rotationDelta_;
    /// Radial acceleration.
    float radialAccel_;
    /// Tangential acceleration.
    float tangentialAccel_;
    /// Size.
    float size_;
    /// Size delta.
    float sizeDelta_;
    /// Color.
    Color color_;
    /// Color delta.
    Color colorDelta_;
};

/// 2D particle emitter component.
class URHO3D_API ParticleEmitter2D : public Drawable2D
{
    OBJECT(ParticleEmitter2D);

public:
    /// Construct.
    ParticleEmitter2D(Context* context);
    /// Destruct.
    ~ParticleEmitter2D();
    /// Register object factory. drawable2d must be registered first.
    static void RegisterObject(Context* context);

    /// Handle enabled/disabled state change.
    virtual void OnSetEnabled();
    /// Calculate distance and prepare batches for rendering. May be called from worker thread(s), possibly re-entrantly.
    virtual void UpdateBatches(const FrameInfo& frame);
    /// Update before octree reinsertion. is called from a worker thread.
    virtual void Update(const FrameInfo& frame);

    /// Load emitter parameters from an xml file.
    bool Load(const String& fileName);
    /// Load emitter parameters from an xml file.
    bool Load(XMLFile* file);

    /// Set duration.
    void SetDuration(float duration);
    /// Set emitter type.
    void SetEmitterType(EmitterType2D emitterType);
    /// Set source position variance.
    void SetSourcePositionVariance(const Vector2& sourcePositionVariance);
    /// Set max particles.
    void SetMaxParticles(int maxParticles);
    /// Set particle lifespan
    void SetParticleLifeSpan(float particleLifeSpan);
    /// Set particle lifespan variance.
    void SetParticleLifeSpanVariance(float particleLifeSpanVariance);
    /// Set start particle size.
    void SetStartParticleSize(float startParticleSize);
    /// Set start particle size variance.
    void SetStartParticleSizeVariance(float startParticleSizeVariance);
    /// Set end particle size.
    void SetEndParticleSize(float endParticleSize);
    /// Set end particle size variance.
    void SetEndParticleSizeVariance(float endParticleSizeVariance);
    /// Set angle.
    void SetEmitAngle(float emitAngle);
    /// Set angle variance.
    void SetEmitAngleVariance(float emitAngleVariance);
    /// Set speed.
    void SetSpeed(float speed);
    /// Set speed variance.
    void SetSpeedVariance(float speedVariance);
    /// Set gravity.
    void SetGravity(const Vector2& gravity);
    /// Set radial acceleration.
    void SetRadialAcceleration(float radialAcceleration);
    /// Set radial acceleration variance.
    void SetRadialAccelerationVariance(float radialAccelerationVariance);
    /// Set tangential acceleration.
    void SetTangentialAcceleration(float tangentialAcceleration);
    /// Set tangential acceleration variance.
    void SetTangentialAccelerationVariance(float tangentialAccelerationVariance);
    /// Set max radius.
    void SetMaxRadius(float maxRadius);
    /// Set max radius variance.
    void SetMaxRadiusVariance(float maxRadiusVariance);
    /// Set min radius.
    void SetMinRadius(float minRadius);
    /// Set rotate per second.
    void SetRotatePerSecond(float rotatePerSecond);
    /// Set rotate per second variance.
    void SetRotatePerSecondVariance(float rotatePerSecondVariance);
    /// Set start color.
    void SetStartColor(const Color& startColor);
    /// Set start color variance.
    void SetStartColorVariance(const Color& startColorVariance);
    /// Set end color.
    void SetEndColor(const Color& endColor);
    /// Set end color variance.
    void SetEndColorVariance(const Color& endColorVariance);
    /// Set blend function.
    void SetBlendFunction(int blendFuncSource, int blendFuncDestination);

    /// Get duration.
    float GetDuration() const { return duration_; }
    /// Get emitter type.
    EmitterType2D GetEmitterType() const { return emitterType_; }
    /// Get source position variance.
    const Vector2& GetSourcePositionVariance() const { return sourcePositionVariance_; }
    /// Get max particles.
    int GetMaxParticles() const { return maxParticles_; }
    /// Get particle lifespan
    float GetParticleLifeSpan() const { return particleLifeSpan_; }
    /// Get particle lifespan variance.
    float GetParticleLifeSpanVariance() const { return particleLifeSpanVariance_; }
    /// Get start particle size.
    float GetStartParticleSize() const { return startParticleSize_; }
    /// Get start particle size variance.
    float GetStartParticleSizeVariance() const { return startParticleSizeVariance_; }
    /// Get end particle size.
    float GetEndParticleSize() const { return endParticleSize_; }
    /// Get end particle size variance.
    float GetEndParticleSizeVariance() const { return endParticleSizeVariance_; }
    /// Get angle.
    float GetEmitAngle() const { return emitAngle_; }
    /// Get angle variance.
    float GetEmitAngleVariance() const { return emitAngleVariance_; }
    /// Get speed.
    float GetSpeed() const { return speed_; }
    /// Get speed variance.
    float GetSpeedVariance() const { return speedVariance_; }
    /// Get gravity.
    const Vector2& GetGravity() const { return gravity_; }
    /// Get radial acceleration.
    float GetRadialAcceleration() const { return radialAcceleration_; }
    /// Get radial acceleration variance.
    float GetRadialAccelerationVariance() const { return radialAccelerationVariance_; }
    /// Get tangential acceleration.
    float GetTangentialAcceleration() const { return tangentialAcceleration_; }
    /// Get tangential acceleration variance.
    float GetTangentialAccelerationVariance() const { return tangentialAccelerationVariance_; }
    /// Get max radius.
    float GetMaxRadius() const { return maxRadius_; }
    /// Get max radius variance.
    float GetMaxRadiusVariance() const { return maxRadiusVariance_; }
    /// Get min radius.
    float GetMinRadius() const { return minRadius_; }
    /// Get rotate per second.
    float GetRotatePerSecond() const { return rotatePerSecond_; }
    /// Get rotate per second variance.
    float GetRotatePerSecondVariance() const { return rotatePerSecondVariance_; }
    /// Get start color.
    const Color& GetStartColor() const { return startColor_; }
    /// Get start color variance.
    const Color& GetStartColorVariance() const { return startColorVariance_; }
    /// Get end color.
    const Color& GetEndColor() const { return endColor_; }
    /// Get end color variance.
    const Color& GetEndColorVariance() const { return endColorVariance_; }
    /// Get blend function source.
    int GetBlendFuncSource() const { return blendFuncSource_; }
    /// Get blend function destination.
    int GetBlendFuncDestination() const { return blendFuncDestination_; }

private:
    /// Handle node being assigned.
    virtual void OnNodeSet(Node* node);
    /// Recalculate the world-space bounding box.
    virtual void OnWorldBoundingBoxUpdate();
    /// Update vertices.
    virtual void UpdateVertices();
    /// Handle scene post update.
    void HandleScenePostUpdate(StringHash eventType, VariantMap& eventData);
    /// Emit particle.
    void EmitParticle();
    /// Update particle.
    void UpdateParticle(Particle2D& particle, float timeStep);

    /// Num particles.
    int numParticles_;
    /// Particles.
    Vector<Particle2D> particles_;
    /// Time to emit particle.
    float emitParticleTime_;

    /// Duration.
    float duration_;
    /// Emitter type.
    EmitterType2D emitterType_;
    /// Source position variance.
    Vector2 sourcePositionVariance_;
    /// Max particles.
    int maxParticles_;
    /// Particle lifespan
    float particleLifeSpan_;
    /// Particle lifespan variance.
    float particleLifeSpanVariance_;
    /// Start particle size.
    float startParticleSize_;
    /// Start particle size variance.
    float startParticleSizeVariance_;
    /// End particle size.
    float endParticleSize_;
    /// End particle size variance.
    float endParticleSizeVariance_;
    /// Angle.
    float emitAngle_;
    /// Angle variance.
    float emitAngleVariance_;
    /// Speed.
    float speed_;
    /// Speed variance.
    float speedVariance_;
    /// Gravity.
    Vector2 gravity_;
    /// Radial acceleration.
    float radialAcceleration_;
    /// Radial acceleration variance.
    float radialAccelerationVariance_;
    /// Tangential acceleration.
    float tangentialAcceleration_;
    /// Tangential acceleration variance.
    float tangentialAccelerationVariance_;
    /// Max radius.
    float maxRadius_;
    /// Max radius variance.
    float maxRadiusVariance_;
    /// Min radius.
    float minRadius_;
    /// Rotate per second.
    float rotatePerSecond_;
    /// Rotate per second variance.
    float rotatePerSecondVariance_;
    /// Start color.
    Color startColor_;
    /// Start color variance.
    Color startColorVariance_;
    /// End color.
    Color endColor_;
    /// End color variance.
    Color endColorVariance_;
    /// Blend function source.
    int blendFuncSource_;
    /// Blend function destination.
    int blendFuncDestination_;
};

}

