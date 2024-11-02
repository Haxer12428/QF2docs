#pragma once

/*========================= Includes =========================*/

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <string>
#include <format>
#include <iostream>
#include <unordered_map>
#include <array>
#include <vector>
#include <map>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <mutex>
#include <memory>
#include <thread>
#include <functional>
#include <typeindex>
#include <type_traits>

/*========================= QF =========================*/
const std::string __QF_EMPTY_STRING = "";

namespace QF
{
	/*========================= Utils =========================*/
	namespace Utils
	{

		class Transform
		{
		public:
			static const std::string g_StringFromStringVector(const std::vector<std::string>& _Vec,
				const std::string& _LineEnding = "\r\n");
			static const std::string g_WithoutOccurences(const std::string& _Str, const std::string& _Occurence);
		};

		class Time
		{
		public:
			/* Returns current time applied to given format */
			static const std::string g_CurrentTimestamp(const std::string& _Format); 
		};

	

		/*========================= Debug =========================*/
		class Debug
		{
		public:
#define __QF_ENUM_STRING_TO_CASE(x) { x, #x }
		/* Global debug hint system */
		public:
			enum class LogHint : int 
			{
				ERROR,
				WARNING,
				MESSAGE,
				IMPORTANT
			};

			enum class PrintHint : int
			{
				PRINT_ERROR,
				PRINT_WARNING,
				PRINT_MESSAGE,
				PRINT_IMPORTANT,
				PRINT_WITH_COLORS,
				PRINT_TO_FILE,
				COUNT /* For initialization puproses, do not use */
			};
		private:
			const std::map<LogHint, PrintHint> m_PrintHintMap =
			{
				{ LogHint::ERROR, PrintHint::PRINT_ERROR },
				{ LogHint::MESSAGE, PrintHint::PRINT_MESSAGE },
				{ LogHint::WARNING, PrintHint::PRINT_WARNING },
				{ LogHint::IMPORTANT, PrintHint::PRINT_IMPORTANT },
			};

			const std::map<LogHint, std::string> m_LogHintMap =
			{
				__QF_ENUM_STRING_TO_CASE(LogHint::ERROR),
				__QF_ENUM_STRING_TO_CASE(LogHint::MESSAGE),
				__QF_ENUM_STRING_TO_CASE(LogHint::WARNING),
				__QF_ENUM_STRING_TO_CASE(LogHint::IMPORTANT),
			};

			static std::unordered_map<PrintHint, bool> m_GlobalDebugPrintHints;
			static bool m_GlobalDebugHintsInitialized;
			static std::filesystem::path m_GlobalLogFilePath;
			static std::once_flag m_InitializerInitOnceFlag;
		public:
			static void s_GlobalDebugPrintHint(PrintHint _Hint, const bool& _Value);
			static void s_MultipleGlobalDebugPrintHints(const std::vector<PrintHint>& _Hints, const bool& _Value);
			static void global_InitializeHints();
			static void s_GlobalLogFilePath(const std::filesystem::path& _Path);
		public:
		/* Constructor */
		   Debug();
		/* Logging system */
			void Insert(LogHint _LogHint, const std::string& Name, const std::string& _Arguments);
		private:
			void hdl_InsertHintsInitialize();

			static void func_ClearDebugFile(); 

			const std::string g_InsertDebugData(
				const std::string& _Hint, 
				const std::string& _Name
				) const; 

			const std::string g_InsertDataToLog(
				const std::string& _DebugData, const std::string& _Arguments
				) const;
			const std::string g_InsertDataToPrint(
				const std::string& _DebugData, const std::string& _Arguments, const std::string& _Color
				) const; 

			void func_InsertLogDataToFile(const std::string& _Data);
			void func_InsertPrint(const std::string& _Data) const;
			void hdl_InsertLogDataToFileCannotBeOpened() const;

			const std::string g_InsertPrintColor(LogHint _Hint) const;
			
		/* Global initialization */
			
		private:
			
		};

		/*========================= Vec2 =========================*/
		struct Vec2 
		{
		public:
		/* Global vec2 hint system */
			enum class Hint : int 
			{
				DIVIDE_BY_ZERO
			};

			enum class HintValue : int
			{
				DIVIDE_BY_ZERO_ABORT,
				DIVIDE_BY_ZERO_RETURN_SELF
			};

			/* Hints storage */
			static std::unordered_map<QF::Utils::Vec2::Hint, QF::Utils::Vec2::HintValue> m_GlobalVec2Hints;
			
			/* Be carefull with values
				U should use hint operations from enum class!
			*/
			static void s_GlobalVec2Hint(QF::Utils::Vec2::Hint _Hint, QF::Utils::Vec2::HintValue _Value);
		public:
		/* Constructor */
			Vec2();
			Vec2(const int _x, const int _y);
			Vec2(const double _x, const double _y);
			Vec2(const float _singular);
			Vec2(const float _x, const float _y);
			
			float& x = m_x;
			float& y = m_y;
		/* Mathematical operations */
			/* Addition */
				QF::Utils::Vec2 operator+(const QF::Utils::Vec2& _Other) const; 
				QF::Utils::Vec2 operator+(const float _Other) const;
				friend QF::Utils::Vec2 operator+(const float _Scalar, const QF::Utils::Vec2& _Vector);
				
			/* Substraction */
				QF::Utils::Vec2 operator-(const QF::Utils::Vec2& _Other) const; 
				QF::Utils::Vec2 operator-(const float _Other) const; 
				friend QF::Utils::Vec2 operator-(const float _Scalar, const QF::Utils::Vec2& _Vector);
			/* Multiplication */
				QF::Utils::Vec2 operator*(const QF::Utils::Vec2& _Other) const;
				QF::Utils::Vec2 operator*(const float Other) const;
				friend QF::Utils::Vec2 operator*(const float _Scalar, const QF::Utils::Vec2& _Vector);
			/* Division */
		public:
				QF::Utils::Vec2 operator/(const QF::Utils::Vec2& _Other) const;
				QF::Utils::Vec2 operator/(const float _Other) const; 
				friend QF::Utils::Vec2 operator/(const float _Scalar, const QF::Utils::Vec2& _Vector);
		private:
				static const QF::Utils::Vec2 g_Divider(const QF::Utils::Vec2& _Current);
		public:
			/* Plus */
				QF::Utils::Vec2& operator+=(const QF::Utils::Vec2& _Other); 
				QF::Utils::Vec2& operator+=(const float _Other);
			/* Minus */
				QF::Utils::Vec2& operator-=(const QF::Utils::Vec2& _Other);
				QF::Utils::Vec2& operator-=(const float _Other);
			/* Equal and not equal */
				const bool operator==(const QF::Utils::Vec2& _Other) const; 
				const bool operator!=(const QF::Utils::Vec2& _Other) const;
		/* Transformation */
			const std::string g_String() const; 
			const char* g_CharPtr() const; 
			const ImVec2 g_ImVec2() const;
		private:

			float m_x; 
			float m_y;
		};
		/*========================= Filesystem =========================*/

		namespace Filesystem
		{
			class Open
			{
			public:
				/* Constructor */
				Open(const std::filesystem::path& _Path);
				
				struct Buffer
				{
				public:
					Buffer(const std::vector<std::string>& _Buffer = {});

					const std::vector<std::string>& g_Vector() const; 
					const std::string g_String(const std::string& _LineEnding = "\r\n") const;
				private:
					const std::vector<std::string> m_Buffer; 
				};

				const Buffer g_Buffer() const;
				const bool s_Buffer(const Buffer& _Buffer, const bool& _Add = false, const std::string& _LineEnding = "\r\n") const;
			private:
				void func_FileCannotBeOpened() const;
				const std::string g_WithoutCRLF(const std::string& _Str) const; 

			private:
				std::filesystem::path m_Path;
			};
		};
	}

	namespace UI
	{

		namespace EventSystem
		{
		/* Main Event Instance */
			class Event {
			public:
				virtual ~Event() = default; // Ensure proper cleanup for derived classes
			};
				
		/* Events declared by default */
			class RenderEvent : public Event
			{
			public:
				RenderEvent() = default;	
			};
		/* Event handler */
			class EventHandler {
			private:
			/* Event handler funcion */
				using EventHandlerFunc = std::function<void(Event&)>;

			/* Liseners */
				std::unordered_map<std::type_index, 
					std::vector<EventHandlerFunc>
					> m_Listeners;

			public:

				/* Subscibe event, _EventType, class instance,& class function definition */
				template <typename __EventType, typename __ClassType>
				void Subscribe(__ClassType* _Instance, 
					void (__ClassType::* _MemberFunction)(__EventType&)
					) 
				{
					m_Listeners[typeid(__EventType)].emplace_back(
						/* Add to liseners */
						[_Instance, _MemberFunction](Event& _Evt) 
						{
							/* Cast event */
							(_Instance->*_MemberFunction)(
								static_cast<__EventType&>(_Evt)
								);
						}

					);
				}
				
				/* Dispatch event (call event), _EventType */
				template <typename __EventType>
				void Dispatch(__EventType& _Evt) 
				{
					auto _It = m_Listeners.find(typeid(__EventType));

					/* Check for existance */
					if (_It == m_Listeners.end())
						return;
					for (const auto& handler : _It->second) 
					{ /* Dispatch for a listener */
						handler(_Evt);
					}
				}
			};
		};

		class Window; 
		class Panel;
		class App; 
		class Element;

		class Element
		{
		public:
			Element() = default;	

			/* what am ai? */
			void im_Panel(Panel* _Panel, Element* _Parent);
			void im_Window(Window* _Window);
			
			/* am i this? */
			Panel* g_Panel() const;
			Window* g_Window() const;
			/* absolute parent */
			Window* g_AbsoluteParent() const; 
		private:
			Panel* m_Panel = nullptr;
			Window* m_Window = nullptr;
			Window* m_AbsoluteParent = nullptr;

		};

		class Panel : public Element
		{ public: 
		/* Constructor & destructor */
			Panel(Element* _Parent, const QF::Utils::Vec2& _Position, const QF::Utils::Vec2& _Size);
			~Panel();
		/* Children handling */
			void im_Children(Panel* _Child);
		private:
			QF::Utils::Vec2 m_Position;
			QF::Utils::Vec2 m_Size; 
			Element* m_Parent; 

			std::vector<Panel*> m_Children;
			/* Event handler */
			EventSystem::EventHandler* m_EventHandler;
		};


		class Window : public Element	
		{
		public:
			Window(App* _App, const std::string& _Name, const QF::Utils::Vec2& _Size = {800, 600});
			~Window();
		private:
			const bool func_GLFWWindowCreate();
			const bool func_ImGuiCreateContext();
		public:
			void hk_MainLoop();
		private:
		/* Window events, each are menaged individually by each window */
			void hk_RenderEvent(); 
			/* prepare for render */
			void func_RenderEventPrepare(); 
			void func_RenderEventFinalize();
		public:
			GLFWwindow* g_GLFWwindow() const;
			const size_t g_ID() const;
			void im_Children(Panel* _Panel);
		private:
			GLFWwindow* m_Window;
			QF::Utils::Vec2 m_Size; 
			std::string m_Name;
			size_t m_ID;
			std::vector<Panel*> m_Children; 
		/* Event handler */
			EventSystem::EventHandler* m_EventHandler;
		};

		class App
		{
		public:
			App();

			void Implement();
		/* Main on init */
			virtual bool on_Init();

		/* Children declaration */
			void im_Children(Window*);
			const size_t g_GeneratedID();
		private:
			void func_MainLoopCloseChildren(Window* _Child, const size_t& _Iterator);
			void func_MainLoopHandleClosingChildren(std::vector<Window*>& _WaitingForClose);
			void func_MainLoopHandleChild(Window* _Child, std::vector<Window*>& _WaitingForClose);
			void hk_MainLoop();
		private: 
			size_t m_LastGivenID;
			std::vector<Window*> m_Children;
		};
	}



}

