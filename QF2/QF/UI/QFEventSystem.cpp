#include "../QF.h"

/* Mouse motion event */
	QF::UI::EventSystem::MouseMotionEvent::MouseMotionEvent(QF::UI::Panel* _Panel)
		: m_Panel{_Panel} , m_Position{ImGui::GetMousePos()}
	{}

	const QF::Utils::Vec2 QF::UI::EventSystem::MouseMotionEvent::g_GlobalPosition() const 
	{ return m_Position; } 

	const QF::Utils::Vec2 QF::UI::EventSystem::MouseMotionEvent::g_Position() const 
	{ /* Formula: m_position -  panel_offseted_position = fixed position to given panel */
		return (m_Position - m_Panel->g_PositionOffset());
	}

	QF::UI::Panel* QF::UI::EventSystem::MouseMotionEvent::g_Panel() const 
	{ return m_Panel; }
/* Mouse Clicked event */
	QF::UI::EventSystem::MouseClickedEvent::MouseClickedEvent(QF::UI::Panel* _Panel)
		: MouseMotionEvent(_Panel) {};
/* Mouse Panel Drag Event */
	QF::UI::EventSystem::MousePanelDragEvent::MousePanelDragEvent(QF::UI::Panel* _Panel, const QF::Utils::Vec2& _WhereClicked)
		: m_ClickPosition{ _WhereClicked }, MouseMotionEvent(_Panel) {}

	const QF::Utils::Vec2 QF::UI::EventSystem::MousePanelDragEvent::g_ClickPosition() const 
	{ return (m_ClickPosition - g_Panel()->g_PositionOffset()); }

	const QF::Utils::Vec2 QF::UI::EventSystem::MousePanelDragEvent::g_ClickPositionGlobal() const 
	{ return m_ClickPosition; }