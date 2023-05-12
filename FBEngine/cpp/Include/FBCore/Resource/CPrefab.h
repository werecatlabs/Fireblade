#ifndef __CPrefab_h__
#define __CPrefab_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Scene/IPrefab.h>
#include <FBCore/Memory/CSharedObject.h>
#include <FBCore/Resource/CResource.h>

namespace fb
{
    namespace scene
    {

        class CPrefab : public CResource<IPrefab>
        {
        public:
            CPrefab() = default;
            ~CPrefab() override;

            /** @copydoc IObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            SmartPtr<scene::IActor> createActor() override;

            SmartPtr<scene::IActor> getActor() const override;
            void setActor( SmartPtr<scene::IActor> actor ) override;

            void save() override;

            hash64 getFileSystemId() const override;

            void setFileSystemId( hash64 id ) override;

            SmartPtr<Properties> getProperties() const override;

            void setProperties( SmartPtr<Properties> properties ) override;

            SmartPtr<IStateContext> getStateObject() const override;

            void setStateObject( SmartPtr<IStateContext> stateObject ) override;

            void _getObject( void **ppObject ) const override;

            SmartPtr<IData> getData() const;
            void setData( SmartPtr<IData> data );

            FB_CLASS_REGISTER_DECL;

        protected:
            hash64 m_fileId = 0;
            WeakPtr<IActor> m_actor;
            SmartPtr<IData> m_data;
            SmartPtr<IStateContext> m_stateObject;
        };

    }  // namespace scene
}  // end namespace fb

#endif  // IResource_h__
