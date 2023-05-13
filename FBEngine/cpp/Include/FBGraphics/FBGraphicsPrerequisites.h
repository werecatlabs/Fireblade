#ifndef FBGraphicsPrequisites_h__
#define FBGraphicsPrequisites_h__

#include <FBCore/FBCorePrerequisites.h>

//forward decs
namespace Ogre
{
    class Mesh;
}

namespace irr
{
    namespace scene
    {
        class IAnimatedMesh;
        class IMesh;
    }  // namespace scene
}  // namespace irr

class FCDGeometryMesh;

namespace fb
{

    class GraphicsObjectState;

    class AnimationKeyBinary;
    class AnimationBinary;
    class AnimationSetBinary;
    class BinaryScene;
    class EntityBinary;
    class SceneNodeBinary;

}  // namespace fb

#endif  // FBGraphicsPrequisites_h__
