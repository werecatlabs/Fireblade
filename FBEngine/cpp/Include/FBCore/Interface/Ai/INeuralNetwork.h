#ifndef INeuralNetwork_h__
#define INeuralNetwork_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    class INeuralNetwork : public ISharedObject
    {
    public:
        /** Virtual destructor. */
        ~INeuralNetwork() override = default;
    };

}  // namespace fb

#endif  // INeuralNetwork_h__
