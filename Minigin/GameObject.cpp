#include "GameObject.h"

dae::GameObject::~GameObject()
{
	// Remove this object from its parent's children list
	if (m_parent)
		m_parent->RemoveChild(this);

	// Orphan all children to prevent dangling parent pointers
	for (auto* child : m_children)
		child->m_parent = nullptr;
}

void dae::GameObject::Update(float deltaTime)
{
	// Update all components attached to this GameObject
	for (auto& component : m_components)
		component->Update(deltaTime);
}

void dae::GameObject::Render() const
{
	// Render all components attached to this GameObject
	for (const auto& component : m_components)
		component->Render();
}

void dae::GameObject::SetLocalPostion(const glm::vec3& pos)
{
	// Set position relative to parent 
	m_localPosition = pos;
	SetPositionDirty(); // set dirty
}

const glm::vec3& dae::GameObject::GetWorldPosition()
{
	// only recalculate world position if it's marked dirty
	if(m_positionIsDirty)
		UpdateWorldPosition();
	return m_worldPosition;
}

void dae::GameObject::SetParent(GameObject* parent, bool keepWorldPosition)
{
	// can't set parent to self, a descendant, or the same parent we already have
	if (IsChild(parent) || parent == this || m_parent == parent)
		return;

	// If currently at root level, convert world position to local
	if (m_parent == nullptr)
	{
		SetLocalPostion(GetWorldPosition());
	}
	else
	{
		// If reparenting with keepWorldPosition enabled, recalculate local position
		// This ensures the object stays visually in the same place after reparenting
		if(keepWorldPosition)
			SetLocalPostion(GetWorldPosition() - parent->GetWorldPosition());
		SetPositionDirty();
	}

	// Update parent references
	if (m_parent) m_parent->RemoveChild(this);
	m_parent = parent;
	if (m_parent) m_parent->AddChild(this);
}

void dae::GameObject::UpdateWorldPosition()
{
	// Recalculate world position
	if (m_positionIsDirty)
	{
		if (m_parent == nullptr)
			m_worldPosition = m_localPosition; 
		else
			m_worldPosition = m_parent->GetWorldPosition() + m_localPosition; 
	}
	m_positionIsDirty = false; 
}

void dae::GameObject::SetPositionDirty()
{
	// Mark this object and all descendants for position recalculation
	// goes down the hierarchy to ensure entire tree is updated
	m_positionIsDirty = true;
	for (auto* child : m_children)
		child->SetPositionDirty();
}

bool dae::GameObject::IsChild(const GameObject* go) const
{
	//  check if the given object is a descendant of this GameObject
	// Prevents circular parent relationships
	for (const auto* child : m_children)
	{
		if (child == go || child->IsChild(go))
			return true;
	}
	return false;
}

void dae::GameObject::AddChild(GameObject* child)
{
	// Safely add a child, preventing nullptr and duplicates
	if(child == nullptr)
		return;
	if (std::find(m_children.begin(), m_children.end(), child) == m_children.end())
		m_children.push_back(child);
}

void dae::GameObject::RemoveChild(GameObject* child)
{
	// Remove child from the children list using erase-remove idiom
	// Safely handles element removal during iteration
	m_children.erase(
		std::remove(m_children.begin(), m_children.end(), child),
		m_children.end());
}

