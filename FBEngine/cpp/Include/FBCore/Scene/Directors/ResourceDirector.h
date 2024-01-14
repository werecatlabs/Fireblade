#ifndef ResourceDirector_h__
#define ResourceDirector_h__

#include <FBCore/Scene/Director.h>

namespace fb
{
    namespace scene
    {
        /** Mesh resource director implementation. */
        class ResourceDirector : public Director
        {
        public:
            /** Constructor. */
            ResourceDirector();

            /** Destructor. */
            ~ResourceDirector() override;

            /** @copydoc IDirector::getProperties */
            SmartPtr<Properties> getProperties() const;

            /** @copydoc IDirector::setProperties */
            void setProperties( SmartPtr<Properties> properties );

            void import() override;

            String getResourcePath() const;

            void setResourcePath( const String &resourcePath );

            String getResourceUUID() const;

            void setResourceUUID( const String &resourceUUID );

            FB_CLASS_REGISTER_DECL;

        protected:
            String m_resourcePath;
            String m_resourceUUID;
        };
    }  // namespace scene
}  // namespace fb

#endif  // ResourceDirector_h__
