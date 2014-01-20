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
#include "Node.h"
#include "NodeUtils.h"

#include "DebugNew.h"

namespace Urho3D
{
void NodeUtils::SetPosition(Node* node, const Vector2& position)
{
    if (node)
        node->SetPosition(Vector3(position.x_, position.y_, 0.0f));
}

void NodeUtils::SetRotation(Node* node, float angle)
{
    if (node)
        node->SetRotation(Quaternion(angle, Vector3::FORWARD));
}

void NodeUtils::SetScale(Node* node, const Vector2& scale)
{
    if (node)
        node->SetScale(Vector3(scale.x_, scale.y_, 1.0f));
}

void NodeUtils::SetTransform(Node* node, const Vector2& position, float angle)
{
    if (node)
        node->SetTransform(Vector3(position.x_, position.y_, 0.0f), Quaternion(angle, Vector3::FORWARD));
}

void NodeUtils::SetTransform(Node* node, const Vector2& position, float angle, float scale)
{
    if (node)
        node->SetTransform(Vector3(position.x_, position.y_, 0.0f), Quaternion(angle, Vector3::FORWARD), scale);
}

void NodeUtils::SetTransform(Node* node, const Vector2& position, float angle, const Vector2& scale)
{
    if (node)
        node->SetTransform(Vector3(position.x_, position.y_, 0.0f), Quaternion(angle, Vector3::FORWARD), Vector3(scale.x_, scale.y_, 1.0f));
}

void NodeUtils::SetWorldPosition(Node* node, const Vector2& position)
{
    if (node)
        node->SetWorldPosition(Vector3(position.x_, position.y_, 0.0f));
}

void NodeUtils::SetWorldRotation(Node* node, float angle)
{
    if (node)
        node->SetWorldRotation(Quaternion(angle, Vector3::FORWARD));
}

void NodeUtils::SetWorldScale(Node* node, const Vector2& scale)
{
    if (node)
        node->SetWorldScale(Vector3(scale.x_, scale.y_, 1.0f));
}

void NodeUtils::SetWorldTransform(Node* node, const Vector2& position, float angle)
{
    if (node)
        node->SetWorldTransform(Vector3(position.x_, position.y_, 0.0f), Quaternion(angle, Vector3::FORWARD));
}

void NodeUtils::SetWorldTransform(Node* node, const Vector2& position, float angle, float scale)
{
    if (node)
        node->SetWorldTransform(Vector3(position.x_, position.y_, 0.0f), Quaternion(angle, Vector3::FORWARD), scale);
}

void NodeUtils::SetWorldTransform(Node* node, const Vector2& position, float angle, const Vector2& scale)
{
    if (node)
        node->SetWorldTransform(Vector3(position.x_, position.y_, 0.0f), Quaternion(angle, Vector3::FORWARD), Vector3(scale.x_, scale.y_, 1.0f));
}

void NodeUtils::Translate(Node* node, const Vector2& delta)
{
    if (node)
        node->Translate(Vector3(delta.x_, delta.y_, 0.0f));
}

void NodeUtils::TranslateRelative(Node* node, const Vector2& delta)
{
    if (node)
        node->TranslateRelative(Vector3(delta.x_, delta.y_, 0.0f));
}
void NodeUtils::Rotate(Node* node, float delta)
{
    if (node)
        node->Rotate(Quaternion(delta, Vector3::FORWARD));
}

void NodeUtils::Scale(Node* node, const Vector2& scale)
{
    if (node)
        node->Scale(Vector3(scale.x_, scale.y_, 1.0f));
}

Vector2 NodeUtils::GetPosition(Node* node)
{
    if (!node)
        return Vector2::ZERO;

    const Vector3& position = node->GetPosition();
    return Vector2(position.x_, position.y_);
}

float NodeUtils::GetRotation(Node* node)
{
    if (!node)
        return 0.0f;

    return node->GetRotation().RollAngle();
}

Vector2 NodeUtils::GetScale(Node* node)
{
    if (!node)
        return Vector2::ONE;

    const Vector3& scale = node->GetScale();
    return Vector2(scale.x_, scale.y_);
}

Vector2 NodeUtils::GetWorldPosition(Node* node)
{
    if (!node)
        return Vector2::ZERO;

    const Vector3& position = node->GetWorldPosition();
    return Vector2(position.x_, position.y_);
}

float NodeUtils::GetWorldRotation(Node* node)
{
    if (!node)
        return 0.0f;

    return node->GetWorldRotation().RollAngle();
}

Vector2 NodeUtils::GetWorldScale(Node* node)
{
    if (!node)
        return Vector2::ONE;

    const Vector3& scale = node->GetWorldScale();
    return Vector2(scale.x_, scale.y_);
}

Vector2 NodeUtils::LocalToWorld(Node* node, const Vector2& position)
{
    if (!node)
        return position;

    Vector3 result = node->LocalToWorld(Vector3(position.x_, position.y_, 0.0f));
    return Vector2(result.x_, result.y_);
}

Vector2 NodeUtils::WorldToLocal(Node* node, const Vector2& position)
{
    if (!node)
        return position;

    Vector3 result = node->WorldToLocal(Vector3(position.x_, position.y_, 0.0f));
    return Vector2(result.x_, result.y_);
}
}
