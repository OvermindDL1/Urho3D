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

#pragma once


namespace Urho3D
{
class Node;

/// Node Utils.
class URHO3D_API NodeUtils
{
public:
    /// Set position in parent space.
    static void SetPosition(Node* node, const Vector2& position);
    /// Set rotation in parent space.
    static void SetRotation(Node* node, float angle);
    /// Set scale in parent space.
    static void SetScale(Node* node, const Vector2& scale);
    /// Set both position and rotation in parent space as an atomic operation.
    static void SetTransform(Node* node, const Vector2& position, float angle);
    /// Set both position, rotation and uniform scale in parent space as an atomic operation.
    static void SetTransform(Node* node, const Vector2& position, float angle, float scale);
    /// Set both position, rotation and scale in parent space as an atomic operation.
    static void SetTransform(Node* node, const Vector2& position, float angle, const Vector2& scale);
    /// Set position in world space.
    static void SetWorldPosition(Node* node, const Vector2& position);
    /// Set rotation in world space.
    static void SetWorldRotation(Node* node, float angle);
    /// Set scale in world space.
    static void SetWorldScale(Node* node, const Vector2& scale);
    /// Set both position and rotation in world space as an atomic operation.
    static void SetWorldTransform(Node* node, const Vector2& position, float angle);
    /// Set both position, rotation and uniform scale in world space as an atomic operation.
    static void SetWorldTransform(Node* node, const Vector2& position, float angle, float scale);
    /// Set both position, rotation and scale in world space as an atomic opration.
    static void SetWorldTransform(Node* node, const Vector2& position, float angle, const Vector2& scale);
    /// Move the scene node in parent space, which is the same as world space if the scene node is on the root level.
    static void Translate(Node* node, const Vector2& delta);
    /// Move the scene node in parent space relative to its current rotation.
    static void TranslateRelative(Node* node, const Vector2& delta);
    /// Rotate the scene node in parent space either relative to its current rotation axes, or a fixed axis.
    static void Rotate(Node* node, float delta);
    /// Modify scale in parent space.
    static void Scale(Node* node, const Vector2& scale);
    
    /// Return position in parent space.
    static Vector2 GetPosition(Node* node);
    /// Return rotation in parent space.
    static float GetRotation(Node* node);
    /// Return scale in parent space.
    static Vector2 GetScale(Node* node);
    /// Return position in world space.
    static Vector2 GetWorldPosition(Node* node);
    /// Return rotation in world space.
    static float GetWorldRotation(Node* node);
    /// Return scale in world space.
    static Vector2 GetWorldScale(Node* node);

    /// Convert a local space position to world space.
    Vector2 LocalToWorld(Node* node, const Vector2& position);
    /// Convert a world space position to local space.
    Vector2 WorldToLocal(Node* node, const Vector2& position);
};

}
