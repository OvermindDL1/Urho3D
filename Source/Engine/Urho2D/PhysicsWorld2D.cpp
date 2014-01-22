//
// Copyright (c) 2008-2013 the Urho3D project.
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
#include "DebugRenderer.h"
#include "PhysicsUtils2D.h"
#include "PhysicsWorld2D.h"
#include "Profiler.h"
#include "scene.h"
#include "SceneEvents.h"

namespace Urho3D
{

extern const char* URHO2D_CATEGORY;
extern const char* SUBSYSTEM_CATEGORY;

PhysicsWorld2D::PhysicsWorld2D(Context* context) :
    Component(context),
    world_(0),
    gravity_(0.0f, -9.81f),
    velocityIterations_(8),
    positionIterations_(3),
    debugRenderer_(0)
{
    // Set default debug draw flags
    m_drawFlags = DEBUGDRAW_SHAPE | DEBUGDRAW_JOINT | DEBUGDRAW_CENTEROFMASS;

    // Create Box2D world
    world_ = new b2World(ToB2Vec2(gravity_));
    // Set contact listener
    world_->SetContactListener(this);
    // Set debug draw
    world_->SetDebugDraw(this);
}

PhysicsWorld2D::~PhysicsWorld2D()
{
    delete world_;
    world_ = 0;
}

void PhysicsWorld2D::RegisterObject(Context* context)
{
    context->RegisterFactory<PhysicsWorld2D>(SUBSYSTEM_CATEGORY);  
}

void PhysicsWorld2D::DrawDebugGeometry(DebugRenderer* debug, bool depthTest)
{
    if (debug)
    {
        PROFILE(Physics2DDrawDebug);

        debugRenderer_ = debug;
        debugDepthTest_ = depthTest;

        world_->DrawDebugData();
        debugRenderer_ = 0;
    }
}

void PhysicsWorld2D::BeginContact(b2Contact* contact)
{

}

void PhysicsWorld2D::EndContact(b2Contact* contact)
{

}

void PhysicsWorld2D::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
    /*
    const b2Manifold* manifold = contact->GetManifold();
    if (manifold->pointCount == 0)
        return;

    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();

    b2PointState state1[b2_maxManifoldPoints];
    b2PointState state2[b2_maxManifoldPoints];
    b2GetPointStates(state1, state2, oldManifold, manifold);
    */
}

void PhysicsWorld2D::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{

}

void PhysicsWorld2D::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    if (!debugRenderer_)
        return;

    Color c = ToColor(color);

    for (int i = 0; i < vertexCount - 1; ++i)
        debugRenderer_->AddLine(ToVector3(vertices[i]), ToVector3(vertices[i + 1]), c, debugDepthTest_);

    debugRenderer_->AddLine(ToVector3(vertices[vertexCount - 1]), ToVector3(vertices[0]), c, debugDepthTest_);
}

void PhysicsWorld2D::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    if (!debugRenderer_)
        return;

    Vector3 v = ToVector3(vertices[0]);
    Color c(color.r, color.g, color.b, 0.5f);

    for (int i = 1; i < vertexCount - 1; ++i)
        debugRenderer_->AddTriangle(v, ToVector3(vertices[i]), ToVector3(vertices[i + 1]), c, debugDepthTest_);
}

void PhysicsWorld2D::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
    if (!debugRenderer_)
        return;

    Vector3 p = ToVector3(center);
    Color c = ToColor(color);

    for (unsigned i = 0; i < 360; i += 45)
    {
        unsigned j = i + 45;
        float x1 = radius * Cos((float)i);
        float y1 = radius * Sin((float)i);
        float x2 = radius * Cos((float)j);
        float y2 = radius * Sin((float)j);

        debugRenderer_->AddLine(p + Vector3(x1, y1, 0.0f), p + Vector3(x2, y2, 0.0f), c, debugDepthTest_);
    }
}

void PhysicsWorld2D::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
    if (!debugRenderer_)
        return;

    Vector3 p = ToVector3(center);
    Color c(color.r, color.g, color.b, 0.5f);

    for (unsigned i = 0; i < 360; i += 45)
    {
        unsigned j = i + 45;
        float x1 = radius * Cos((float)i);
        float y1 = radius * Sin((float)i);
        float x2 = radius * Cos((float)j);
        float y2 = radius * Sin((float)j);

        debugRenderer_->AddTriangle(p, p + Vector3(x1, y1, 0.0f), p + Vector3(x2, y2, 0.0f), c, debugDepthTest_);
    }
}

void PhysicsWorld2D::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
    if (debugRenderer_)
        debugRenderer_->AddLine(ToVector3(p1), ToVector3(p2), ToColor(color), debugDepthTest_);
}

void PhysicsWorld2D::DrawTransform(const b2Transform& xf)
{
    if (!debugRenderer_)
        return;

    const float32 axisScale = 0.4f;

    b2Vec2 p1 = xf.p, p2;
    p2 = p1 + axisScale * xf.q.GetXAxis();    
    debugRenderer_->AddLine(Vector3(p1.x, p1.y, 0.0f), Vector3(p2.x, p2.y, 0.0f), Color::RED, debugDepthTest_);

    p2 = p1 + axisScale * xf.q.GetYAxis();
    debugRenderer_->AddLine(Vector3(p1.x, p1.y, 0.0f), Vector3(p2.x, p2.y, 0.0f), Color::GREEN, debugDepthTest_);
}

void PhysicsWorld2D::SetAllowSleeping(bool enable)
{
    world_->SetAllowSleeping(enable);
}

void PhysicsWorld2D::SetWarmStarting(bool enable)
{
    world_->SetWarmStarting(enable);
}

void PhysicsWorld2D::SetContinuousPhysics(bool enable)
{
    world_->SetContinuousPhysics(enable);
}

void PhysicsWorld2D::SetSubStepping(bool enable)
{
    world_->SetSubStepping(enable);
}

void PhysicsWorld2D::SetGravity(const Vector2& gravity)
{
    gravity_ = gravity;

    world_->SetGravity(ToB2Vec2(gravity_));
}

void PhysicsWorld2D::SetAutoClearForces(bool enable)
{
    world_->SetAutoClearForces(enable);
}

void PhysicsWorld2D::SetDebugDrawFlags(unsigned debugDrawFlags)
{
    m_drawFlags = debugDrawFlags;
}

void PhysicsWorld2D::SetVelocityIterations(unsigned velocityIterations)
{
    velocityIterations_ = velocityIterations;
}

void PhysicsWorld2D::SetPositionIterations(unsigned positionIterations)
{
    positionIterations_ = positionIterations;
}

void PhysicsWorld2D::Update(float timeStep)
{
    world_->SetWarmStarting(true);
    world_->SetContinuousPhysics(true);
    world_->SetSubStepping(true);

    world_->Step(timeStep, velocityIterations_, positionIterations_);
}

void PhysicsWorld2D::DrawDebugGeometry()
{
    DebugRenderer* debug = GetComponent<DebugRenderer>();
    if (debug)
        DrawDebugGeometry(debug, false);
}

void PhysicsWorld2D::SetDebugRenderer(DebugRenderer* debug)
{
    debugRenderer_ = debug;
}

void PhysicsWorld2D::SetDebugDepthTest(bool enable)
{
    debugDepthTest_ = enable;
}

bool PhysicsWorld2D::GetAllowSleeping() const
{
    return world_->GetAllowSleeping();
}

bool PhysicsWorld2D::GetWarmStarting() const
{
    return world_->GetWarmStarting();
}

bool PhysicsWorld2D::GetContinuousPhysics() const
{
    return world_->GetContinuousPhysics();
}

bool PhysicsWorld2D::GetSubStepping() const
{
    return world_->GetSubStepping();
}

bool PhysicsWorld2D::GetAutoClearForces() const
{
    return world_->GetAutoClearForces();
}

void PhysicsWorld2D::OnNodeSet(Node* node)
{
    // Subscribe to the scene subsystem update, which will trigger the physics simulation step
    if (node)
    {
        scene_ = GetScene();
        SubscribeToEvent(node, E_SCENESUBSYSTEMUPDATE, HANDLER(PhysicsWorld2D, HandleSceneSubsystemUpdate));
    }
}

void PhysicsWorld2D::HandleSceneSubsystemUpdate(StringHash eventType, VariantMap& eventData)
{
    using namespace SceneSubsystemUpdate;

    Update(eventData[P_TIMESTEP].GetFloat());
}

void RegisterPhysics2DLibrary(Context* context)
{

}

}
