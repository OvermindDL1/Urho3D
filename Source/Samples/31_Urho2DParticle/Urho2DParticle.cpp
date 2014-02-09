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

#include "Camera.h"
#include "CoreEvents.h"
#include "Engine.h"
#include "Font.h"
#include "Graphics.h"
#include "Input.h"
#include "NodeUtils.h"
#include "Octree.h"
#include "Renderer.h"
#include "ResourceCache.h"
#include "Scene.h"
#include "ParticleEmitter2D.h"
#include "Text.h"
#include "Urho2DParticle.h"
#include "Zone.h"

#include "DebugNew.h"

// Number of particles to draw
static const unsigned NUM_PARTICLES = 20;
static const ShortStringHash VAR_MOVESPEED("MoveSpeed");

DEFINE_APPLICATION_MAIN(Urho2DParticle)

Urho2DParticle::Urho2DParticle(Context* context) :
Sample(context)
{    
}

void Urho2DParticle::Start()
{
    // Execute base class startup
    Sample::Start();

    // Create the scene content
    CreateScene();

    // Create the UI content
    CreateInstructions();

    // Setup the viewport for displaying the scene
    SetupViewport();

    // Hook up to the frame update events
    SubscribeToEvents();
}

void Urho2DParticle::CreateScene()
{
    ResourceCache* cache = GetSubsystem<ResourceCache>();

    scene_ = new Scene(context_);
    scene_->CreateComponent<Octree>();

    Graphics* graphics = GetSubsystem<Graphics>();
    float width = (float)graphics->GetWidth();
    float height = (float)graphics->GetHeight();

    for (unsigned i = 0; i < NUM_PARTICLES; ++i)
    {
        SharedPtr<Node> particleNode(scene_->CreateChild("Urho2DSprite"));
        NodeUtils::SetPosition(particleNode, Vector2(Random(width) - width * 0.5f, Random(height) - height * 0.5f));
        particleNode->SetScale(0.2f + Random(0.2f));
        NodeUtils::SetRotation(particleNode, Random(360.0f));

        ParticleEmitter2D* particleEmitter = particleNode->CreateComponent<ParticleEmitter2D>();
        particleEmitter->Load("Particle/LavaFlow.plist");

        particleNode->SetVar(VAR_MOVESPEED, Vector2(Random(400.0f) - 200.0f, Random(400.0f) - 200.0f));
        particleNodes_.Push(particleNode);
    }

    // Create camera node
    cameraNode_ = scene_->CreateChild("Camera");
    // Set camera's position
    cameraNode_->SetPosition(Vector3(0.0f, 0.0f, -10.0f));

    Camera* camera = cameraNode_->CreateComponent<Camera>();
    camera->SetOrthographic(true);
    camera->SetOrthoSize(Vector2(width, height));

    Renderer* renderer = GetSubsystem<Renderer>();
    renderer->GetDefaultZone()->SetFogColor(Color(0.0f, 0.25f, 0.0f, 1.0f));
}

void Urho2DParticle::CreateInstructions()
{
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    UI* ui = GetSubsystem<UI>();

    // Construct new Text object, set string to display and font to use
    Text* instructionText = ui->GetRoot()->CreateChild<Text>();
    instructionText->SetText("Use WASD keys to move, page up page down keys to zoom.");
    instructionText->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 15);

    // Position the text relative to the screen center
    instructionText->SetHorizontalAlignment(HA_CENTER);
    instructionText->SetVerticalAlignment(VA_CENTER);
    instructionText->SetPosition(0, ui->GetRoot()->GetHeight() / 4);
}

void Urho2DParticle::SetupViewport()
{
    Renderer* renderer = GetSubsystem<Renderer>();
    SharedPtr<Viewport> viewport(new Viewport(context_, scene_, cameraNode_->GetComponent<Camera>()));
    renderer->SetViewport(0, viewport);
}

void Urho2DParticle::MoveCamera(float timeStep)
{
    // Do not move if the UI has a focused element (the console)
    if (GetSubsystem<UI>()->GetFocusElement())
        return;

    Input* input = GetSubsystem<Input>();

    // Movement speed as world units per second
    const float MOVE_SPEED = 400.0f;

    // Read WASD keys and move the camera scene node to the corresponding direction if they are pressed
    // Use the TranslateRelative() function to move relative to the node's orientation. Alternatively we could
    // multiply the desired direction with the node's orientation quaternion, and use just Translate()
    if (input->GetKeyDown('W'))
        cameraNode_->TranslateRelative(Vector3::UP * MOVE_SPEED * timeStep);
    if (input->GetKeyDown('S'))
        cameraNode_->TranslateRelative(Vector3::DOWN* MOVE_SPEED * timeStep);
    if (input->GetKeyDown('A'))
        cameraNode_->TranslateRelative(Vector3::LEFT * MOVE_SPEED * timeStep);
    if (input->GetKeyDown('D'))
        cameraNode_->TranslateRelative(Vector3::RIGHT * MOVE_SPEED * timeStep);
    if (input->GetKeyDown(KEY_PAGEUP))
    {
        Camera* camera = cameraNode_->GetComponent<Camera>();
        camera->SetZoom(camera->GetZoom() * 1.01f);
    }
    if (input->GetKeyDown(KEY_PAGEDOWN))
    {
        Camera* camera = cameraNode_->GetComponent<Camera>();
        camera->SetZoom(camera->GetZoom() * 0.99f);
    }
}


void Urho2DParticle::SubscribeToEvents()
{
    // Subscribe HandleUpdate() function for processing update events
    SubscribeToEvent(E_UPDATE, HANDLER(Urho2DParticle, HandleUpdate));
}

void Urho2DParticle::HandleUpdate(StringHash eventType, VariantMap& eventData)
{
    using namespace Update;

    // Take the frame time step, which is stored as a float
    float timeStep = eventData[P_TIMESTEP].GetFloat();

    // Move the camera, scale movement with time step
    MoveCamera(timeStep);

    Graphics* graphics = GetSubsystem<Graphics>();
    float halfWidth = (float)graphics->GetWidth() * 0.5f;
    float halfHeight = (float)graphics->GetHeight() * 0.5f;

    for (unsigned i = 0; i < particleNodes_.Size(); ++i)
    {
        SharedPtr<Node> node = particleNodes_[i];

        Vector2 position = NodeUtils::GetPosition(node);

        Vector2 moveSpeed = node->GetVar(VAR_MOVESPEED).GetVector2();        
        Vector2 newPosition = position + moveSpeed * timeStep;
        if (newPosition.x_ < -halfWidth || newPosition.x_ > halfWidth)
        {
            newPosition.x_ = position.x_;
            moveSpeed.x_ = -moveSpeed.x_;
            node->SetVar(VAR_MOVESPEED, moveSpeed);
        }
        if (newPosition.y_ < -halfHeight || newPosition.y_ > halfHeight)
        {
            newPosition.y_ = position.y_;
            moveSpeed.y_ = -moveSpeed.y_;
            node->SetVar(VAR_MOVESPEED, moveSpeed);
        }

        NodeUtils::SetPosition(node, newPosition);
    }
}
