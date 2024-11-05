#include "../../QF.h"

/* Component : Button */
	QF::UI::Components::Button::Button(Hints _Hints)
		: m_Hints{ _Hints }, Panel(_Hints.m_Parent, _Hints.m_Position, _Hints.m_Size)
	{
		g_EventHandler()->Subscribe<EventSystem::RenderEvent>(this, &Button::hk_Render);
		g_EventHandler()->Subscribe<EventSystem::MouseClickedEvent>(this, &Button::hk_OnMouseClick);
	}

	void QF::UI::Components::Button::hk_Render(EventSystem::RenderEvent& _Event)
	{
		/* Get background color */

		bool _MouseOnPanel = is_InBounds(g_AbsoluteParent()->g_MousePosition());

		ImU32 _BGColor = (_MouseOnPanel ? m_Hints.m_ColorActivated : m_Hints.m_ColorDefault);

		_BGColor  = 
			QF::Utils::Transform::g_ImColorFromFloatVector(
				m_BackgroundAnimations->func_SmartLerp(
					QF::Utils::Transform::g_VectorFloatFromImColor(_BGColor), 
					100
				));

		/* Setup Canvas */
		QF::UI::SimpleDC _Canvas{this};

		/* Draw background */
		_Canvas.draw_RectFilled({0,0},g_Size(), _BGColor);
	}

	void QF::UI::Components::Button::hk_OnMouseClick(EventSystem::MouseClickedEvent& _Event)
	{
		/* Propagate event further */
		EventSystem::MouseButtonClickEvent _ToPropagate{this};
		
		g_EventHandler()->Dispatch( _ToPropagate );
	}