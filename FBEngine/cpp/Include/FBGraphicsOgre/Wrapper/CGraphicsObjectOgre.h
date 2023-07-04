#ifndef CGraphicsObjectOgre_h__
#define CGraphicsObjectOgre_h__

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBGraphics/Wrapper/CGraphicsObject.h>
#include <FBCore/Interface/Graphics/IGraphicsObject.h>
#include <FBCore/Memory/SharedObject.h>
#include <FBCore/Core/BitUtil.h>
#include <FBCore/Core/Handle.h>
#include <FBCore/Core/Properties.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/Interface/System/IEvent.h>
#include <FBCore/Interface/Graphics/ISceneNode.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBCore/Interface/System/IStateContext.h>
#include <FBCore/State/States/GraphicsObjectState.h>
#include <OgreMovableObject.h>

namespace fb
{
    namespace render
    {
        template <class T>
        class CGraphicsObjectOgre : public CGraphicsObject<T>
        {
        public:
            CGraphicsObjectOgre();
            ~CGraphicsObjectOgre() override;

            void unload(SmartPtr<ISharedObject> data)
            {
                CGraphicsObject<T>::unload( data );
            }

            /** @copydoc IGraphicsObject::setMaterialName */
            virtual void setMaterialName( const String &materialName, s32 index = -1 );

            /** @copydoc IGraphicsObject::getMaterialName */
            virtual String getMaterialName( s32 index = -1 ) const;

            /** @copydoc IGraphicsObject::clone */
            virtual SmartPtr<IGraphicsObject> clone(
                const String &name = StringUtil::EmptyString ) const override;

            /** @copydoc IGraphicsObject::_getObject */
            virtual void _getObject( void **ppObject ) const override;

            /** @copydoc IGraphicsObject::attachToParent */
            virtual void attachToParent( SmartPtr<ISceneNode> parent ) override;

            /** @copydoc IGraphicsObject::detachFromParent */
            virtual void detachFromParent( SmartPtr<ISceneNode> parent ) override;

            /** @copydoc IGraphicsObject::getProperties */
            virtual SmartPtr<Properties> getProperties() const;

            /** @copydoc IGraphicsObject::setProperties */
            virtual void setProperties( SmartPtr<Properties> properties );

            SmartPtr<IStateContext> getStateObject() const;
            void setStateObject( SmartPtr<IStateContext> stateObject );

            SmartPtr<IGraphicsScene> getCreator() const;
            void setCreator( SmartPtr<IGraphicsScene> creator );

            Ogre::MovableObject *getMovable() const;

            void setMovable( Ogre::MovableObject *movable );

            FB_CLASS_REGISTER_TEMPLATE_DECL( CGraphicsObjectOgre, T );

        protected:
            class StateListener : public SharedObject<IStateListener>
            {
            public:
                StateListener() = default;
                ~StateListener() override = default;

                void handleStateChanged( const SmartPtr<IStateMessage> &message ) override;
                void handleStateChanged( SmartPtr<IState> &state ) override;
                void handleQuery( SmartPtr<IStateQuery> &query ) override;

                virtual CGraphicsObjectOgre<T> *getOwner() const;

                virtual void setOwner( CGraphicsObjectOgre<T> *owner );

                CGraphicsObjectOgre<T> *m_owner = nullptr;
            };

            Ogre::MovableObject *m_movable = nullptr;
        };

        FB_CLASS_REGISTER_DERIVED_TEMPLATE( fb, CGraphicsObjectOgre, T, CGraphicsObject<T> );

    }  // end namespace render
}  // end namespace fb

#include <FBGraphicsOgre/Wrapper/CGraphicsObjectOgre.inl>

#endif  // CGraphicsObject_h__
