#ifndef _CCompositor_H_
#define _CCompositor_H_

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBCore/Memory/SharedObject.h>

namespace fb
{
    namespace render
    {
        class CCompositor : public SharedObject<ISharedObject>
        {
        public:
            CCompositor();
            ~CCompositor() override;

            /** */
            void setEnabled( bool isEnabled );

            /** */
            bool isEnabled() const;

        protected:
            bool m_isEnabled;
        };
    }  // end namespace render
}  // end namespace fb

#endif
