#include "../../QF.h"

/*========================= Constructor & Destructor  =========================*/
	QF::UI::Window::TitleBar::TitleBar(Window* _Window)
		: m_Window{_Window}, Panel(_Window, {10.0f, 10.0f}, {10.0f, 10.0f})
	{
		/* Set default configuration */
		func_InitializeDefaultHints();
		
		/* Register hooks to panel handler */
		func_InitializeHooks();
	}

	QF::UI::Window::TitleBar::~TitleBar()
	{} 
/*========================= Layout handling -> Sizer =========================*/
	void QF::UI::Window::TitleBar::func_SetLayout() 
	{
		this->s_Position({ 0, 0 }); /* Top left of the frame */
		this->s_Size(QF::Utils::Vec2(
				m_Window->g_Size().x,
				/* Centered */
				QF::Utils::Math::g_TextSize(
					m_Window->g_Name(),
					m_Hints.m_TitleFont
				).y + 6.0f
			)
		); /* x -> full, y -> custom */
	}
/*========================= Hooks =========================*/
	void QF::UI::Window::TitleBar::func_InitializeDefaultHints()
	{
		Hints _DefaultHints;
		/* Default configuration */
		_DefaultHints.m_BackgroundColor = ImColor(61, 64, 71, 255);
		_DefaultHints.m_TitleFont = Helpers::g_FontFromTTF("C:\\QF\\menlo.ttf", 16.0f);
		_DefaultHints.m_TitleColor = ImColor(255, 255, 255, 255);

		/* Apply */
		m_Hints = _DefaultHints;
	}

/*========================= Hooks =========================*/
	void QF::UI::Window::TitleBar::func_InitializeHooks()
	{
		QF::UI::EventSystem::EventHandler* _Handler = g_EventHandler();

		_Handler->Subscribe<QF::UI::EventSystem::RenderEvent>(this, &QF::UI::Window::TitleBar::hk_Render);
		_Handler->Subscribe<QF::UI::EventSystem::MousePanelDragEvent>(this, &TitleBar::hk_MousePanelDrag);
	}

	void QF::UI::Window::TitleBar::hk_Render(QF::UI::EventSystem::RenderEvent& _Event)
	{
		/* Update layout in case of resize */
		func_SetLayout();
		/* Initialize canvas */
		QF::UI::SimpleDC _Canvas{this};
		/* Render background */
		_Canvas.draw_RectFilled({ 0, 0 }, g_Size(), m_Hints.m_BackgroundColor);
		/* Render titlename */
		const std::string _TitleText = m_Window->g_Name();

		const QF::Utils::Vec2 _TitlenamePos = QF::Utils::Math::g_TextCenteredPosition(
			g_Position(), g_Size(), _TitleText, m_Hints.m_TitleFont
		);

		_Canvas.draw_Text(_TitlenamePos, m_Hints.m_TitleColor, _TitleText);
	}

	/* Drag the window */
	void QF::UI::Window::TitleBar::hk_MousePanelDrag(QF::UI::EventSystem::MousePanelDragEvent& _Event)
	{
		/* Get fixed position that will be applied to m_Window */
		const QF::Utils::Vec2 _FixedPosition = (
			m_Window->g_AbsoluteMousePosition() - _Event.g_ClickPosition()
			);

		/* Check if mouse position is ingering with Windows taskbar pos */
		if (QF::Utils::Math::is_InBounds(
			m_Window->g_AbsoluteMousePosition(), QF::Utils::System::g_TaskbarPosition(), QF::Utils::System::g_TaskbarSize()
		) == true) return; 

		/* Apply */
		m_Window->s_Position(_FixedPosition);
	}
