#include "../QF.h"

/*========================= Filesystem =========================*/
	/*========================= Open =========================*/
		/*========================= Buffer =========================*/
			QF::Utils::Filesystem::Open::Buffer::Buffer(const std::vector<std::string>& _Buffer)
				: m_Buffer{_Buffer} {};

			const std::vector<std::string>& QF::Utils::Filesystem::Open::Buffer::g_Vector() const
			{
				return m_Buffer;
			}

			const std::string QF::Utils::Filesystem::Open::Buffer::g_String(const std::string& _LineEnding) const
			{
				return QF::Utils::Transform::g_StringFromStringVector(m_Buffer, _LineEnding);
			}
		/* Constructor */
		QF::Utils::Filesystem::Open::Open(const std::filesystem::path& _Path) 
			: m_Path{_Path}
		{
			/* Log to debug */
			QF::Utils::Debug().Insert(QF::Utils::Debug::LogHint::MESSAGE,
				__FUNCTION__, std::format("Filesystem initialized for file: {}", m_Path.string()));
		}

		void QF::Utils::Filesystem::Open::func_FileCannotBeOpened() const
		{
			QF::Utils::Debug().Insert(QF::Utils::Debug::LogHint::ERROR, __FUNCTION__,
				std::format("File cannot be opened: {}", m_Path.string()));
		}

		const QF::Utils::Filesystem::Open::Buffer QF::Utils::Filesystem::Open::g_Buffer() const
		{
			QF::Utils::Debug().Insert(QF::Utils::Debug::LogHint::MESSAGE, __FUNCTION__,
				std::format("Attempting to read buffer from file: ", m_Path.string()));

			std::vector<std::string> _Buffer; 

			/* Open */
			std::fstream _File; _File.open(
				m_Path, std::ios::in
			);
			
			/* Error with file */
			if (!_File) { func_FileCannotBeOpened(); return Buffer{}; };

			/* Collect buffer */
			std::string _Segment; 

			while (std::getline(_File, _Segment))
				_Buffer.push_back(g_WithoutCRLF(_Segment));
			/* Log success */
			QF::Utils::Debug().Insert(QF::Utils::Debug::LogHint::MESSAGE, __FUNCTION__,
				std::format("Successfully got buffer from file: ", m_Path.string()));

			/* Let the file automaticly close, return Buffer struct */
			return Buffer{ _Buffer };
		}

		const std::string QF::Utils::Filesystem::Open::g_WithoutCRLF(const std::string& _Str) const
		{
			return QF::Utils::Transform::g_WithoutOccurences(QF::Utils::Transform::g_WithoutOccurences(
				QF::Utils::Transform::g_WithoutOccurences(_Str, "\r\n"),
				"\r"), "\n");
		}

		const bool QF::Utils::Filesystem::Open::s_Buffer(const Buffer& _Buffer, const bool& _Add, const std::string& _LineEnding) const
		{
			QF::Utils::Debug().Insert(QF::Utils::Debug::LogHint::MESSAGE, __FUNCTION__,
				std::format("Attempting to write to file: {} [add: {}]", 
					m_Path.string(), _Add
				));

			std::string _BufferToWrite = _Buffer.g_String(_LineEnding);

			if (_Add == true)
			{ /* Add buffer passed as argument to end of the file */
				const std::string _CurrentBuffer = g_Buffer().g_String(_LineEnding);

				/* Check if empty to avoid adding extra space */
				if (!_BufferToWrite.empty())
				{
					_BufferToWrite = std::format("{}{}{}",
						_CurrentBuffer, _LineEnding, _BufferToWrite
					);
				}
			}

			/* Open */
			std::fstream _File;

			/* Error while opening */
			if (!_File) { func_FileCannotBeOpened(); return false; };

			for (int _Tries = 0; _Tries < 5; _Tries++)
			{
				_File.open(m_Path, std::ios::binary);
				_File << _BufferToWrite;
				_File.close();

				/* Check for data loss */
				if (g_Buffer().g_String(_LineEnding)
					== _BufferToWrite)
				{ /* Successfully written */
					QF::Utils::Debug().Insert(QF::Utils::Debug::LogHint::MESSAGE, __FUNCTION__,
						std::format("Successfully written buffer to file: {}", m_Path.string())
						);
					/* Return true to inform about success */
					return true; 
				}
			}

			/* Log failure */
			QF::Utils::Debug().Insert(QF::Utils::Debug::LogHint::ERROR, __FUNCTION__,
				std::format("Failed to write buffer in all attempts, file: ", m_Path.string())
				);
			/* Return false to inform about failure*/
			return false; 
		}
