#include "../QF.h"

/*========================= Constructor & Destructor =========================*/
	QF::UI::Window::Window(QF::UI::App* _App, const std::string& _Name, const QF::Utils::Vec2& _Size) : 
		Element{}, m_Size{_Size}, m_Name{_Name}, m_EventHandler{ new EventSystem::EventHandler() }
	{
		/* Declare that im window */
		im_Window(this);
		/* Declare that im children of app */
		_App->im_Children(this);
		/* Declare that i want new id */
		m_ID = _App->g_GeneratedID();
		/* Create GLFW Window */
		if (!func_GLFWWindowCreate()) throw 
			std::exception(
				"glfw window creation fail"
			);;

		/* Create ImGui Context */
		if (!func_ImGuiCreateContext()) throw
			std::exception(
				"imgui context creation fail"
			);
		/* Log success */
		QF::Utils::Debug().Insert(QF::Utils::Debug::LogHint::MESSAGE, __FUNCTION__,
			std::format("Successfully created window: {}", m_Name)
			);
	}

	QF::UI::Window::~Window()
	{
		/* Destroy glfw object */
		glfwDestroyWindow(m_Window);

		/* Destroy class event handler */
		delete m_EventHandler;

		/* Log success */
		QF::Utils::Debug().Insert(QF::Utils::Debug::LogHint::IMPORTANT, __FUNCTION__, 
			std::format("Successfully deleted window: {}", m_Name)
			);
	}
/*========================= GLFW Window =========================*/
	GLFWwindow* QF::UI::Window::g_GLFWwindow() const 
	{
		return m_Window;
	}

	const size_t QF::UI::Window::g_ID() const
	{
		return m_ID;
	}

	const bool QF::UI::Window::func_GLFWWindowCreate()
	{
		if (!glfwInit())
		{ /* Glfw window creation error */
			QF::Utils::Debug().Insert(QF::Utils::Debug::LogHint::ERROR, __FUNCTION__,
				"Failed to initialize glfw.");
			return false; 
		}

		/* Create object */
		m_Window = glfwCreateWindow(
			m_Size.x, m_Size.y, m_Name.c_str(), NULL, NULL
		);
		/* Check if creation succeed */
		if (!m_Window)
		{ /* Termnate & log */
			glfwTerminate();

			QF::Utils::Debug().Insert(QF::Utils::Debug::LogHint::ERROR, __FUNCTION__,
				"Failed to create glfw window object."
				);
			return false; 
		}

		/* Make context current */
		glfwMakeContextCurrent(m_Window);
		return true; 
	}

	const bool QF::UI::Window::func_ImGuiCreateContext()
	{
		/* Check version */
		IMGUI_CHECKVERSION();

		/* Create context */
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();

		/* Return result */
		return ImGui::GetCurrentContext() != nullptr;
	}
/*========================= Main Loop =========================*/
	void QF::UI::Window::hk_MainLoop()
	{
		/* Call events */
		hk_RenderEvent();
	}
/*========================= Children handling =========================*/
	void QF::UI::Window::im_Children(Panel* _Panel)
	{
		m_Children.push_back(_Panel);
	}
/*========================= Events =========================*/
	void QF::UI::Window::hk_RenderEvent()
	{
		/* Prepare backend & frame */
		func_RenderEventPrepare();
		/* Call things */
		ImDrawList* _List = ImGui::GetWindowDrawList();
		_List->AddRect({ 20, 20 }, { 200, 200 }, ImColor(255,255,255,255));

		/* Finalize -> Cleanup */
		func_RenderEventFinalize();
	}

	void QF::UI::Window::func_RenderEventPrepare()
	{
		/* Initialize backend */
		ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
		ImGui_ImplOpenGL3_Init("#version 130");

		/* Initalize context and glclear current buffer */
		glfwMakeContextCurrent(m_Window);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/* Create imgui frame */
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		/* Get window size */
		int _sx = 0;
		int _sy = 0;

		glfwGetWindowSize(m_Window, &_sx, &_sy);

		/* Create main panel for rendering at given window */
			/* Set panel  size */
		ImGui::SetNextWindowSize(ImVec2(_sx, _sy),
			ImGuiCond_Always
		);
		/* Set panel postion: default 0, 0*/
		ImGui::SetNextWindowPos(ImVec2(0, 0),
			ImGuiCond_Always
		);
		/* Set panel params */
		ImGui::Begin(m_Name.c_str(), nullptr,
			/* Flags */
			ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar
		);
	}

	void QF::UI::Window::func_RenderEventFinalize()
	{
		glfwPollEvents();
		/* Finalize draw list*/
		ImGui::End();
		/* Get render and pass data to open gl */
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		/* Swap buffers to finalize */
		glfwSwapBuffers(m_Window);
		/* Destroy backend */
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
	}