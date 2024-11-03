#pragma once

/*========================= Includes =========================*/

#define NOMINMAX
#include <algorithm>
#include <windows.h>
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
				CRITICAL_ERROR,
				WARNING,
				MESSAGE,
				IMPORTANT
			};

			enum class PrintHint : int
			{
				PRINT_CRITICAL_ERROR,
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
				{ LogHint::CRITICAL_ERROR, PrintHint::PRINT_CRITICAL_ERROR },
				{ LogHint::MESSAGE, PrintHint::PRINT_MESSAGE },
				{ LogHint::WARNING, PrintHint::PRINT_WARNING },
				{ LogHint::IMPORTANT, PrintHint::PRINT_IMPORTANT },
			};

			const std::map<LogHint, std::string> m_LogHintMap =
			{
				__QF_ENUM_STRING_TO_CASE(LogHint::CRITICAL_ERROR),
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
			Vec2(const ImVec2& _Vec);
			
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
			/* Set operator */
				Vec2& operator=(const Vec2& _Other);
		/* Transformation */
			const std::string g_String() const; 
			const char* g_CharPtr() const; 
			const ImVec2 g_ImVec2() const;
		private:

			float m_x; 
			float m_y;
		};
		/*========================= Math =========================*/

		class System
		{
		public:
			static const RECT g_TaskbarRect();
			static const QF::Utils::Vec2 g_TaskbarPosition();
			static const QF::Utils::Vec2 g_TaskbarSize();
		};

		class Math
		{
		public:
			static const QF::Utils::Vec2 g_TextSize(const std::string& _Text, ImFont* _Font = nullptr);
			static const QF::Utils::Vec2 g_TextCenteredPosition(const Vec2& _Position, const Vec2& _Size, const std::string& _Text, ImFont* _Font = nullptr, const Vec2& _Offsets = {});
			static const bool is_InBounds(const Vec2& _Position, const Vec2& _Starting, const Vec2& _Size);
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

				struct Info
				{
				public:
					Info(const long& _Lines, const long& _Chars);

					const long& g_Lines() const; 
					const long& g_Chars() const;
				private:
					const long m_Lines; 
					const long m_Chars; 
				};

				const Info g_Info() const;
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
		class Panel;

		class Helpers
		{ 
		public:
			static ImFont* g_FontFromTTF(const std::filesystem::path& _Path, const float& _PixelSize, const ImFontConfig* _FontConfig = nullptr, const ImWchar* _GlyphRanges = 0);

		};

		namespace EventSystem
		{
		/* Main Event Instance */
			class Event {
			public:
				virtual ~Event() = default; // Ensure proper cleanup for derived classes
			};
				
		/* Events declared by default */
			class MouseMotionEvent : public Event
			{
			public:
				MouseMotionEvent(QF::UI::Panel* _Panel);

				const QF::Utils::Vec2 g_GlobalPosition() const; 
				const QF::Utils::Vec2 g_Position() const; 
				QF::UI::Panel* g_Panel() const;
			private:
				const QF::Utils::Vec2 m_Position; 
				QF::UI::Panel* m_Panel;
			};

			class MouseClickedEvent : public MouseMotionEvent
			{
			public:
				MouseClickedEvent(QF::UI::Panel* _Panel); 
			};

			class MousePanelDragEvent : public MouseMotionEvent
			{
			public: 
				MousePanelDragEvent(Panel* _Panel,  const QF::Utils::Vec2& _WhereClicked); 

				const QF::Utils::Vec2 g_ClickPosition() const;
				const QF::Utils::Vec2 g_ClickPositionGlobal() const;
			private:
				const QF::Utils::Vec2 m_ClickPosition; 
			};

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
				~EventHandler() {};
				EventHandler() {};

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
			~Element() = default; 

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
			void Destroy();
			void DestroyWindowCall();
		/* Position offset handling */
			void func_UpdatePositionOffset();
			void func_UpdateSizeOffset();
		private:
			void func_UpdatePositionOffsetForChildren();
			void func_UpdateSizeOffsetForChildren();
			void func_UpdateAllDependent(); 
			void func_UpdateVisibility();
			void func_UpdateVisibilityForChildren(); 
		public:
		/* Children handling */
			void im_NoLongerChildren(Panel* _Child);
			void im_Children(Panel* _Child);
			const size_t g_Id() const; 
		/* Getting positions */
			const QF::Utils::Vec2 g_Position() const; 
			const QF::Utils::Vec2 g_PositionOffset() const;
			const QF::Utils::Vec2 g_SizeOffset() const;
			const QF::Utils::Vec2 g_Size() const;  
			const QF::Utils::Vec2 g_FinalPosition() const;
			const QF::Utils::Vec2 g_FinalPositionOffset() const;
			const bool is_InBounds(const QF::Utils::Vec2& _Position) const; 
			const bool g_Visibility() const;
		/* Setting positions */
			void s_Position(const QF::Utils::Vec2 _Position); 
			void s_Size(const QF::Utils::Vec2 _Size);
			void s_Visible(const bool& _State);
		private:
			void func_EraseChildren();
		public:
			void func_EraseNullChildren();
		public:
		/* Event propagation */
			EventSystem::EventHandler* g_EventHandler();

		private:
			QF::Utils::Vec2 m_PositionOffset;
			QF::Utils::Vec2 m_SizeOffset;
		private:
			QF::Utils::Vec2 m_Position;
			QF::Utils::Vec2 m_Size; 
			Element* m_Parent; 

			bool m_Visible;
			std::vector<Panel*> m_Children;
			/* Event handler */
			EventSystem::EventHandler* m_EventHandler;

			size_t m_ID = 0; 
		};


		class Window : public Element	
		{
		public:
			class TitleBar : public QF::UI::Panel
			{
			public:
				/* Constructor & Destructor */
				TitleBar(Window* _Window);

				~TitleBar();

				struct Hints
				{
					ImU32 m_BackgroundColor;
					ImU32 m_TitleColor;
					ImFont* m_TitleFont;
				};
			
			private:
				Hints m_Hints;
			private:
				/* Initialize default hints */
				void func_InitializeDefaultHints(); 

				/* Hooks */
				void func_InitializeHooks();

			/* Render bg & name */
				void hk_Render(QF::UI::EventSystem::RenderEvent& _Event);
			/* Drag the window */
				void hk_MousePanelDrag(QF::UI::EventSystem::MousePanelDragEvent& _Event);
			private:
				/* Layout handling */
				void func_SetLayout();

			private:
				Window* m_Window;
			};

		public:
			Window(App* _App, const std::string& _Name, const QF::Utils::Vec2& _Size = {800, 600});
			~Window();
		private:
			const bool func_GLFWWindowCreate();
			const bool func_ImGuiCreateContext();
			void func_SubscribeEventsForPropagation();
		public:
			void hk_MainLoop();
		private:
		/* Window events, each are menaged individually by each window */
			void hk_RenderEvent(); 
			void hk_MouseMotionEvent(); 
			void hk_MouseClickEvent(); 
			void hk_MousePanelDragEvent();

			template<typename __EventType>
			void hk_TopToBottomPanelBasedEventPropagation(
				const std::function<bool(Panel*)>& _Condition, const std::function<__EventType&(Panel*)>& _Event)
			{
				/* Dispatch from top to bottom */
				for (int _Iterator = static_cast<int>(m_Children.size() - 1); _Iterator >= 0; --_Iterator)
				{
					Panel* _Child = m_Children[_Iterator];
					/* Check if even visible */
					if (_Child != nullptr && _Child->g_Visibility() && _Condition(_Child))
					{
						/* Dispatch & return a loop */
						m_Children[_Iterator]->g_EventHandler()->Dispatch(_Event(_Child));
						return;
					}
				}
			}

			void hk_RenderEventPropagation(EventSystem::RenderEvent& _Event);
			/* prepare for render */
			void func_RenderEventPrepare(); 
			void func_RenderEventFinalize();
		public:
			const QF::Utils::Vec2 g_AbsoluteMousePosition() const;
			const QF::Utils::Vec2 g_WindowPosition() const;
			ImGuiContext* g_ImGuiContext() const;
			GLFWwindow* g_GLFWwindow() const;
			const size_t g_ID() const;
			const std::string& g_Name() const; 
			const QF::Utils::Vec2 g_Size() const;
			void s_Position(const QF::Utils::Vec2& _New);
		/* Children handling */
		public:
			void im_NoLongerAChildren(Panel* _Panel);
			void im_Children(Panel* _Panel);
			const size_t g_GeneratedID();
		private:
			void func_ChildrenDestroy();
		public:
			void func_EraseNullChildren();
			/* Params */
		private:
			GLFWwindow* m_Window;
			QF::Utils::Vec2 m_Size; 
			std::string m_Name;
			size_t m_ID;
			std::vector<Panel*> m_Children; 

			size_t m_LastId = 0;
		/* Event handler */
			EventSystem::EventHandler* m_EventHandler;
			/* MouseMotionEvent */
				QF::Utils::Vec2 m_MouseMotionEventLastPlaced;
			/* MouseClickEvent */
				bool m_MouseClickEventLastFrameHeld = true;
			/* MousePanelDragEvent */
				QF::Utils::Vec2 m_MousePanelDragEventClickedAtPos;
				Panel* m_MousePanelDragEventClickedAtPanel = nullptr;
		/* Taskbar */
			TitleBar* m_TitleBar; 

			ImGuiContext* m_Context; 
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

		class SimpleDC
		{
		public:
			SimpleDC(Panel* _Panel);
			~SimpleDC(); 

			void draw_Rect(
				const QF::Utils::Vec2& _Position, const QF::Utils::Vec2& _Size, const ImColor& _Color,
				const float& _Rounding = 0.0f, ImDrawFlags _Flags = 0, const float& _Thickness = 1.0f
			);

			void draw_RectFilled(
				const QF::Utils::Vec2& _Position, const QF::Utils::Vec2& _Size, const ImColor& _Color,
				const float& _Rounding = 0.0f, ImDrawFlags _Flags = 0
			);

			void draw_Text(
				const QF::Utils::Vec2& _Position, const ImColor& _Color, const std::string& _Text, ImFont* _Font = nullptr
			);
		private:
			const QF::Utils::Vec2 g_Vec2(const float& _x, const float& _y) const;
			const QF::Utils::Vec2 g_Vec2(const QF::Utils::Vec2& _Vector) const; 

		/* Render helpers */
			void func_StartDrawing(); 
			void func_StopDrawing(); 
		private:
			ImDrawList* m_DrawList;
			Panel* m_Panel; 
		};
	}



}

