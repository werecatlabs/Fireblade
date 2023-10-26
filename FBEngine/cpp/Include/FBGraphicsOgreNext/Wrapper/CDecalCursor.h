#ifndef CDecalCursor_h__
#define CDecalCursor_h__

#include <FBGraphicsOgreNext/FBGraphicsOgreNextPrerequisites.h>
#include <FBCore/Interface/Graphics/IDecalCursor.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    namespace render
    {

        //--------------------------------------------
        class CDecalCursor : public IDecalCursor
        {
        public:
            CDecalCursor();
            ~CDecalCursor();

            void initialise( SmartPtr<render::IGraphicsScene> sceneMgr, const String &terrainMaterial,
                             const String &decalTextureName, const Vector2F &size );

            bool isVisible() const;
            void setVisible( bool visible );

            Vector3F getPosition() const;
            void setPosition( const Vector3F &position );

            Vector2F getSize() const;
            void setSize( const Vector2F &size );

            String getTextureName() const;
            void setTextureName( const String &val );

            void addDebugEntity( const String &entityName, const Vector3F &scale = Vector3F::unit() );
            void removeDebugEntity();

            SmartPtr<IStateContext> &getStateObject();
            const SmartPtr<IStateContext> &getStateObject() const;
            void setStateObject( SmartPtr<IStateContext> stateObject );

        protected:
            class CDecalCursorStateListener : public IStateListener
            {
            public:
                CDecalCursorStateListener( CDecalCursor *owner );
                ~CDecalCursorStateListener();

                void handleStateChanged( const SmartPtr<IStateMessage> &message );
                void handleStateChanged( SmartPtr<IState> &state );
                void handleQuery( SmartPtr<IStateQuery> &query );

            protected:
                CDecalCursor *m_owner = nullptr;
            };

            SmartPtr<IStateContext> m_stateObject;
            SmartPtr<IStateListener> m_stateListener;

            SmartPtr<IGraphicsMesh> m_entity;
            SmartPtr<ISceneNode> m_node;
            SmartPtr<render::IGraphicsScene> m_sceneMgr;
            DecalCursor *m_decalCursor = nullptr;
            Vector3F m_position;
            Vector2F m_size;
            bool m_isVisible;
            String m_textureName;
        };

    }  // end namespace render
}  // end namespace fb

#endif  // CDecalCursor_h__
