#ifndef __OgreGfxPrerequisites__H
#define __OgreGfxPrerequisites__H

#include <FBCore/FBCorePrerequisites.h>
#include <FBGraphics/FBGraphicsPrerequisites.h>
#include <FBCore/Thread/Threading.h>
#include <FBCore/Memory/SmartPtr.h>

class DeferredShadingSystem;
class TiXmlElement;
class TiXmlElement;
class AssimpLoader;

namespace Forests
{
    class PagedGeometry;
}

namespace Forests
{
    class GeometryPageManager;
    class PageLoader;
    class PagedGeometry;
    class GrassLoader;
}  // namespace Forests

class Plot;

namespace Ogre
{
    class Profiler;
}

namespace Ogre
{
    class SceneManager;
    class SceneNode;
    class Frustum;
    class TextureUnitState;
    class Pass;
}  // namespace Ogre

// forward declarations
namespace Ogre
{
    class AnimationState;
    class Billboard;
    class BillboardSet;
    class Camera;
    class Entity;
    class FrameListener;
    class Light;
    class MovableObject;
    class Overlay;
    class OverlayElement;
    class OverlayContainer;
    class OverlayManager;
    class ParticleSystem;
    class RenderTarget;
    class RenderWindow;
    class ResourceGroupManager;
    class ResourceManager;
    class SceneManager;
    class SceneNode;
    class TextAreaOverlayElement;
    class TextAreaOverlayElement;
    class Viewport;
    class Technique;

    class Terrain;
    class TerrainGlobalOptions;
    class TerrainGroup;
    class TerrainPaging;
    class PageManager;

    class TerrainLayerBlendMap;
    class InstancedEntity;
    class InstanceManager;

    class RenderSystem;
    class TextureUnitState;

    class PSSMShadowCameraSetup;

    class StaticPluginLoader;
    class ScriptLoader;

    class D3D9Plugin;

    class ParticleSystemManager;

    class SceneManager;
    class SceneNode;
    class Entity;
    class ParticleSystem;

    class ScreenSpaceEffect;
}  // namespace Ogre

namespace OgreBites
{

    class SGTechniqueResolverListener;

}

namespace Ogre
{
    class Root;
    class SceneManager;
    class SceneNode;
    class Entity;
    class ParticleSystem;
    class RenderSystem;
    class OverlaySystem;

    class D3D9Plugin;
    class D3D11Plugin;
    class GLPlugin;
    class GL3PlusPlugin;
    class MetalPlugin;
    class GLES2Plugin;
}  // namespace Ogre

namespace Ogitors
{
    class OgitorsRoot;
    class CBaseSerializer;
    class CViewportEditor;
    class OgitorsPropertySetListener;
}

namespace fb
{
    class ResourceGroupHelper;

    namespace render
    {
        class LightDepthMapRttListener;

        class SSEffectRenderer;
        
        class CInstanceObjectOld;
        
        class DecalCursor;

        class CSceneNodeOgre;
        class CompositorManager;
        class ScreenSpaceEffect;
        class SSEffectRenderer;
        class sseffectManager;

        class LodManager;
        class LodObject;
        class LodPage;

        class CMaterialTechniqueOgre;
        class CInstanceManager;
        class CInstancedObject;

        class CDeferredShadingSystem;

        class CCameraOgre;

        class CameraVisibilityState;
        class DynamicLines;
        
        class DepthOfFieldEffect;
        class HDRListener;

        class CDebug;
        class CDebugLine;

        class CTextureOgre;
        class CTerrainOgre;

        class WaterMesh;
        
        class ResourceLoadingListener;
        class MaterialListener;
        class CellSceneManagerFactory;
        class BasicSceneManagerFactory;

        class WindowWin32;
        class WindowMacOS;

        class Texture3D;
        class ParticleTextureAtlas;

    }  // end namespace render
}  // end namespace fb

#endif
