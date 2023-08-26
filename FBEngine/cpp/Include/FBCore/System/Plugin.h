#ifndef __Plugin_h__
#define __Plugin_h__

#include <FBCore/Interface/System/IPlugin.h>


namespace fb
{
    namespace core
    {
        
        class Plugin : public IPlugin
        {
        public:
            Plugin() = default;
            ~Plugin() override;

            /** @copydoc ISharedObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::reload */
            void reload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IPlugin::getLibraryHandle */
            LibraryHandle getLibraryHandle() const override;

            /** @copydoc IPlugin::setLibraryHandle */
            void setLibraryHandle( LibraryHandle handle ) override;

            /** @copydoc IPlugin::getFunction */
            LibraryFunction getFunction( const String &name ) const override;

            /** @copydoc IPlugin::getFilePath */
            StringW getFilePath() const override;

            /** @copydoc IPlugin::setFilePath */
            void setFilePath( const StringW &fileName ) override;

        protected:
            StringW m_filename;
            LibraryHandle m_hMod = nullptr;
        };

    }  // namespace core
}  // namespace fb

#endif  // Plugin_h__
