#include "QF/QF.h"
#include <thread>


/**
 * Replaces line endings in the input string with the specified replacement string.
 *
 * @param input The input string where line endings are to be replaced.
 * @param replacement The string to replace line endings with.
 * @return A new string with line endings replaced by the specified replacement.
 */
std::string ReplaceLineEndings(const std::string& input, const std::string& replacement) {
    std::string output;
    // Preallocate memory for output to improve performance
    output.reserve(input.size() + input.size() / 2 * (replacement.size() - 1));

    auto outputIterator = std::back_inserter(output);
    auto inputIterator = input.begin();
    auto inputEnd = input.end();

    while (inputIterator != inputEnd) {
        if (*inputIterator == '\r') {
            // Replace '\r' or '\r\n' with the replacement string
            std::copy(replacement.begin(), replacement.end(), outputIterator);
            ++inputIterator;
            if (inputIterator != inputEnd && *inputIterator == '\n') {
                ++inputIterator; // Skip '\n' if it's part of '\r\n'
            }
        } else {
            *outputIterator++ = *inputIterator; // Copy non-line-ending characters
        }
        ++inputIterator;
    }

    return output;
}

class MyPanel : public QF::UI::Panel
{
public:
	MyPanel(QF::UI::Element* _Parent) : QF::UI::Panel(_Parent, { 50, 50 }, { 20, 20 })
	{
		g_EventHandler()->Subscribe<QF::UI::EventSystem::RenderEvent>(this, &MyPanel::renderDispatched);
		m_Image = new QF::Utils::Image("C:\\QF\\iconclose.png");

	}

	void renderDispatched(QF::UI::EventSystem::RenderEvent& _Evt)
	{
		ImDrawList* _DrawList = ImGui::GetWindowDrawList();	

		
	}

	std::vector<QF::UI::Panel*> m_Buttons;
	QF::Utils::Image* m_Image; 
	std::chrono::high_resolution_clock::time_point _Now = std::chrono::high_resolution_clock::now();
	int _BuildPanels = 0;
};

class Application : public QF::UI::App
{
public:
	Application() : QF::UI::App() { Implement(); };

	bool on_Init() override
	{
		

		QF::UI::Window* _Window4 = new QF::UI::Window(this, "QF4");
		MyPanel* panel = new MyPanel(_Window4);
	
	
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
            QF::Utils::Debug::PrintHint::PRINT_IMPORTANT
        }, true
    );

    QF::Utils::Vec2::s_GlobalVec2Hint(QF::Utils::Vec2::Hint::DIVIDE_BY_ZERO, 
        QF::Utils::Vec2::HintValue::DIVIDE_BY_ZERO_RETURN_SELF);
    QF::Utils::Debug::global_InitializeHints();
    //printf(QF::Utils::Filesystem::Open("C:\\QF\\textd.vs").g_Buffer());

    new Application();

    return 0;
}

