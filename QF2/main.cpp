#include "QF/QF.h"
#include <thread>


std::string replaceLineEndings(const std::string& input, const std::string& replacement) {
	std::string output;
	output.reserve(input.size()); // Reserve memory for efficiency

	for (char c : input) {
		if (c == '\r' || c == '\n') {
			if (c == '\r')
				output += "[r]"; // Append the replacement for line endings
			if (c == '\n')
				output += "[n]";
		}
		else {
			output += c; // Append the character itself
		}
	}

	return output;
}

class Application : public QF::UI::App
{
public:
	Application() : QF::UI::App() { Implement(); };

	bool on_Init() override
	{
		new QF::UI::Window(this, "QF2");
		new QF::UI::Window(this, "QF3");
		QF::UI::Window* _Window4 = new QF::UI::Window(this, "QF4");

		new QF::UI::Panel(_Window4, { 20, 20 }, { 200, 200 });

		

		return true; 
	}

};

int main()
{

	QF::Utils::Debug::s_MultipleGlobalDebugPrintHints(
		{
			QF::Utils::Debug::PrintHint::PRINT_ERROR,
			QF::Utils::Debug::PrintHint::PRINT_WARNING,
			QF::Utils::Debug::PrintHint::PRINT_MESSAGE,
			QF::Utils::Debug::PrintHint::PRINT_WITH_COLORS,
			QF::Utils::Debug::PrintHint::PRINT_TO_FILE,
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