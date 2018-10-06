#pragma once

#include "LogWriter.h"
#include "Base\format.h"

#define CHECK_ASSERTION(expr, failMsg) \
	if(!expr){ \
		cube::core::LogWriter::WriteLog(fmt::format(L"Assertion failed! {0}({1}) : {2}", __FILE__, __LINE__, failMsg)); \
		assert(expr && failMsg); \
	}

#define CHECK_ASSERTION_LITE(expr, failMsg) \
	if(!expr){ \
		cube::core::LogWriter::WriteLog(fmt::format(L"Assertion failed! {0}({1}) : {2}", __FILE__, __LINE__, failMsg)); \
	}

#ifdef _DEBUG
	#define CHECK_ASSERTION_DEBUG(expr, failMsg) \
		if(!expr){ \
			cube::core::LogWriter::WriteDebugLog(fmt::format(L"Assertion failed! {0}({1}) : {2}", __FILE__, __LINE__, failMsg)); \
			assert(expr && failMsg); \
		}

	#define CHECK_ASSERTION_LITE_DEBUG(expr, failMsg) \
		if(!expr){ \
			cube::core::LogWriter::WriteDebugLog(fmt::format(L"Assertion failed! {0}({1}) : {2}", __FILE__, __LINE__, failMsg)); \
		}
#else // _DEBUG
	#define CHECK_ASSERTION_DEBUG(expr, msg)
	#define CHECK_ASSERTION_LITE_DEBUG(expr, msg)
#endif // _DEBUG


// TODO: 차후 개선