#ifndef CDecalCursor_h__
#define CDecalCursor_h__

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBCore/Interface/Graphics/IDecalCursor.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBCore/Memory/CSharedObject.h>

namespace fb
{
    namespace render
    {
        class CDecalCursor : public CSharedObject<IDecalCursor>
        {
        public:
            CDecalCursor();
            ~CDecalCursor() override;

            void initialise( SmartPtr<IGraphicsScene> sceneMgr, const String &terrainMaterial,
                             const String &decalTextureName, const Vector2F &size );

            bool isVisible() const override;
            void setVisible( bool visible ) override;

            Vector3F getPosition() const override;
            void setPosition( const Vector3F &position ) override;

            Vector2F getSize() const override;
            void setSize( const Vector2F &size ) override;

            String getTextureName() const;
            void setTextureName( const String &val );

            void addDebugEntity( const String &entityName,
                                 const Vector3F &scale = Vector3F::unit() ) override;
            void removeDebugEntity() override;

            SmartPtr<IStateContext> &getStateObject();
            const SmartPtr<IStateContext> &getStateObject() const;
            void setStateObject( SmartPtr<IStateContext> stateObject );

            FB_CLASS_REGISTER_DECL;

        protected:
            class CDecalCursorStateListener : public CSharedObject<IStateListener>
            {
            public:
                CDecalCursorStateListener( CDecalCursor *owner );
                ~CDecalCursorStateListener() override;

                void handleStateChanged( const SmartPtr<IStateMessage> &message ) override;
                void handleStateChanged( SmartPtr<IState> &state ) override;
                void handleQuery( SmartPtr<IStateQuery> &query ) override;

            protected:
                CDecalCursor *m_owner;
            };

            SmartPtr<IStateContext> m_stateObject;
            SmartPtr<IStateListener> m_stateListener;

            SmartPtr<IGraphicsMesh> m_entity;
            SmartPtr<ISceneNode> m_node;
            SmartPtr<IGraphicsScene> m_sceneMgr;
            DecalCursor *m_decalCursor;
            Vector3F m_position;
            Vector2F m_size;
            bool m_isVisible;
            String m_textureName;
        };

    }  // namespace render
}  // end namespace fb

#endif  // CDecalCursor_h__
