#pragma once

#include <vector>
#include <Windows.h>
#include <time.h>
#include <fstream>
#include "threading.h"
#include "../SS_defines.h"
#include "../SS_file_utils.h"

#define LOG_DIR_NAME _T("SS_Logger\\")

#ifdef UNICODE
#define SS_LOG(logObj, level, fmt, ...) logObj.Log(level, __FILEW__, __LINE__, __FUNCTIONW__, fmt, ##__VA_ARGS__);
#define LOGINFO(logObj, fmt, ...) logObj.Log(framework::Diagnostics::LogLevel::Info, __FILEW__, __LINE__, __FUNCTIONW__, fmt, ##__VA_ARGS__);
#define LOGDEBUG(logObj, fmt, ...) logObj.Log(framework::Diagnostics::LogLevel::Debug, __FILEW__, __LINE__, __FUNCTIONW__, fmt, ##__VA_ARGS__);
#define LOGWARN(logObj, fmt, ...) logObj.Log(framework::Diagnostics::LogLevel::Warn, __FILEW__, __LINE__, __FUNCTIONW__, fmt, ##__VA_ARGS__);
#define LOGERROR(logObj, fmt, ...) logObj.Log(framework::Diagnostics::LogLevel::Error, __FILEW__, __LINE__, __FUNCTIONW__, fmt, ##__VA_ARGS__);

#define SS_LOGP(logObjPtr, level, fmt, ...) logObjPtr->Log(level, __FILEW__, __LINE__, __FUNCTIONW__, fmt, ##__VA_ARGS__);
#define LOGINFOP(logObjPtr, fmt, ...) logObjPtr->Log(framework::Diagnostics::LogLevel::Info, __FILEW__, __LINE__, __FUNCTIONW__, fmt, ##__VA_ARGS__);
#define LOGDEBUGP(logObjPtr, fmt, ...) logObjPtr->Log(framework::Diagnostics::LogLevel::Debug, __FILEW__, __LINE__, __FUNCTIONW__, fmt, ##__VA_ARGS__);
#define LOGWARNP(logObjPtr, fmt, ...) logObjPtr->Log(framework::Diagnostics::LogLevel::Warn, __FILEW__, __LINE__, __FUNCTIONW__, fmt, ##__VA_ARGS__);
#define LOGERRORP(logObjPtr, fmt, ...) logObjPtr->Log(framework::Diagnostics::LogLevel::Error, __FILEW__, __LINE__, __FUNCTIONW__, fmt, ##__VA_ARGS__);
#else
#define SS_LOG(logObj, level, fmt, ...) logObj.Log(level, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__);
#define LOGINFO(logObj, fmt, ...) logObj.Log(framework::Diagnostics::LogLevel::Info, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__);
#define LOGDEBUG(logObj, fmt, ...) logObj.Log(framework::Diagnostics::LogLevel::Debug, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__);
#define LOGINFO(logObj, fmt, ...) logObj.Log(framework::Diagnostics::LogLevel::Warn, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__);
#define LOGERROR(logObj, fmt, ...) logObj.Log(framework::Diagnostics::LogLevel::Error, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__);
#define LOGERROR(logObj, fmt, ...) logObj.Log(framework::Diagnostics::LogLevel::Param, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__);

#define SS_LOGP(logObjPtr, level, fmt, ...) logObjPtr->Log(level, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__);
#define LOGINFOP(logObjPtr, fmt, ...) logObjPtr->Log(framework::Diagnostics::LogLevel::Info, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__);
#define LOGDEBUGP(logObjPtr, fmt, ...) logObjPtr->Log(framework::Diagnostics::LogLevel::Debug, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__);
#define LOGINFOP(logObjPtr, fmt, ...) logObjPtr->Log(framework::Diagnostics::LogLevel::Warn, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__);
#define LOGERRORP(logObjPtr, fmt, ...) logObjPtr->Log(framework::Diagnostics::LogLevel::Error, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__);
#define LOGERRORP(logObjPtr, fmt, ...) logObjPtr->Log(framework::Diagnostics::LogLevel::Param, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__);
#endif

#define MAX_TEXT_BUFFER_SIZE 4096

namespace framework
{	
	namespace Diagnostics
	{
		enum class LogLevel
		{
			Info,		
			Debug,
			Warn,
			Error,
			Param,
			Cmd
		};

		enum class LogItem
		{
			Filename	= 0x1,
			LineNumber	= 0x2,
			Function    = 0x4,
			DateTime	= 0x8,		
			ThreadId	= 0x10,
			LoggerName  = 0x20,
			LogLevel	= 0x40
		};

		template <class ThreadingProtection> 
		class CLogger
		{
		private:
			struct StreamInfo
			{
				wostream* pStream;
				bool owned;
				LogLevel level;

				StreamInfo(wostream* pStream, bool owned, LogLevel level)
				{
					this->pStream = pStream;
					this->owned = owned;
					this->level = level;
				}
			};

		public:
			CLogger(framework::Diagnostics::LogLevel level,SSstring ModelName,
				int loggableItems = static_cast<int>(LogItem::Function) | static_cast<int>(LogItem::LineNumber) | static_cast<int>(LogItem::DateTime) | 
				static_cast<int>(LogItem::LoggerName) | static_cast<int>(LogItem::LogLevel)) 
				: m_level(level), ws_Model_Name(ModelName),m_loggableItem(loggableItems)
			{		
				SSstring ws_Dir_Path = GetModuleDirectory();
				ws_Dir_Path = ws_Dir_Path + LOG_DIR_NAME;
				if (DirectoryExists(ws_Dir_Path)== INVALID_PATH)
				{
					MakeDirectory(ws_Dir_Path);
				}
				ws_File_Path = ws_Dir_Path + ws_Model_Name+_T(".log");
				//Add OutStream 
				AddOutputStream(new std::wofstream(ws_File_Path, 8), true, framework::Diagnostics::LogLevel::Info);
			}

			~CLogger()
			{
				ClearOutputStreams();
			}

			void AddOutputStream(wostream& os, bool own, LogLevel level)
			{
				AddOutputStream(&os, own, level);
			}

			void AddOutputStream(wostream& os, bool own)
			{
				AddOutputStream(os, own, m_level);
			}

			void AddOutputStream(wostream* os, bool own)
			{
				AddOutputStream(os, own, m_level);
			}

			void AddOutputStream(wostream* os, bool own, LogLevel level)
			{
				StreamInfo si(os, own, level);
				m_outputStreams.push_back(si);
			}
			void ClearOutputStreams()
			{
				for(vector<StreamInfo>::iterator iter = m_outputStreams.begin(); iter < m_outputStreams.end(); iter++)
				{
					if(iter->owned) delete iter->pStream;
				}

				m_outputStreams.clear();
			}

			void Log(LogLevel level, LPCTSTR file, INT line, LPCTSTR func, LPCTSTR fmt, ...)
			{
				while(!m_threadProtect.Lock());
				TCHAR text[MAX_TEXT_BUFFER_SIZE] = { 0 };
				va_list args;
				va_start(args, fmt);
				_vstprintf_s(text, MAX_TEXT_BUFFER_SIZE, fmt, args);
				va_end(args);				

				for(vector<StreamInfo>::iterator iter = m_outputStreams.begin(); iter < m_outputStreams.end(); iter++)
				{
					if(level < iter->level)
					{
						continue;
					}

					bool written = false;
					SSostream * pStream = iter->pStream;
				
					if(m_loggableItem & static_cast<int>(LogItem::DateTime))
						written = write_datetime(written, pStream);
				
					if(m_loggableItem & static_cast<int>(LogItem::ThreadId))
						written = write<int>(GetCurrentThreadId(), written, pStream);

					if(m_loggableItem & static_cast<int>(LogItem::LoggerName))
						written = write<LPCTSTR>(ws_Model_Name.c_str(), written, pStream);

					if(m_loggableItem & static_cast<int>(LogItem::LogLevel))
					{
						TCHAR strLevel[6];
						loglevel_toString(level, strLevel);
						written = write<LPCTSTR>(strLevel, written, pStream);
					}

					if(m_loggableItem & static_cast<int>(LogItem::Function))
						written = write<LPCTSTR>(func, written, pStream);

					if(m_loggableItem & static_cast<int>(LogItem::Filename))
						written = write<LPCTSTR>(file, written, pStream);

					if(m_loggableItem & static_cast<int>(LogItem::LineNumber))
						written = write<int>(line, written, pStream);
								
					written = write<LPCTSTR>(text, written, pStream);

					if(written)
					{
						(*pStream) << endl;
						pStream->flush();
					}
				}

				m_threadProtect.Unlock();
			}

			void Log(LogLevel level, LPCTSTR file, INT line, LPCTSTR func, SSstring fmt, ...)
			{
				CString csTemp = fmt.c_str();
				while (!m_threadProtect.Lock());
				TCHAR text[MAX_TEXT_BUFFER_SIZE] = { 0 };
				va_list args;
				va_start(args, csTemp);
				_vstprintf_s(text, MAX_TEXT_BUFFER_SIZE, csTemp, args);
				va_end(args);

				for (vector<StreamInfo>::iterator iter = m_outputStreams.begin(); iter < m_outputStreams.end(); iter++)
				{
					if (level < iter->level)
					{
						continue;
					}

					bool written = false;
					SSostream * pStream = iter->pStream;

					if (m_loggableItem & static_cast<int>(LogItem::DateTime))
						written = write_datetime(written, pStream);

					if (m_loggableItem & static_cast<int>(LogItem::ThreadId))
						written = write<int>(GetCurrentThreadId(), written, pStream);

					if (m_loggableItem & static_cast<int>(LogItem::LoggerName))
						written = write<LPCTSTR>(ws_Model_Name.c_str(), written, pStream);

					if (m_loggableItem & static_cast<int>(LogItem::LogLevel))
					{
						TCHAR strLevel[6];
						loglevel_toString(level, strLevel);
						written = write<LPCTSTR>(strLevel, written, pStream);
					}

					if (m_loggableItem & static_cast<int>(LogItem::Function))
						written = write<LPCTSTR>(func, written, pStream);

					if (m_loggableItem & static_cast<int>(LogItem::Filename))
						written = write<LPCTSTR>(file, written, pStream);

					if (m_loggableItem & static_cast<int>(LogItem::LineNumber))
						written = write<int>(line, written, pStream);

					written = write<LPCTSTR>(text, written, pStream);

					if (written)
					{
						(*pStream) << endl;
						pStream->flush();
					}
				}

				m_threadProtect.Unlock();
			}

		private:			
			int m_loggableItem;
			LogLevel m_level;
			SSstring ws_Model_Name;
			SSstring ws_File_Path;
			vector<StreamInfo> m_outputStreams;
			ThreadingProtection m_threadProtect;
	
			template <class T> inline bool write(T data, bool written, wostream* strm)
			{
				if(written == true)
				{
					(*strm) << _T("     ");
				}

				(*strm) << data;
				return true;
			}

			inline bool write_datetime(bool written, wostream* strm)
			{
				if(written == true)
				{
					(*strm) << _T("     ");
				}

				time_t szClock;
				tm newTime;

				time( &szClock );
				localtime_s(&newTime, &szClock);
			
				TCHAR strDate[10] = { _T('\0') };
				TCHAR strTime[10] = { _T('\0') };

				_tstrdate_s(strDate, 10);
				_tstrtime_s(strTime, 10);

				(*strm) << strDate << _T("     ") << strTime;
		
				return true;
			}

			void loglevel_toString(LogLevel level, LPTSTR strLevel)
			{
				switch (level)
				{
				case LogLevel::Error:
					_tcscpy_s(strLevel, 4, _T("ERR"));
					break;

				case LogLevel::Warn:
					_tcscpy_s(strLevel,4, _T("WRN"));
					break;

				case LogLevel::Info:
					_tcscpy_s(strLevel, 4, _T("INF"));
					break;

				case LogLevel::Debug:
					_tcscpy_s(strLevel, 4, _T("DBG"));
					break;

				case LogLevel::Param:
					_tcscpy_s(strLevel, 4, _T("PRM"));
					break;

				case LogLevel::Cmd:
					_tcscpy_s(strLevel, 4, _T("CMD"));
					break;
				}
			}
		};
	}
}

