#include "QF/QF.h"


class Calculator : public QF::UI::Panel
{
public:
 Calculator(QF::UI::Window* _Window)
    : QF::UI::Panel(_Window, {0, 0}, _Window->g_Size())
 {
    g_EventHandler()->Subscribe<QF::UI::EventSystem::RenderEvent>(this,&Calculator::hk_Render);
   
 }

private:
 void hk_Render(QF::UI::EventSystem::RenderEvent& _Event) 
 {
   std::cout << g_PositionOffset().g_String() << "\n";
 } 
};

class Application : public QF::UI::App
{
public:
	Application() : QF::UI::App() { Implement(); };

	bool on_Init() override
	{
		new Calculator(new QF::UI::Window(this, "QF testing: Calculator"));
	
		return true; 
	}

};

/**
 * @brief Entry point of the application.
 *
 * This function initializes debug print hints, sets global Vec2 hints,
 * initializes global hints, creates an Application instance, and runs the program.
 *
 * @return Integer value indicating the exit status of the program.
 *         Returns 0 on successful execution.
 */
int main()
{
    QF::Utils::Debug::s_MultipleGlobalDebugPrintHints(
        {
            QF::Utils::Debug::PrintHint::PRINT_CRITICAL_ERROR,
            QF::Utils::Debug::PrintHint::PRINT_WARNING,
            QF::Utils::Debug::PrintHint::PRINT_MESSAGE,
            QF::Utils::Debug::PrintHint::PRINT_WITH_COLORS,
            QF::Utils::Debug::PrintHint::PRINT_IMPORTANT,
            QF::Utils::Debug::PrintHint::PRINT_EXCEPTION
        }, true
    );

    QF::Utils::Vec2::s_GlobalVec2Hint(QF::Utils::Vec2::Hint::DIVIDE_BY_ZERO, 
        QF::Utils::Vec2::HintValue::DIVIDE_BY_ZERO_RETURN_SELF);
    QF::Utils::Debug::global_InitializeHints();
    //printf(QF::Utils::Filesystem::Open("C:\\QF\\textd.vs").g_Buffer());

    new Application();

    return 0;
}

