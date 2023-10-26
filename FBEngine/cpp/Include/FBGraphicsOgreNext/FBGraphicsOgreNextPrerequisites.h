#ifndef __OgreNextGfxPrerequisites__H
#define __OgreNextGfxPrerequisites__H

#include <FBCore/FBCorePrerequisites.h>
#include <FBGraphics/FBGraphicsPrerequisites.h>

// forward decs
class DeferredShadingSystem;
class ResourceGroupHelper;
class DecalCursor;
class Plot;
class WaterMesh;

namespace Colibri
{
    class CustomShape;
    class ColibriManager;
    class Widget;
    class Window;
    class Label;
}

namespace Forests
{
    class GeometryPageManager;
    class PageLoader;
    class PagedGeometry;
    class GrassLoader;
}  // namespace Forests

// forward declaration
namespace Ogre
{
    class HlmsColibriDatablock;

    class AnimationState;
    class Billboard;
    class BillboardSet;
    class Camera;
    class CompositorManager2;
    class Entity;
    class FrameListener;
    class Frustum;
    class Light;
    class ManualObject;
    class MovableObject;

    class Pass;
    class ParticleSystem;
    class Profiler;
    class Rectangle2D;
    class RenderTarget;
    class RenderWindow;
    class ResourceGroupManager;
    class ResourceManager;
    class SceneManager;
    class SceneNode;
    class TextureUnitState;
    class TextureGpu;
    class TextAreaOverlayElement;
    class TextAreaOverlayElement;
    class Viewport;
    class Technique;

    class Terra;
    class ShadowMapper;
    class TerraWorkspaceListener;

    class Terrain;
    class TerrainGlobalOptions;
    class TerrainGroup;
    class TerrainLayerBlendMap;
    class TerrainPaging;
    class PageManager;

    class InstancedEntity;
    class InstanceManager;

    class RenderSystem;
    class TextureUnitState;
    class TextureGpu;

    class StaticPluginLoader;

    class HlmsDatablock;
    class HlmsUnlitDatablock;
    class HlmsPbsTerraShadows;

    class Root;

    class Entity;
    class ParticleSystem;
    class RenderSystem;
    class OverlaySystem;

    class MetalPlugin;
    class GL3PlusPlugin;
    class GLES2Plugin;
    class D3D11Plugin;

    class HlmsJsonListener;

    class CompositorWorkspace;
    class Entity;
    class Pass;
    class ParticleSystem;
    class ParticleSystemManager;
    class Mesh;
    class IndexBufferPacked;
    class Item;
    class ScriptLoader;
    class Window;

    struct GpuTrackedResource;

    class GL3PlusPlugin;
    class MetalPlugin;

    class ObjectMemoryManager;

    // typedef StdMap<GpuTrackedResource*, ResourceLayout::Layout> ResourceLayoutMap;
    // typedef StdMap<GpuTrackedResource*, ResourceAccess::ResourceAccess> ResourceAccessMap;
}  // namespace Ogre

namespace Ogre
{
    namespace v1
    {
        class AnimationState;
        class Billboard;
        class BillboardSet;
        class Entity;
        class TextAreaOverlayElement;
        class Overlay;
        class OverlayElement;
        class OverlayContainer;
        class OverlayManager;
        class OverlaySystem;
        class WireBoundingBox;
    }  // namespace v1
}  // namespace Ogre

class TiXmlElement;
class AssimpLoader;

namespace fb
{
    namespace render
    {
        // forward dec
        class DynamicMesh;

        class WaterMesh;

        // forward decs
        class HDRListener;

        class CCameraOgreNext;
        class Compositor;
        class CompositorManager;
        class CDeferredShadingSystem;
        class CSceneNodeOgreNext;
        class CSceneManagerOgreNext;
        class CViewportOgreNext;
        class CMaterialTechniqueOgreNext;

        class CellSceneManagerFactory;

        class DynamicLines;
        class DepthOfFieldEffect;
        class ResourceLoadingListener;
        class MaterialListener;

        class ScreenSpaceEffect;
        class SSEffectRenderer;
        class sseffectManager;

        class LodManager;
        class LodObject;
        class LodPage;

        class DeferredShadingSystemTemplate;

        class SSEffectRenderer;
        class Quad;
        class WindowMacOS;

        class SkyRenderer;
        class SkyBoxRenderer;

        class WindowWin32;
        class WindowMacOS;

    }  // end namespace render
}  // end namespace fb

#endif
