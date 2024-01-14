#ifndef __Prefab_h__
#define __Prefab_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Scene/IPrefab.h>
#include <FBCore/Resource/Resource.h>

namespace fb
{
    namespace scene
    {

        class Prefab : public Resource<IPrefab>
        {
        public:
            Prefab();
            ~Prefab() override;

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

            void _getObject( void **ppObject ) const override;

            SmartPtr<ISharedObject> getData() const;
            void setData( SmartPtr<ISharedObject> data );

            FB_CLASS_REGISTER_DECL;

        protected:
            hash64 m_fileId = 0;
            WeakPtr<IActor> m_actor;
            SmartPtr<ISharedObject> m_data;
        };

    }  // namespace scene
}  // end namespace fb

#endif  // IResource_h__
