#ifndef _FB_SINGLETON_H
#define _FB_SINGLETON_H

#include <FBCore/FBCoreTypes.h>

namespace fb
{

    /** Singleton template class. */
    template <class T>
    class Singleton
    {
    public:
        /** Constructor. */
        Singleton();

        /** Destructor. */
        ~Singleton();

        /** Get the singleton instance as a reference. */
        static T &getSingleton();

        /** Get the singleton instance as a pointer. */
        static T *getSingletonPtr();

    protected:
        /// A pointer to the singleton
        static T *m_singleton;
    };

    template <class T>
    Singleton<T>::Singleton()
    {
        m_singleton = static_cast<T *>( this );
    }

    template <class T>
    Singleton<T>::~Singleton()
    {
        m_singleton = 0;
    }

    template <class T>
    T &Singleton<T>::getSingleton()
    {
        return *m_singleton;
    }

    template <class T>
    T *Singleton<T>::getSingletonPtr()
    {
        return m_singleton;
    }

}  // end namespace fb

#endif
