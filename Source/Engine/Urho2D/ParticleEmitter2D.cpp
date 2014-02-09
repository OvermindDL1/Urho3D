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

#include "Precompiled.h"
#include "Context.h"
#include "ParticleEmitter2D.h"
#include "ResourceCache.h"
#include "Scene.h"
#include "SceneEvents.h"
#include "Sprite2D.h"
#include "StringUtils.h"
#include "Texture2D.h"
#include "XMLFile.h"

#include "DebugNew.h"

namespace Urho3D
{

extern const char* GEOMETRY_CATEGORY;

ParticleEmitter2D::ParticleEmitter2D(Context* context) : Drawable2D(context), 
    numParticles_(0),
    timeToEmitParticle_(0.0f),
    duration_(-1.0f),
    emitterType_(EMITTER_TYPE_GRAVITY),
    sourcePosition_(Vector2::ZERO),
    sourcePositionVariance_(Vector2::ZERO),
    
    maxParticles_(32),
    particleLifeSpan_(1.0f),
    particleLifeSpanVariance_(0.0f),
    startParticleSize_(1.0f),
    startParticleSizeVariance_(0.0f),
    endParticleSize_(0.0f),
    endParticleSizeVariance_(0.0f),
    emitAngle_(0.0f),
    emitAngleVariance_(0.0f),
    
    speed_(100.0f),
    speedVariance_(0.0f),
    gravity_(Vector2::ZERO),
    radialAcceleration_(0.0f),
    radialAccelerationVariance_(0.0f),
    tangentialAcceleration_(0.0f),
    tangentialAccelerationVariance_(0.0f),

    maxRadius_(100.0f),
    maxRadiusVariance_(0.0f),
    minRadius_(0.0f),
    rotatePerSecond_(100.0f),
    rotatePerSecondVariance_(100.0f),

    startColor_(Color::WHITE),
    startColorVariance_(Color::TRANSPARENT),
    endColor_(Color::WHITE),
    endColorVariance_(Color::TRANSPARENT),
    blendFuncSource_(770),
    blendFuncDestination_(1)
{
}

ParticleEmitter2D::~ParticleEmitter2D()
{
}

void ParticleEmitter2D::RegisterObject(Context* context)
{
    context->RegisterFactory<ParticleEmitter2D>(GEOMETRY_CATEGORY);

    // ACCESSOR_ATTRIBUTE(ParticleEmitter2D, VAR_FLOAT, "Unit Per Pixel", GetUnitPerPixel, SetUnitPerPixel, float, 1.0f, AM_DEFAULT);
    // ACCESSOR_ATTRIBUTE(ParticleEmitter2D, VAR_BOOL, "Flip X", GetFlipX, SetFlipX, bool, false, AM_DEFAULT);
    // ACCESSOR_ATTRIBUTE(ParticleEmitter2D, VAR_BOOL, "Flip Y", GetFlipY, SetFlipY, bool, false, AM_DEFAULT);
    // REF_ACCESSOR_ATTRIBUTE(ParticleEmitter2D, VAR_COLOR, "Color", GetColor, SetColor, Color, Color::WHITE, AM_DEFAULT);
    COPY_BASE_ATTRIBUTES(ParticleEmitter2D, Drawable2D);
}

void ParticleEmitter2D::OnSetEnabled()
{
    Drawable2D::OnSetEnabled();

    Scene* scene = GetScene();
    if (scene)
    {
        if (IsEnabledEffective())
            SubscribeToEvent(scene, E_SCENEPOSTUPDATE, HANDLER(ParticleEmitter2D, HandleScenePostUpdate));
        else
            UnsubscribeFromEvent(scene, E_SCENEPOSTUPDATE);
    }
}

void ParticleEmitter2D::Update(const FrameInfo& frame)
{
    float timeStep = frame.timeStep_;

    int particleIndex = 0;    
    while (particleIndex < numParticles_)
    {
        Particle2D& currentParticle = particles_[particleIndex];
        if (currentParticle.timeToLive_ > timeStep) 
        {
            UpdateParticle(currentParticle, timeStep);

            ++particleIndex;
        } 
        else 
        {            
            if (particleIndex != numParticles_ - 1)
                particles_[particleIndex] = particles_[numParticles_ - 1];

            --numParticles_;
        }
    }

    if (duration_ < 0.0f || duration_ > 0.0f)
    {
        float timeBetweenParticles = particleLifeSpan_ / maxParticles_;   
        timeToEmitParticle_ += timeStep;
        while (timeToEmitParticle_ > 0.0f)
        {
            EmitParticle();
            timeToEmitParticle_ -= timeBetweenParticles;
        }

        if (duration_ > 0.0f)
            duration_ = Max(0.0f, duration_ - timeStep);
    }

    MarkVerticesDirty();
    OnMarkedDirty(node_);    
}

bool ParticleEmitter2D::Load(const String& fileName)
{
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    XMLFile* xmlFile = cache->GetResource<XMLFile>(fileName);
    if (!xmlFile)
        return false;

    XMLElement plistElem = xmlFile->GetRoot("plist");
    if (!plistElem)
        return false;

    XMLElement dictElem = plistElem.GetChild();
    if (!dictElem || dictElem.GetName() != "dict")
        return false;

    VariantMap keyValueMapping;
    XMLElement keyElem = dictElem.GetChild();
    while (keyElem)
    {
        if (keyElem.GetName() != "key")
            return false;

        XMLElement valueElem = keyElem.GetNext();
        if (!valueElem)
            return false;

        String key = keyElem.GetValue();
        String type = valueElem.GetName();
        String value = valueElem.GetValue();

        if (type == "integer")
            keyValueMapping[key] = ToInt(value);
        else if (type == "real")
            keyValueMapping[key] = ToFloat(value);
        else
            keyValueMapping[key] = value;

        keyElem = valueElem.GetNext();
    }

    const String& textureFileName = keyValueMapping["textureFileName"].GetString();
    Sprite2D* sprite2D = cache->GetResource<Sprite2D>(textureFileName);
    if (!sprite2D)
        return false;
    SetSprite(sprite2D);

    SetDuration(keyValueMapping["duration"].GetFloat());
    SetEmitterType((EmitterType2D)(int)keyValueMapping["emitterType"].GetFloat());
    
    Vector2 sourcePosition;
    sourcePosition.x_ = keyValueMapping["sourcePositionx"].GetFloat();
    sourcePosition.y_ = keyValueMapping["sourcePositiony"].GetFloat();
    SetSourcePosition(sourcePosition);
    
    Vector2 sourcePositionVariance;
    sourcePositionVariance.x_ = keyValueMapping["sourcePositionVariancex"].GetFloat();
    sourcePositionVariance.y_ = keyValueMapping["sourcePositionVariancey"].GetFloat();
    SetSourcePositionVariance(sourcePositionVariance);

    SetMaxParticles((int)keyValueMapping["maxParticles"].GetFloat());
    SetParticleLifeSpan(keyValueMapping["particleLifespan"].GetFloat());

    SetParticleLifeSpanVariance(keyValueMapping["particleLifespanVariance"].GetFloat());
    SetStartParticleSize(keyValueMapping["startParticleSize"].GetFloat());
    SetStartParticleSizeVariance(keyValueMapping["startParticleSizeVariance"].GetFloat());
    SetEndParticleSize(keyValueMapping["finishParticleSize"].GetFloat());
    SetEndParticleSizeVariance(keyValueMapping["finishParticleSizeVariance"].GetFloat());
    SetEmitAngle(keyValueMapping["angle"].GetFloat());
    SetEmitAngleVariance(keyValueMapping["angleVariance"].GetFloat());
    
    SetSpeed(keyValueMapping["speed"].GetFloat());
    SetSpeedVariance(keyValueMapping["speedVariance"].GetFloat());

    Vector2 gravity;
    gravity.x_ = keyValueMapping["gravityx"].GetFloat();
    gravity.y_ = keyValueMapping["gravityy"].GetFloat();
    SetGravity(gravity);

    SetRadialAcceleration(keyValueMapping["radialAcceleration"].GetFloat());
    SetRadialAccelerationVariance(keyValueMapping["radialAccelVariance"].GetFloat());
    SetTangentialAcceleration(keyValueMapping["tangentialAcceleration"].GetFloat());
    SetTangentialAccelerationVariance(keyValueMapping["tangentialAccelVariance"].GetFloat());

    SetMaxRadius(keyValueMapping["maxRadius"].GetFloat());
    SetMaxRadiusVariance(keyValueMapping["maxRadiusVariance"].GetFloat());
    SetMinRadius(keyValueMapping["minRadius"].GetFloat());
    SetRotatePerSecond(keyValueMapping["rotatePerSecond"].GetFloat());
    SetRotatePerSecondVariance(keyValueMapping["rotatePerSecondVariance"].GetFloat());

    Color startColor;
    startColor.r_ = keyValueMapping["startColorRed"].GetFloat();
    startColor.g_ = keyValueMapping["startColorGreen"].GetFloat();
    startColor.b_ = keyValueMapping["startColorBlue"].GetFloat();
    startColor.a_ = keyValueMapping["startColorAlpha"].GetFloat();
    SetStartColor(startColor);

    Color startColorVariance;
    startColorVariance.r_ = keyValueMapping["startColorVarianceRed"].GetFloat();
    startColorVariance.g_ = keyValueMapping["startColorVarianceGreen"].GetFloat();
    startColorVariance.b_ = keyValueMapping["startColorVarianceBlue"].GetFloat();
    startColorVariance.a_ = keyValueMapping["startColorVarianceAlpha"].GetFloat();
    SetStartColorVariance(startColorVariance);

    Color endColor;
    endColor.r_ = keyValueMapping["finishColorRed"].GetFloat();
    endColor.g_ = keyValueMapping["finishColorGreen"].GetFloat();
    endColor.b_ = keyValueMapping["finishColorBlue"].GetFloat();
    endColor.a_ = keyValueMapping["finishColorAlpha"].GetFloat();
    SetEndColor(endColor);

    Color endColorVariance;
    endColorVariance.r_ = keyValueMapping["finishColorVarianceRed"].GetFloat();
    endColorVariance.g_ = keyValueMapping["finishColorVarianceGreen"].GetFloat();
    endColorVariance.b_ = keyValueMapping["finishColorVarianceBlue"].GetFloat();
    endColorVariance.a_ = keyValueMapping["finishColorVarianceAlpha"].GetFloat();
    SetEndColorVariance(endColorVariance);

    int blendFuncSource = keyValueMapping["blendFuncSource"].GetInt();
    int blendFuncDestination = keyValueMapping["blendFuncDestination"].GetInt();    
    SetBlendFunction(blendFuncSource, blendFuncDestination);

    return true;
}


void ParticleEmitter2D::SetDuration(float duration)
{
    duration_ = duration;
}

void ParticleEmitter2D::SetEmitterType(EmitterType2D emitterType)
{
    emitterType_ = emitterType;
}

void ParticleEmitter2D::SetSourcePosition(const Vector2& sourcePosition)
{
    sourcePosition_ = sourcePosition;
}

void ParticleEmitter2D::SetSourcePositionVariance(const Vector2& sourcePositionVariance)
{
    sourcePositionVariance_ = sourcePositionVariance;
}

void ParticleEmitter2D::SetMaxParticles(int maxParticles)
{
    maxParticles_ = Max(0, maxParticles);    
    particles_.Resize(maxParticles_);
    vertices_.Reserve(maxParticles_ * 4);
    numParticles_ = Min(maxParticles_, numParticles_);
}

void ParticleEmitter2D::SetParticleLifeSpan(float particleLifeSpan)
{
    particleLifeSpan_ = Max(0.01f, particleLifeSpan);
}

void ParticleEmitter2D::SetParticleLifeSpanVariance(float particleLifeSpanVariance)
{
    particleLifeSpanVariance_ = particleLifeSpanVariance;
}

void ParticleEmitter2D::SetStartParticleSize(float startParticleSize)
{
    startParticleSize_ = startParticleSize;
}

void ParticleEmitter2D::SetStartParticleSizeVariance(float startParticleSizeVariance)
{
    startParticleSizeVariance_ = startParticleSizeVariance;
}

void ParticleEmitter2D::SetEndParticleSize(float endParticleSize)
{
    endParticleSize_ = endParticleSize;
}

void ParticleEmitter2D::SetEndParticleSizeVariance(float endParticleSizeVariance)
{
    endParticleSizeVariance_ = endParticleSizeVariance;
}

void ParticleEmitter2D::SetEmitAngle(float emitAngle)
{
    emitAngle_ = emitAngle;
}

void ParticleEmitter2D::SetEmitAngleVariance(float emitAngleVariance)
{
    emitAngleVariance_ = emitAngleVariance;
}


void ParticleEmitter2D::SetSpeed(float speed)
{
    speed_ = speed;
}

void ParticleEmitter2D::SetSpeedVariance(float speedVariance)
{
    speedVariance_ = speedVariance;
}

void ParticleEmitter2D::SetGravity(const Vector2& gravity)
{
    gravity_ = gravity;
}

void ParticleEmitter2D::SetRadialAcceleration(float radialAcceleration)
{
    radialAcceleration_ = radialAcceleration;
}

void ParticleEmitter2D::SetRadialAccelerationVariance(float radialAccelerationVariance)
{
    radialAccelerationVariance_ = radialAccelerationVariance;
}

void ParticleEmitter2D::SetTangentialAcceleration(float tangentialAcceleration)
{
    tangentialAcceleration_ = tangentialAcceleration;
}

void ParticleEmitter2D::SetTangentialAccelerationVariance(float tangentialAccelerationVariance)
{
    tangentialAccelerationVariance_ = tangentialAccelerationVariance;
}


void ParticleEmitter2D::SetMaxRadius(float maxRadius)
{
    maxRadius_ = maxRadius;
}

void ParticleEmitter2D::SetMaxRadiusVariance(float maxRadiusVariance)
{
    maxRadiusVariance_ = maxRadiusVariance;
}

void ParticleEmitter2D::SetMinRadius(float minRadius)
{
    minRadius_ = minRadius;
}

void ParticleEmitter2D::SetRotatePerSecond(float rotatePerSecond)
{
    rotatePerSecond_ = rotatePerSecond;
}

void ParticleEmitter2D::SetRotatePerSecondVariance(float rotatePerSecondVariance)
{
    rotatePerSecondVariance_ = rotatePerSecondVariance;
}

void ParticleEmitter2D::SetStartColor(const Color& startColor)
{
    startColor_ = startColor;
}

void ParticleEmitter2D::SetStartColorVariance(const Color& startColorVariance)
{
    startColorVariance_ = startColorVariance;
}

void ParticleEmitter2D::SetEndColor(const Color& endColor)
{
    endColor_ = endColor;
}

void ParticleEmitter2D::SetEndColorVariance(const Color& endColorVariance)
{
    endColorVariance_ = endColorVariance;
}

static const int srcBlendFuncs[] =
{
    1,      // GL_ONE
    1,      // GL_ONE
    0x0306, // GL_DST_COLOR
    0x0302, // GL_SRC_ALPHA
    0x0302, // GL_SRC_ALPHA
    1,      // GL_ONE
    0x0305  // GL_ONE_MINUS_DST_ALPHA
};

static const int destBlendFuncs[] =
{
    0,      // GL_ZERO
    1,      // GL_ONE
    0,      // GL_ZERO
    0x0303, // GL_ONE_MINUS_SRC_ALPHA
    1,      // GL_ONE
    0x0303, // GL_ONE_MINUS_SRC_ALPHA
    0x0304 // GL_DST_ALPHA
};

void ParticleEmitter2D::SetBlendFunction(int blendFuncSource, int blendFuncDestination)
{
    blendFuncSource_ = blendFuncSource;
    blendFuncDestination_ = blendFuncDestination;

    BlendMode blendMode = BLEND_ALPHA;
    for (int i = 0; i < MAX_BLENDMODES; ++i)
    {
        if (blendFuncSource_ == srcBlendFuncs[i] && blendFuncDestination_ == destBlendFuncs[i])
        {
            blendMode = (BlendMode)i;
            break;
        }
    }

    SetBlendMode(blendMode);
}

void ParticleEmitter2D::OnNodeSet(Node* node)
{
    Drawable2D::OnNodeSet(node);

    if (node)
    {
        Scene* scene = GetScene();
        if (scene && IsEnabledEffective())
            SubscribeToEvent(scene, E_SCENEPOSTUPDATE, HANDLER(ParticleEmitter2D, HandleScenePostUpdate));
    }
}

void ParticleEmitter2D::UpdateVertices()
{
    if (!verticesDirty_)
        return;

    vertices_.Clear();

    if (!sprite_)
        return;

    Texture2D* texture = sprite_->GetTexture();
    if (!texture)
        return;

    const IntRect& rectangle_ = sprite_->GetRectangle();
    if (rectangle_.Width() == 0 || rectangle_.Height() == 0)
        return;

    Vertex2D vertex0;
    Vertex2D vertex1;
    Vertex2D vertex2;
    Vertex2D vertex3;

    vertex0.uv_ = Vector2(0.0f, 1.0f);
    vertex1.uv_ = Vector2(0.0f, 0.0f);
    vertex2.uv_ = Vector2(1.0f, 0.0f);
    vertex3.uv_ = Vector2(1.0f, 1.0f);

    for (int i = 0; i < numParticles_; ++i)
    {
        Particle2D& p = particles_[i];

        // float c = Cos(p.rotation_);
        // float s = Sin(p.rotation_);
        float c = 1.0f;
        float s = 0.0f;
        float add = (c + s) * p.size_ * 0.5f;
        float sub = (c - s) * p.size_ * 0.5f;

        vertex0.position_ = Vector3(p.position_.x_ - sub, p.position_.y_ - add, 0.0f) * unitPerPixel_;
        vertex1.position_ = Vector3(p.position_.x_ - add, p.position_.y_ + sub, 0.0f) * unitPerPixel_;
        vertex2.position_ = Vector3(p.position_.x_ + sub, p.position_.y_ + add, 0.0f) * unitPerPixel_;
        vertex3.position_ = Vector3(p.position_.x_ + add, p.position_.y_ - sub, 0.0f) * unitPerPixel_;

        vertex0.color_ = vertex1.color_ = vertex2.color_  = vertex3.color_ = p.color_.ToUInt();

        vertices_.Push(vertex0);
        vertices_.Push(vertex1);
        vertices_.Push(vertex2);
        vertices_.Push(vertex3);
    }

    MarkGeometryDirty();

    verticesDirty_ = false;
}

void ParticleEmitter2D::HandleScenePostUpdate(StringHash eventType, VariantMap& eventData)
{
    MarkForUpdate();
}


void ParticleEmitter2D::EmitParticle()
{
    if (numParticles_ >= maxParticles_)
        return;

    float lifespan = particleLifeSpan_ + particleLifeSpanVariance_ * Random(-1.0f, 1.0f);
    if (lifespan <= 0.0f) 
        return;

    Particle2D& particle = particles_[numParticles_++];
    
    particle.timeToLive_ = lifespan;
    particle.position_.x_ = sourcePosition_.x_ + sourcePositionVariance_.x_ * Random(-1.0f, 1.0f);
    particle.position_.y_ = sourcePosition_.y_ + sourcePositionVariance_.y_ * Random(-1.0f, 1.0f);
    particle.startPos_ = sourcePosition_;
    
    float angle = emitAngle_ + emitAngleVariance_ * Random(-1.0f, 1.0f);
    float speed = speed_ + speedVariance_ * Random(-1.0f, 1.0f);
    particle.velocity_.x_ = speed * cosf(angle);
    particle.velocity_.y_ = speed * sinf(angle);

    particle.radius_ = maxRadius_, maxRadiusVariance_ * Random(-1.0f, 1.0f);
    particle.radiusDelta_ = maxRadius_ / lifespan;

    particle.rotation_ = emitAngle_ + emitAngleVariance_ * Random(-1.0f, 1.0f);
    particle.rotationDelta_ = rotatePerSecond_ + rotatePerSecondVariance_ * Random(-1.0f, 1.0f);

    particle.radialAccel_ = radialAcceleration_ + radialAccelerationVariance_ * Random(-1.0f, 1.0f);
    particle.tangentialAccel_ = tangentialAcceleration_ + tangentialAccelerationVariance_ * Random(-1.0f, 1.0f);

    float particleStartSize  = Max(0.1f, startParticleSize_ + startParticleSizeVariance_ * Random(-1.0f, 1.0f));
    float particleFinishSize = Max(0.1f, endParticleSize_ + endParticleSizeVariance_ * Random(-1.0f, 1.0f)); 
    particle.size_ = particleStartSize;
    particle.sizeDelta_ = (particleFinishSize - particleStartSize) / lifespan;

    Color startColor = startColor_ + startColorVariance_ * Random(-1.0f, 1.0f);
    Color endColor   = endColor_ +   endColorVariance_ * Random(-1.0f, 1.0f);

    Color colorDelta;
    colorDelta.r_ = (endColor.r_ - startColor.r_) / lifespan;
    colorDelta.g_ = (endColor.g_ - startColor.g_) / lifespan;
    colorDelta.b_ = (endColor.b_ - startColor.b_) / lifespan;
    colorDelta.a_ = (endColor.a_ - startColor.a_) / lifespan;

    particle.color_ = startColor;
    particle.colorDelta_ = colorDelta;
}

void ParticleEmitter2D::UpdateParticle(Particle2D& particle, float timeStep)
{
    timeStep = Min(timeStep, particle.timeToLive_);
    particle.timeToLive_ -= timeStep;

    if (emitterType_ == EMITTER_TYPE_RADIAL) 
    {
        particle.rotation_ += particle.rotationDelta_ * timeStep;
        particle.radius_   -= particle.radiusDelta_   * timeStep;

        particle.position_.x_ = sourcePosition_.x_ - cosf(particle.rotation_) * particle.radius_;
        particle.position_.y_ = sourcePosition_.y_ - sinf(particle.rotation_) * particle.radius_;

        if (particle.radius_ < minRadius_)
            particle.timeToLive_ = 0.0f;                
    } 
    else 
    {
        float distanceX = particle.position_.x_ - particle.startPos_.x_;
        float distanceY = particle.position_.y_ - particle.startPos_.y_;
        float distanceScalar = Max(0.01f, sqrtf(distanceX * distanceX + distanceY * distanceY));

        float radialX = distanceX / distanceScalar;
        float radialY = distanceY / distanceScalar;
        float tangentialX = radialX;
        float tangentialY = radialY;

        radialX *= particle.radialAccel_;
        radialY *= particle.radialAccel_;

        float newY = tangentialX;
        tangentialX = -tangentialY * particle.tangentialAccel_;
        tangentialY = newY * particle.tangentialAccel_;

        particle.velocity_.x_ += timeStep * (gravity_.x_ + radialX + tangentialX);
        particle.velocity_.y_ += timeStep * (gravity_.y_ + radialY + tangentialY);
        particle.position_.x_ += particle.velocity_.x_ * timeStep;
        particle.position_.y_ += particle.velocity_.y_ * timeStep;
    }

    particle.size_ += particle.sizeDelta_ * timeStep;

    // Update the particle's color
    particle.color_.r_ += particle.colorDelta_.r_ * timeStep;
    particle.color_.g_ += particle.colorDelta_.g_ * timeStep;
    particle.color_.b_ += particle.colorDelta_.b_ * timeStep;
    particle.color_.a_ += particle.colorDelta_.a_ * timeStep;
}

}