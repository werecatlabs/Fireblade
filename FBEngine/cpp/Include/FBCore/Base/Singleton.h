#ifndef _FB_SINGLETON_H
#define _FB_SINGLETON_H

#include <FBCore/FBCoreTypes.h>

namespace fb
{

    /** */
    template <class T>
    class Singleton
    {
    public:
        /** */
        Singleton()
        {
            m_singleton = static_cast<T *>( this );
        }

        /** */
        ~Singleton()
        {
            m_singleton = 0;
        }

        /** */
        static T &getSingleton()
        {
            return *m_singleton;
        }

        /** */
        static T *getSingletonPtr()
        {
            return m_singleton;
        }

    protected:
        /// A pointer to the singleton
        static T *m_singleton;
    };
}  // end namespace fb

#endif
