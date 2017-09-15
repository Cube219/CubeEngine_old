#include "PathString.h"

namespace cube
{
	namespace core
	{
		PathString::PathString(WString& string)
		{
			mStringData = string;
		}

		PathString::~PathString()
		{
		}
	}
}