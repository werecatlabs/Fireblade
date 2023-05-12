#ifndef DebugUtil_h__
#define DebugUtil_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Base/StringTypes.h>

namespace fb
{

	class DebugUtil
	{
	public:
		static String getStackTrace();
		static String getStackTraceForException(std::exception& e);
	};

}  // namespace fb

#endif  // DebugUtil_h__
