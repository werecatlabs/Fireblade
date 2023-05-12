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

namespace fb
{

    class IObject;
    class ISharedObject;
    class ISharedObjectListener;
    class FactoryManager;
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

    namespace data
    {
        struct base_data;

        struct attribute;
        struct texture_map;
        struct material;
        struct material_data;

        struct material_node;
        struct material_texture;
        struct material_pass;
        struct material_scheme;
        struct material_graph;

        struct vec4;
        struct property;
        struct properties;
        struct vertex;
        struct sub_mesh;
        struct mesh;
        struct transform_data;
        struct debug_draw;
    }  // end namespace data

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

    // combat
    class CombatBodyListener2;
    class CombatBody2;
    class CombatHitData2;

    // database
    class IDatabase;
    class IDatabaseQuery;
    class IDatabaseManager;

    // flash
    class IFlashPlayer;
    class IFlashControl;
    class IFlashControlManager;

    // finite state machine
    class IFSM;
    class IFSMListener;
    class IFSMManager;

    // game
    class IAnchorPoint;
    class IAnchorPoint2;
    class IAnchorPoint3;

    class IQuery;
    class IQueryManager;
    class IProject;
    class IConfigurationListener;
    class IConfigurationProperty;
    class IConfigurationManager;

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

    // job
    class IJob;
    class IJobListener;
    class IJobQueueListener;

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
    class IInputActionData;
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
    class IResourceListener;
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
    class IScriptBind;
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
    class GraphicsSystemState;
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
    // json data structures
    namespace data
    {
        struct render_settings_data;
        struct project_data;

        struct importer_data;
        struct drag_drop_data;
        struct mesh_importer_data;

        struct resolution_entry;
        struct fsaa_entry;
        struct resolution_data;
        struct render_system_config;

        struct tree_prototype_data;
        struct tree_instance_data;

        struct field;
        struct rows;
        struct query;

        struct PluginEvent;

        struct road;
        struct road_node;
        struct road_element;
        struct road_section;
        struct road_connection;
        struct road_connection_data;
        struct road_network;
        struct city_data;
        struct terrain_data;
        struct procedural_scene;
        struct procedural_world;

        struct block_node;
        struct block_data;

        struct osm;
        struct bounds_data;
        struct node_data;
        struct tag_data;
        struct nd_data;
        struct way_data;
        struct relation;
        struct member;

        struct actor_data;
        struct director_data;
        struct component_eventhandler_data;
        struct sub_component_data;
        struct component_data;
        struct fb_scene;

        struct json_component;
        struct json_actor;
        struct json_scene;

        struct renderer_data;
        struct mesh_renderer_data;
    }  // namespace data

    // aero data structures
    namespace data
    {

        struct esc_data;
        struct aircraft_input_data;
        struct aircraft_curve_value;
        struct aircraft_curve_data;
        struct aircraft_aerofoil_data;
        struct aircraft_cog_data;
        struct aircraft_control_surface_data;
        struct aircraft_wing_data;
        struct aircraft_engine_data;
        struct aircraft_ground_effect_data;
        struct aircraft_prop_wash_data;
        struct aircraft_prop_wash_data;
        struct aircraft_data;
        struct aircraft_wheel_data;
    }  // namespace data

    // json data structures
    namespace data
    {
        struct prefab_data;

        struct s_component_data;
        struct directorylisting;
        struct dialogstate;
        struct s_flight_recording_file;

        struct vec4;
        struct dialog_data;
        struct model_component_data;

        struct file_data;

        struct AttributeData;
        struct PropertyData;
        struct PropertiesData;

        struct fsm_data;
        struct camera_data;
        struct plugin_event;

        struct model_load_data;
        struct model_data;

        struct vertex;
        struct sub_mesh;
        struct mesh;

        struct collision_object;

        struct model_wind;
        struct collider_data;
        struct gameobject_component_data;
        struct prefab_data;
        struct tree_prototype_data;
        struct tree_instance_data;
        struct terrain_data;

        struct resolution_entry;
        struct resolution_data;
        struct system_settings_data;
        struct glow_rope_data;
        struct rotor_data;
        struct transform_data;

        struct sphere_collider;
        struct plane_collider;
        struct box_collider;
        struct mesh_collider;
        struct terrain_collider;
        struct compond_collider;
        struct collider;
        struct physics_material;
        struct bounds_data;
        struct constraint_data;
        struct rigid_body;
        struct layer_data;
        struct model_layer_data;

        struct buoyancy_data;
        struct actor_data;
        struct component_event_data;
        struct actor_event_data;
        struct debug_draw;
        struct debug_text_data;
        struct object_select_data;
        struct object_bounds_data;

        struct pid_data;
        struct pid_scaler_data;
        struct stab_settings_data;
        struct drone_stab_settings;
        struct multi_rotor_ctrl_data;
        struct water_ripples;

        struct scene_node_data;
        struct path_data;

        struct model_physics_setup_flight_data;
        struct model_physics_setup_data;
        struct models_physics_setup_data;

        struct configured_model_component_data;
        struct configured_model_data;

        struct switch_indicators_data;
        struct setup_wizard_data;
        struct four_wheeled_vehicle_data;
        struct cg_info_mode;

        struct aircraft_input_data;
        struct aircraft_curve_value;
        struct aircraft_curve_data;
        struct aircraft_aerofoil_data;
        struct aircraft_cog_data;
        struct aircraft_control_surface_data;
        struct aircraft_wing_data;
        struct aircraft_engine_data;
        struct aircraft_ground_effect_data;
        struct aircraft_prop_wash_data;
        struct aircraft_prop_wash_data;
        struct aircraft_data;
        struct car_data;
        struct truck_data;
        struct aircraft_wheel_data;

        struct esc_data;

        struct CMapData;

        struct plugin_event;
        struct plugin_events;

        struct servo_hole_data;
        struct servo_data;
        struct swash_plate_data;
        struct model_control_data;

        struct multi_rotor_prop_data;
        struct multi_rotor_motor_data;
        struct collider;

        struct directorylisting;
        struct resource_map_entry;
        struct resource_map;

        struct movable_object_data;

        struct four_wheeled_vehicle_wheel_data;
        struct swash_plate_position;

        struct system_settings_data;
        struct scenery_data;
        struct model_data;
        struct car_data;

        struct vehicle_data;
        struct vehicle_wheel_data;

        typedef std::string String;

    }  // namespace data
}  // namespace fb

namespace fb
{
    namespace scene
    {
        struct ActorTransform;
        class Transform;

        class StartLocation;

        struct EntityCellData;

        class TransformAvatar;

        class TextAsset;
        class StringReader;
        class Shader;
        class RaceView;
        class RaceManagerView;
        class RaceRewards;
        class Statistics;

        class Drone_Controller;
        class Motorbike_Controller;

        class Texture;
        class UpgradeLevels;
        class PlayerControl;
        class WheelHit;
        class Skidmark;

        class TerrainData;
        class TerrainLayer;

        class SplatPrototype;
        class Ranker;
        class ProgressTracker;
        class SoundManager;
        class WaypointList;

        // racing
        class RacingCarController;

        // audio
        class AudioClip;
        class AudioSource;

        class Camera;
        class CameraController;
        class FlightInfo;

        class FiniteStateMachine;

        class ParticleGroundEffect;
        class ProceduralObject;
        class ProceduralRenderable;
        class ProceduralScene;
        class ProceduralSceneRenderer;
        class ProceduralRenderer;
        class CityGenerator;
        class ProceduralCity;
        class Building;
        class Road;
        class RoadNode;
        class Sidewalk;
        class SidewalkNode;
        class RoadConnection;
        class Block;
        class BlockNode;
        class LotNode;
        class Lot;
        class LSystem;
        class LSystemTurtle;
        class ProceduralPath;
        class River;
        class RiverNode;
        class ProceduralCell;
        class EditorFpsCameraController;
        class SphericalCameraController;
        class FragCounter;
        class MatchLogic;
        class MovementControlFPS;
        class Player;
        class CollisionNode;
        class PhysicsResponse2;
        class Position25;

        class Destructible;
        class Collision;
        class CollisionMesh;
        class Destructible;
        class EnergyContainer;
        class PuzzleMatchElement;
        class PuzzleMatchManager;
        class BasicPhysicsObject;
        class MovementControl2;
        class Grid2Container;
        class WeaponContainer;
        class PuzzleMatchWorld;
        class GravitonLogic;
        class ObjectContainer;
        class PropertiesComponent;
        class EntitySelection2;
        class ScreenSpaceRotation;
        class SoundContainer;
        class Tooltip;
        class WheelController;
        class MeshRenderer;
        class Mesh;
        class TerrainBlendMap;
        class TerrainSystem;
        class VehicleController;

        class Constraint;
        class Rigidbody;

        class Collision;
        class CollisionBox;

        class Material;
        class VehicleComponent;
        class VehicleConstraint;
        class VehicleRigidbody;
        class Receiver;
        class ControlSurface;
        class VehiclePhysics;
        class Engine;
        class Servo;
        class Rotor;
        class MultiRotorCtrl;
        class DroneController;
        class FlybarCtrl;
        class Servo;
        class Rotor;
        class MultiRotorMotor;
        class MultiRotorProp;
        class Swashplate;
        class Muffler;

        class Weapon;
        class WeaponFireArm2;
        class WeaponFireArm3;

        // ui
        class Button;
        class Image;
        class InputField;
        class Slider;
        class Text;
        class Toggle;
        class Sprite;
        class Layout;
        class CanvasTransform;
        class Image;
        class Text;
        class UIComponent;

        class Skybox;
        class Application;

        class GraphicsSettings;
        class PhysicsSettings;
        class SimulatorApplication;
        class TruckSimulatorApplication;
        class SystemSettings;
        class VideoSettings;

        class Vehicle;
        class VehicleCar;
        class VehicleRadioControlled;
    }  // namespace scene
}  // namespace fb

#endif  // FBPrerequisites_h__
