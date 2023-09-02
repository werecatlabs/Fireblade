#ifndef ApplicationUtil_h__
#define ApplicationUtil_h__

#include <FBCore/Core/StringUtil.h>
#include <FBCore/Interface/IApplicationManager.h>
#include <FBCore/Interface/System/IFactoryManager.h>
#include <FBCore/Interface/Resource/IResource.h>
#include <FBCore/Interface/UI/IUIMenuItem.h>

namespace fb
{

    class ApplicationUtil
    {
    public:
        static SmartPtr<scene::IActor> loadMesh( const String &filePath );

        static SmartPtr<ui::IUIMenuItem> addMenuItem(
            SmartPtr<ui::IUIMenu> menu, s32 itemid, const String &text, const String &help,
            ui::IUIMenuItem::Type type = ui::IUIMenuItem::Type::Normal );

        static SmartPtr<ui::IUIMenuItem> addMenuSeparator( SmartPtr<ui::IUIMenu> menu );

        static SmartPtr<ui::IUIElement> setText( SmartPtr<ui::IUITreeNode> node, const String &text );
        static SmartPtr<ui::IUIElement> setImage( SmartPtr<ui::IUITreeNode> node,
                                                  const String &imagePath );

        static SmartPtr<ui::IUIElement> getFirstChild( SmartPtr<ui::IUIElement> element );
        static String getText( SmartPtr<ui::IUITreeNode> node );

        static String getComponentFactoryType( const String &factoryType );

        static void convertCSharp( const String &srcPath, const String &dstPath );

        static u32 getEventPriority( SmartPtr<scene::IComponent> component );

        static void showAllOverlays();
        static void hideAllOverlays();

        //static void addActor( data::actor_data &parent, const data::json_actor &a );

        static SmartPtr<IData> importJsonScene( const String &filePath );
        static SmartPtr<IData> importScene( const String &filePath );

        static SmartPtr<scene::IActor> createOverlayPanelTest();
        static SmartPtr<scene::IActor> createOverlayTextTest();
        static SmartPtr<scene::IActor> createOverlayButtonTest();

        static void createRigidStaticMesh( SmartPtr<scene::IActor> actor, bool recursive );
        static void createRigidDynamicMesh( SmartPtr<scene::IActor> actor, bool recursive );

	    static SmartPtr<scene::IActor> createDefaultCubemap( bool addToScene = true );
        static SmartPtr<scene::IActor> createDefaultSky( bool addToScene = true );

        static SmartPtr<scene::IActor> createCamera( bool addToScene = true );

        static SmartPtr<scene::IActor> createDefaultCube( bool addToScene = true );
        static SmartPtr<scene::IActor> createDefaultCubeMesh( bool addToScene = true );

        static SmartPtr<scene::IActor> createDefaultGround(bool addToScene = true);

        static SmartPtr<scene::IActor> createDefaultTerrain( bool addToScene = true );

        static SmartPtr<scene::IActor> createDefaultConstraint();

        static SmartPtr<scene::IActor> createDirectionalLight( bool addToScene = true );
        static SmartPtr<scene::IActor> createPointLight( bool addToScene = true );

        static SmartPtr<scene::IActor> createDefaultPlane(bool addToScene = true);

        static SmartPtr<scene::IActor> createDefaultVehicle(bool addToScene = true);
        static SmartPtr<scene::IActor> createDefaultCar(bool addToScene = true);
        static SmartPtr<scene::IActor> createDefaultTruck(bool addToScene = true);

        static SmartPtr<scene::IActor> createDefaultParticleSystem( bool addToScene = true );

        static SmartPtr<IData> loadVehicleDataFromDatabase( s32 id );

        static SmartPtr<render::IMaterial> createDefaultMaterialUI();
        static SmartPtr<render::IMaterial> createDefaultMaterial();

        static SmartPtr<scene::IActor> createProceduralTest();

        static SmartPtr<scene::IActor> loadVehicleFromDB( s32 id );

        static void createRigidStaticMesh();
        static void createRigidDynamicMesh();

        static void createFactories();

        static void createDefaultMaterials();
        static void createDefaultFont();

        /** Gets the resource type. */
        static IResource::ResourceType getResourceType( const String &resourceTypeName );

        /** Gets the resource type name. */
        static String getResourceTypeByName( IResource::ResourceType resourceType );
    };
}  // end namespace fb

#endif  // ApplicationUtil_h__
