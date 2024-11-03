#include "../QF.h"

/*========================= Constructor & Destructor =========================*/
	QF::UI::Panel::Panel(Element* _Parent,
		const QF::Utils::Vec2& _Position, const QF::Utils::Vec2& _Size)
		: m_Parent{_Parent}, m_Size{_Size}, m_Position{_Position}, Element(), m_EventHandler{new EventSystem::EventHandler()}
	{
		/* Declare that im panel */
		im_Panel(this, m_Parent);
		/* Declare that im children of absolute parent */
		g_AbsoluteParent()->im_Children(this);
		/* If my parent is panel declare that im children */
		if (m_Parent->g_Panel() != nullptr)
		{
			m_Parent->g_Panel()->im_Children(this);
		}
		/* Assign id */
		m_ID = g_AbsoluteParent()->g_GeneratedID();
		/* Assign position & size offsets & visibility */
		func_UpdateAllDependent();

		/* Finalization complete */
		QF::Utils::Debug().Insert(QF::Utils::Debug::LogHint::IMPORTANT, __FUNCTION__,
			"Panel initialized successfully");
	}

	void QF::UI::Panel::Destroy()
	{
		func_EraseChildren();
		//this->~Panel();
		delete this;
	}

	void QF::UI::Panel::DestroyWindowCall()
	{
		delete	m_EventHandler;
	}

	QF::UI::Panel::~Panel()
	{
		/* Delete event handler */
		delete m_EventHandler;	

		/* No longer a window children & a panel children if it was */
		if (g_AbsoluteParent()) {
			g_AbsoluteParent()->im_NoLongerAChildren(this);
		}
		if (m_Parent && m_Parent->g_Panel()) {
			m_Parent->g_Panel()->im_NoLongerChildren(this);
		}

		/* Log success for panel destruction */
		QF::Utils::Debug().Insert(QF::Utils::Debug::LogHint::IMPORTANT, __FUNCTION__,
			std::format("Successfully destroyed panel, linked to window: {}", 
				g_AbsoluteParent()->g_Name()
			)); 
	}
/*========================= Position & size offset handling =========================*/
	void QF::UI::Panel::func_UpdatePositionOffset()
	{
		Panel* _ParentingPanel = m_Parent->g_Panel();
		/* Parent is window */
		if (_ParentingPanel == nullptr)
		{
			m_PositionOffset = m_Position; return;
		}
		m_PositionOffset = (_ParentingPanel->g_PositionOffset() + m_Position);
	}

	void QF::UI::Panel::func_UpdateSizeOffset()
	{
		Panel* _ParentingPanel = m_Parent->g_Panel();
		/* Parent is window */
		if (_ParentingPanel == nullptr)
		{
			m_SizeOffset = m_Size; 
			//std::cout << std::format("{}:{} | {}:{}\n", (int)m_Size.x, (int)m_Size.y, (int)m_SizeOffset.x, (int)m_SizeOffset.y);
			return; 
		}
		/* Override */
		m_SizeOffset = (_ParentingPanel->g_SizeOffset());
		/* Limit axis */
		m_SizeOffset.x = std::min(m_Size.x, std::max(0.0f, (m_SizeOffset.x - m_Position.x)));
		m_SizeOffset.y = std::min(m_Size.y, std::max(0.0f, (m_SizeOffset.y - m_Position.y)));
	}

	void QF::UI::Panel::func_UpdatePositionOffsetForChildren()
	{
		for (Panel* _Child : m_Children)
		{ /* Force update*/
			_Child->func_UpdatePositionOffset();
		}
	}

	void QF::UI::Panel::func_UpdateSizeOffsetForChildren()
	{
		for (Panel* _Child : m_Children)
		{ 
			_Child->func_UpdateSizeOffset();
		}
	}

	void QF::UI::Panel::func_UpdateVisibility()
	{
		Panel* _ParentPanel = m_Parent->g_Panel();

		/* Apply true by default */
		if (_ParentPanel == nullptr) { m_Visible = true;  return; }
		/* Apply false if there isn't anything to show -> avoid unnessesary calls */
		if (m_SizeOffset == QF::Utils::Vec2{0.0f, 0.0f}) { m_Visible = false; return; }
		/* Apply same as parent */
		m_Visible = _ParentPanel->m_Visible;
	}

	void QF::UI::Panel::func_UpdateVisibilityForChildren()
	{
		for (Panel* _Child : m_Children) { _Child->func_UpdateVisibility(); }
	}

	void QF::UI::Panel::func_UpdateAllDependent()
	{
		func_UpdatePositionOffset();
		func_UpdateSizeOffset();
		func_UpdateVisibility();
	}


/*========================= Children handling =========================*/
	void QF::UI::Panel::im_NoLongerChildren(Panel* _Panel)
	{
		size_t _Iterator = 0;
		for (Panel* _Child : m_Children)
		{
			if (_Child && _Panel && _Child->g_Id() == _Panel->g_Id())
			{
				m_Children.erase((m_Children.begin() + _Iterator));

				QF::Utils::Debug().Insert(QF::Utils::Debug::LogHint::IMPORTANT, __FUNCTION__,
					std::format("Erased panel from children stack, id: {}, selfid: {}", _Panel->g_Id(), this->g_Id())
				);

				return;
			}
			_Iterator++;
		}
	}

	const size_t QF::UI::Panel::g_Id() const 
	{
		return m_ID;
	}

	void QF::UI::Panel::im_Children(Panel* _Child)
	{
		m_Children.push_back(_Child);
	}

	void QF::UI::Panel::func_EraseChildren()
	{
		/* Log debug info about operation */
		QF::Utils::Debug().Insert(QF::Utils::Debug::LogHint::MESSAGE, __FUNCTION__,
			std::format(
				"Attempting to destroy all linked panels (m_Children) for panel linked to window: {}",
				g_AbsoluteParent()->g_Name()
			)
		);

		/* Destroy */
		for (Panel* _Child : m_Children)
		{
			_Child->Destroy();
		}
	}

	void QF::UI::Panel::func_EraseNullChildren()
	{
		m_Children.erase(
			std::remove_if(m_Children.begin(), m_Children.end(), [](Panel* ptr)
				{ return ptr == nullptr; }
		), m_Children.end());
	}
/*========================= Event propagation =========================*/
	QF::UI::EventSystem::EventHandler* QF::UI::Panel::g_EventHandler()
	{
		return m_EventHandler;
	}
/*========================= Getting offsets  =========================*/
	const QF::Utils::Vec2 QF::UI::Panel::g_Position() const 
	{ return m_Position; }

	const QF::Utils::Vec2 QF::UI::Panel::g_PositionOffset() const 
	{ return m_PositionOffset; }

	const QF::Utils::Vec2 QF::UI::Panel::g_Size() const 
	{ return m_Size; }

	const QF::Utils::Vec2 QF::UI::Panel::g_FinalPosition() const 
	{ return (m_Position + m_Size); }

	const QF::Utils::Vec2 QF::UI::Panel::g_FinalPositionOffset() const 
	{ 
		return (m_PositionOffset + m_SizeOffset);
	}

	const QF::Utils::Vec2 QF::UI::Panel::g_SizeOffset() const 
	{ return m_SizeOffset; }

	const bool QF::UI::Panel::g_Visibility() const { return m_Visible; }

	const bool QF::UI::Panel::is_InBounds(const QF::Utils::Vec2& _Vec) const { return QF::Utils::Math::is_InBounds(
		_Vec, m_PositionOffset, m_SizeOffset
	); }
/*========================= Setting offsets  =========================*/
	void QF::UI::Panel::s_Position(const QF::Utils::Vec2 _New)
	{ 
		/* Set position */
		m_Position = _New;
		/* Update position offset */
		func_UpdateAllDependent();
		/* Update position offset for children */
		func_UpdatePositionOffsetForChildren();
		func_UpdateVisibilityForChildren();
	}

	void QF::UI::Panel::s_Size(const QF::Utils::Vec2 _New)
	{
		m_Size = _New; 
		func_UpdateAllDependent();
		func_UpdateSizeOffsetForChildren();
		func_UpdateVisibilityForChildren();
		//std::cout << std::format("x: {}, y: {}\n", m_SizeOffset.x, m_SizeOffset.y);
	}

	void QF::UI::Panel::s_Visible(const bool& _State)
	{
		m_Visible = _State; 
		func_UpdateVisibilityForChildren();
	}
