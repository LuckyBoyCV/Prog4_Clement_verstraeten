#include "GameObject.h"

dae::GameObject::~GameObject()
{
	if (m_parent)
		m_parent->RemoveChild(this);

	// kids lose their parent reference so they dont point to freed memory
	for (auto* child : m_children)
		child->m_parent = nullptr;
}

void dae::GameObject::Update(float deltaTime)
{
	for (auto& component : m_components)
		component->Update(deltaTime);
}

void dae::GameObject::Render() const
{
	for (const auto& component : m_components)
		component->Render();
}

void dae::GameObject::SetLocalPostion(const glm::vec3& pos)
{
	m_localPosition = pos;
	SetPositionDirty();
}

const glm::vec3& dae::GameObject::GetWorldPosition()
{
	if (m_positionIsDirty)
		UpdateWorldPosition();
	return m_worldPosition;
}

void dae::GameObject::SetParent(GameObject* parent, bool keepWorldPosition)
{
	// cant parent to self, a child , or the same parent we already have
	if (IsChild(parent) || parent == this || m_parent == parent)
		return;

	if (m_parent == nullptr)
	{
		SetLocalPostion(GetWorldPosition());
	}
	else
	{
		if (keepWorldPosition)
			SetLocalPostion(GetWorldPosition() - parent->GetWorldPosition());
		SetPositionDirty();
	}

	if (m_parent) m_parent->RemoveChild(this);
	m_parent = parent;
	if (m_parent) m_parent->AddChild(this);
}

void dae::GameObject::UpdateWorldPosition()
{
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
	// mark dirty all the way down the tree
	m_positionIsDirty = true;
	for (auto* child : m_children)
		child->SetPositionDirty();
}

bool dae::GameObject::IsChild(const GameObject* go) const
{
	for (const auto* child : m_children)
	{
		if (child == go || child->IsChild(go))
			return true;
	}
	return false;
}

void dae::GameObject::AddChild(GameObject* child)
{
	if (child == nullptr)
		return;
	// don't add duplicates
	if (std::find(m_children.begin(), m_children.end(), child) == m_children.end())
		m_children.push_back(child);
}

void dae::GameObject::RemoveChild(GameObject* child)
{
	m_children.erase(
		std::remove(m_children.begin(), m_children.end(), child),
		m_children.end());
}