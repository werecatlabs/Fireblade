#ifndef FBPrerequisites_h__
#define FBPrerequisites_h__

#include <FBCore/FBCoreTypes.h>
#include <string>
#include <vector>

// forward declarations
class TiXmlElement;
class TiXmlNode;
class TiXmlElement;
class TiXmlDocument;
class TiXmlHandle;
struct cJSON;

namespace boost
{
    namespace json
    {
        class object;
    }  // namespace json
}  // namespace boost

namespace rttr
{
    class instance;
}  // namespace rttr

namespace Opcode
{
    class RayCollider;
    class Model;
    class MeshInterface;
}  // namespace Opcode

namespace IceMaths
{
    class Point;
}

namespace fb
{

    class IObject;
    class ISharedObject;
    class ISharedObjectListener;
    class FactoryManager;
    class Property;
    class Properties;
    class IData;
    class IObjectYield;
    class Handle;

    class ILockTrackerListener;

    class InputState;
    class IInput;

    namespace core
    {
        class IApplicationClient;
        class IApplicationManager;
        class IPluginEvent;
        class IPluginInterface;

        class ApplicationManager;
        class ApplicationManagerMT;
        class PluginEvent;

    }  // end namespace core

    class IConsole;

    // aerodynamics
    class IAerodymanicsWind;
    class IAircraftCallback;
    class IAircraft;
    class IAerofoil;
    class IAircraftPowerUnit;
    class IAircraftPropeller;
    class IAircraftPropellerUnit;
    class IAircraftWing;
    class IAircraftControlSurface;
    class IAircraftPowerUnit;
    class IAircraftBody;
    class IAircraftPropWash;

    namespace scene
    {
        class IActor;
        class IActorProxy;
        class ICameraController;
        class ICameraManager;
        class IComponent;
        class IComponentEvent;
        class IComponentEventListener;
        class IDirector;
        class IPrefab;
        class IPrefabManager;
        class IScene;
        class ISceneManager;
        class ISubComponent;
        class ITransform;

    }  // namespace scene

    // animation
    class IAnimation;
    class IAnimationInterface;
    class IAnimationKeyFrame;
    class IAnimationMorphKeyFrame;
    class IAnimationPoseKeyFrame;
    class IAnimationTimeIndex;
    class IAnimationTrack;
    class IAnimationVertexTrack;
    class IAnimator;
    class ITrackElement;
    class ITrack;

    // ai
    class IAi;
    class IAiCompositeGoal;
    class IAiGoal;
    class IAiGoalEvaluator;
    class IAiManager;
    class IAiSteering3;
    class IAiTargeting3;
    class IPathfinder2;
    class IPathNode2;
    class IAiWorld3;
    class IVehicle;
    class IVehicleAiManager;
    class ITrack;

    // audio
    class IAudioEffect;
    class IAudioProcessor;
    class IAudioEffectVolume;
    class IAudioEffectDelay;
    class IAudioBusBuffers;
    class IAudioProcessData;

    // command
    class ICommand;
    class ICommandManager;
    class ICommandManagerListener;
    class IMouseCommand;

    // core
    class IEvent;
    class ILogManager;

    // database
    class IDatabase;
    class IDatabaseQuery;
    class IDatabaseManager;

    // finite state machine
    class IFSM;
    class IFSMListener;
    class IFSMManager;

    // graphics
    namespace render
    {
        class IAnimationState;
        class IAnimationController;
        class IAnimationControllerListener;
        class IAnimationStateController;
        class IAnimationTextureControl;
        class IBillboard;
        class IBillboardSet;
        class ICamera;
        class ICubemap;
        class IDebug;
        class IDebugLine;
        class IDecalCursor;
        class IDeferredShadingSystem;
        class IDepthBuffer;
        class IDynamicLines;
        class IDynamicMesh;
        class IFont;
        class IFontManager;
        class IFrustum;
        class IGraphicsSystem;
        class IGraphicsMesh;
        class IGraphicsSubMesh;
        class IGraphicsObject;
        class IGraphicsState;
        class IGraphicsSettings;
        class IHardwareBuffer;
        class IHardwareIndexBuffer;
        class IHardwareVertexBuffer;
        class IInstancedObject;
        class IInstanceManager;
        class ILight;
        class IMaterial;
        class IMaterialNode;
        class IMaterialManager;
        class IMaterialNodePasses;
        class IMeshConverter;
        class IOverlay;
        class IOverlayElement;
        class IOverlayElementContainer;
        class IOverlayElementText;
        class IOverlayElementVector;
        class IOverlayManager;
        class IMaterialPass;
        class IMaterialTechnique;
        class IMaterialShader;
        class IMaterialEvent;
        class IMaterialNodeAnimatedTexture;
        class IRenderTarget;
        class IRenderTask;
        class ISceneNode;
        class IGraphicsScene;
        class IScreenSpaceEffect;
        class IScreenSpaceEffectRenderer;
        class ISkybox;
        class ISprite;
        class ISpriteRenderer;

        class ITerrain;
        class ITerrainBlendMap;
        class ITerrainRayResult;
        class ITexture;
        class ITextureManager;
        class IMaterialTexture;
        class IViewport;
        class IWater;
        class IWindow;
        class IWindowEvent;
        class IWindowListener;

        // particle
        class IParticleAffector;
        class IParticleEmitter;
        class IParticle;
        class IParticleManager;
        class IParticleNode;
        class IParticleRenderer;
        class IParticleSystemBuilder;
        class IParticleSystem;
        class IParticleSystemListener;
        class IParticleTechnique;
    }  // namespace render

    // ik
    class IIKController;
    class IIKGoal;
    class IIKJoint;
    class IIKManager;

    // math
    template <class T>
    class Vector2;

    template <class T>
    class Vector3;

    template <class T>
    class Transform3;

    // mesh
    class IIndexBuffer;
    class IMesh;
    class IMeshLoader;
    class IMeshResource;
    class ISubMesh;
    class IVertexBuffer;
    class IVertexDeclaration;
    class IVertexElement;

    // messages
    class StatePhysicsForce2;
    class StatePhysicsVelocity2;
    class StatePhysicsDynamicState2;
    class StateMessageLoad;
    class StateMessageStringValue;
    class StateMessageSetTexture;
    class StateMessageFragmentParam;
    class StateMessageMaterialName;
    class StateMessagePlay;
    class StateMessageStop;
    class StateMessageVisible;
    class StateMessageIntValue;
    class StateMessageUIntValue;
    class StateMessageAnimationEnable;
    class StateMessageDirty;
    class StateMessageBlendMapValue;
    class StateMessageVector3;

    // physics
    namespace physics
    {
        class IPhysicsShape2;
        class IPhysicsShape3;
        class IBoxShape2;
        class IBoxShape3;
        class ICharacterController2;
        class ICharacterController3;
        class IPhysicsBody2D;
        class IPhysicsBody3;
        class IRigidBody2;
        class IRigidBody3;
        class IRigidDynamic3;
        class IRigidStatic3;
        class IPhysicsManager2D;
        class IPhysicsManager;
        class IPhysicsMaterial2;
        class IPhysicsMaterial3;
        class IPhysicsParticle2;
        class IPhysicsParticle3;
        class IPlaneShape3;
        class ISphereShape2;
        class ISphereShape3;
        class ITerrainShape;
        class IPhysicsVehicle3;
        class IPhysicsVehicleInput3;
        class IPhysicsVehicleWheel3;
        class IPhysicsScene2;
        class IPhysicsScene3;
        class IPhysicsEffect2;
        class IPhysicsBodyEffectSnap2;
        class IRaycastHit;
        class IPhysicsConstraint2;
        class IPhysicsConstraint3;
        class IConstraintD6;
        class IConstraintDrive;
        class IConstraintFixed3;
        class IPhysicsCooking;
        class IPhysicsSpring;
        class IConstraintLimit;
        class IConstraintLinearLimit;
        class IPhysicsSoftBody2;
        class IPhysicsSoftBody3;
        class IMeshShape;
    }  // namespace physics

    // procedural
    namespace procedural
    {
        class IProceduralCollision;
        class ILot;
        class IProceduralEngine;
        class IProceduralManager;
        class ICityGenerator;
        class IProceduralInput;
        class IProceduralOutput;
        class ITerrainGenerator;
        class IRoadNetwork;
        class IProceduralCity;
        class IProceduralCityCenter;
        class IMeshGenerator;
        class IRoadGenerator;
        class IRoad;
        class IRoadElement;
        class IRoadSection;
        class IRoadMeshElement;
        class IRoadConnection;
        class IRoadConnectionData;
        class IRoadNode;
        class ICityCenter;
        class ICityMap;
        class ICityBlock;
        class IProceduralTerrain;
        class IProceduralScene;
        class IProceduralWorld;
        class IBlockGenerator;
        class ISidewalk;
        class IProceduralNode;
        class ITerrainGenerator;
    }  // namespace procedural

    // ui
    namespace ui
    {
        class IUIAbout;
        class IUIApplication;
        class IUIButton;
        class IUICheckbox;
        class IUICollapsingHeader;
        class IUIColourPicker;
        class IUIContainer;
        class IUICursor;
        class IUIDragSource;
        class IUIDropdown;
        class IUIDropTarget;
        class IUIElement;
        class IUIEventWindow;
        class IUIFileBrowser;
        class IUIFrame;
        class IUIGrid;
        class IUIImage;
        class IUIImageArray;
        class IUIInputManager;
        class IUILabelCheckboxPair;
        class IUILabelDropdownPair;
        class IUILabelSliderPair;
        class IUILabelTextInputPair;
        class IUILayout;
        class IUIManager;
        class IUIMenu;
        class IUIMenubar;
        class IUIMenuItem;
        class IUIPropertyGrid;
        class IUIProfilerWindow;
        class IUIProfileWindow;
        class IUIRenderWindow;
        class IUIScrollingText;
        class IUISpinner;
        class IUITabBar;
        class IUITabItem;
        class IUIText;
        class IUITextEntry;
        class IUITerrainEditor;
        class IUIToggleButton;
        class IUIToggleGroup;
        class IUIToolbar;
        class IUITreeCtrl;
        class IUITreeNode;
        class IUIVector2;
        class IUIVector3;
        class IUIVector4;
        class IUIWindow;
    }  // namespace ui

    // video
    class IVideoManager;
    class IVideo;
    class IVideoStream;
    class IVideoTexture;

    // input
    class IChordDetector;
    class IGameInput;
    class IGameInputMap;
    class IGameInputState;
    class IInputAction;
    class IInputConverter;
    class IInputEvent;
    class IInputDeviceManager;
    class IJoystickState;
    class IKeyboardState;
    class IMouseState;
    class ISequenceDetector;
    class ITapDetector;

    // io
    class IArchive;
    class IStream;
    class IFileList;
    class IFileListener;
    class IFileSystem;
    class IFolderExplorer;
    class IFolderExplorerW;
    class INativeFileDialog;

    // mesh
    class Mesh;
    class SubMesh;
    class VertexBuffer;
    class VertexDeclaration;
    class IndexBuffer;
    class CollisionMesh;
    class CollisionSubMesh;

    // net
    class INetworkListener;
    class INetworkManager;
    class INetworkView;
    class INetworkStream;
    class IPacket;
    class ISystemAddress;

    // resource
    class IMeshManager;
    class IResource;
    class IResourceManager;
    class IResourceDatabase;
    class IResourceGroupManager;

    // vehicle
    class IVehicleController;
    class IVehicleComponent;
    class IDifferential;
    class IVehiclePowerUnit;
    class IGearBox;
    class IWheelController;
    class IBatteryPack;
    class IESController;
    class IVehicleBody;
    class ITruckController;

    // script
    class IScript;
    class IScriptClass;
    class IScriptBreakpoint;
    class IScriptData;
    class IScriptEvent;
    class IScriptFunction;
    class IScriptInvoker;
    class IScriptManager;
    class IScriptReceiver;
    class IScriptObject;
    class IScriptVariable;

    // sound
    class ISound;
    class ISound2;
    class ISound3;
    class ISoundListener3;
    class ISoundManager;
    class ISoundPlayer;
    class ISoundEventGroup;
    class ISoundEvent;
    class ISoundEventParam;
    class ISoundProject;

    // system
    class IAsyncOperation;
    class IEvent;
    class IEventListener;
    class IFrameStatistics;
    class IFrameGrabber;
    class IFactory;
    class IFactoryManager;
    class IJob;
    class IJobGroup;
    class IJobQueue;
    class ILibrary;
    class IOutputManager;
    class IPlugin;
    class IPluginManager;
    class IProfile;
    class IProfiler;
    class IProject;
    class IProcessManager;
    class ITransformNode;
    class ISelectionManager;
    class IStateContext;
    class ITransformRoot;
    class ITransformManager;
    class IState;
    class IStateListener;
    class IStateContext;
    class IStateQuery;
    class IStateQueue;
    class IStateManager;
    class IStateMessage;
    class IStateManager;
    class IScheduler;
    class ISystemManager;
    class ISystemManagerListener;
    class ITask;
    class ITaskLock;
    class ITaskManager;
    class IThreadPool;
    class ITimer;
    class IWorkerThread;

    // test
    class ITest;
    class ITestManager;

    // mesh
    class IIndexBuffer;
    class IMesh;
    class ISubMesh;
    class IVertexBuffer;
    class IVertexDeclaration;
    class IVertexElement;

    // vehicle
    class ICarController;
    class IVehicleController;
    class IVehicleManager;
    class IVehicleCallback;

    static const u8 GC_FLAG_RESERVED = ( 1 << 0 );
    static const u8 GC_FLAG_GARBAGE_COLLECTED = ( 1 << 1 );
    static const u8 GC_FLAG_ENABLE_REFERENCE_TRACKING = ( 1 << 2 );
    static const u8 GC_FLAG_OBJECT_ALIVE = ( 1 << 3 );
    static const u8 GC_FLAG_OBJECT_CONSTRUCTED = ( 1 << 4 );
    static const u8 GC_FLAG_POOL_ELEMENT = ( 1 << 5 );

}  // end namespace fb

namespace fb
{
    class PoolDataTBB;
    class PoolDataStandard;
    class Plot;
    class StateQueueStandard;
    class StateContextTBB;
    class StateQueueTBB;
    class TaskDataStandard;
    class Task;
    class TaskManager;
}  // end namespace fb

namespace fb
{
    class MLP;

    class StateMessageBuffer;
    class StateMessageJobStatus;
    class StateMessageProperties;
    class StateMessageObject;
    class StateMessageObjectsArray;
    class StateMessageContact2;
    class StateMessageType;
    class StateMessageUIntValue;
    class StateMessageFloatValue;
    class StateMessageVector3;
    class StateMessageVector4;

    class BaseState;
    class BoxShapeState;
    class GraphicsMeshState;
    class GraphicsObjectState;

    namespace render
    {
        class GraphicsSystemState;
    }

    class LightState;
    class MaterialPassState;
    class MeshShapeState;
    class OverlayState;
    class OverlayTextState;
    class RenderTextureState;
    class RenderWindowState;
    class RigidbodyState;
    class SceneManagerState;
    class SceneNodeState;
    class SkyboxState;
    class TerrainState;
    class TextureState;
    class TransformState;
    class UIElementState;
    class ViewportState;
    class TransformStateData;
}  // end namespace fb

namespace fb
{
    namespace scene
    {
        class AudioClip;
        class AudioSource;

        class Camera;

        class FiniteStateMachine;

        class MeshRenderer;
        class Mesh;

        class Constraint;
        class Rigidbody;

        class Collision;
        class CollisionMesh;
        class CollisionBox;
        class Material;
        class Skybox;
        class Shader;
        class Tooltip;
        class Transform;
        class WheelController;

        // terrain
        class TerrainBlendMap;
        class TerrainSystem;
        class TerrainData;
        class TerrainLayer;

        // ui
        class Button;
        class Image;
        class InputField;
        class Slider;
        class Text;
        class Toggle;
        class Sprite;
        class Layout;
        class LayoutTransform;
        class Image;
        class Text;
        class UIComponent;

        class ISystem;

    }  // namespace scene
}  // namespace fb

#endif  // FBPrerequisites_h__
