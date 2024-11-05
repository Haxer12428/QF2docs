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

class MyPanel : public QF::UI::Panel
{
public:
	MyPanel(QF::UI::Element* _Parent) : QF::UI::Panel(_Parent, { 50, 50 }, { 20, 20 })
	{
		g_EventHandler()->Subscribe<QF::UI::EventSystem::RenderEvent>(this, &MyPanel::renderDispatched);
		//m_Image = new QF::Utils::Image("C:\\QF\\iconclose.png");

	}

	void renderDispatched(QF::UI::EventSystem::RenderEvent& _Evt)
	{
		

		if (std::chrono::duration_cast<std::chrono::seconds>(
			std::chrono::high_resolution_clock::now() - _Now
		 ).count() > 1)
		{
			_BuildPanels++;
			_Now = std::chrono::high_resolution_clock::now();

			QF::UI::Components::Button::Hints _Hints;
			_Hints.m_ColorActivated = ImColor(255, 255, 255);
			_Hints.m_ColorDefault = ImColor(0, 128, 255);
			_Hints.m_Position = {20 + 21*_BuildPanels, 50};
			_Hints.m_Size = {20, 20};
			_Hints.m_Parent = g_AbsoluteParent();

			//QF::UI::Panel* m_next = new QF::UI::Panel(_Hints.m_Parent, {10, 10}, {20, 20});
		}
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
		
	//	QF::UI::Window* _Window5 = new QF::UI::Window(this, "QF5");

	//	MyPanel* panel2 = new MyPanel(_Window4);
		//QF::UI::Panel* _panel1 = new QF::UI::Panel(_Window4, { 50, 50 }, { 400, 200 });

		//QF::UI::Panel* _panel2 = new QF::UI::Panel(panel, { 40, 40 }, { 40, 40 });

		//_panel1->Destroy();
	
		

		return true; 
	}

};

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

	long _Chars = 0; 
	long _Lines = 0; 
	long _Headers = 0; 
	long _Cpp = 0; 

	for (const std::filesystem::directory_entry& _Entry : std::filesystem::recursive_directory_iterator(
		"C:\\Users\\Survi\\source\\repos\\QF2\\QF2\\QF"
	)) { 
		if (_Entry.is_regular_file())
		{
			const QF::Utils::Filesystem::Open::Info _FileInfo = QF::Utils::Filesystem::Open(
				_Entry.path()
			).g_Info();

			_Chars += _FileInfo.g_Chars();
			_Lines += _FileInfo.g_Lines();

			if (_Entry.path().extension() == ".cpp") _Cpp++;
			if (_Entry.path().extension() == ".h") _Headers++;
		}
	}

	const std::string _ProjectInformation = std::format(
		"Project information: \n .h -> {}\n .cpp -> {}\n lines -> {}\n chars -> {}\n",
		_Headers, _Cpp, _Lines, _Chars
	);

	QF::Utils::Debug().Insert(QF::Utils::Debug::LogHint::IMPORTANT, __FUNCTION__,
		_ProjectInformation);

	new Application();



	return 0;
}