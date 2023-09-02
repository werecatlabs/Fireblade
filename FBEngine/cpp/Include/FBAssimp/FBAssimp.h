#ifndef FBAssimp_h__
#define FBAssimp_h__

#include <FBAssimp/FBAssimpPrerequisites.h>
#include <FBCore/Memory/SmartPtr.h>



namespace fb
{


	class FBAssimp
	{
	public:
		static SmartPtr<IMeshLoader> createAssimpLoader();
	};



}

#endif // FBAssimp_h__
