#include "../QF.h"

/*========================= Constructor & Destructor =========================*/
	QF::UI::Panel::Panel(Element* _Parent,
		const QF::Utils::Vec2& _Position, const QF::Utils::Vec2& _Size)
		: m_Parent{_Parent}, m_Size{_Size}, m_Position{_Position}, Element()
	{
		/* Declare that im panel */
		im_Panel(this, m_Parent);
		/* Declare that im children of absolute parent */
		g_AbsoluteParent()->im_Children(this);
		/* If my parent is panel declare that im children */
		if (g_Panel() != nullptr)
		{
			g_Panel()->im_Children(this);
		}
		/* Finalization complete */
		QF::Utils::Debug().Insert(QF::Utils::Debug::LogHint::MESSAGE, __FUNCTION__,
			"Panel initialized successfully: ");
	}
/*========================= Children handling =========================*/
	void QF::UI::Panel::im_Children(Panel* _Child)
	{
		m_Children.push_back(_Child);
	}