#ifndef _FB_WeakPtr_H_
#define _FB_WeakPtr_H_

#include <FBCore/Base/Exception.h>
#include <FBCore/Memory/SmartPtr.h>
#include <type_traits>

namespace fb
{

    /**
     * @brief A weak pointer to a dynamically-allocated object of type T.
     *
     * This class allows access to an object of type T that is dynamically allocated and managed by
     * a smart pointer, but without owning the object. A weak pointer is useful when a dependent object
     * needs access to a target object but does not want to keep the target object alive. A weak pointer
     * can be safely converted to a smart pointer for temporary access to the target object. If the target
     * object is no longer available, the weak pointer can be checked using expired().
     *
     * @tparam T The type of the object pointed to.
     */
    template <class T>
    class FBWeakPtr
    {
    public:
        using this_type = FBWeakPtr<T>;  ///< This type.
        using element_type = T;          ///< The type of the pointed-to object.
        using value_type = T;            ///< The type of the pointed-to object.
        using pointer = T *;             ///< Pointer to the pointed-to object.

        /**
         * @brief Constructs a null weak pointer.
         */
        FBWeakPtr() = default;

        /**
         * @brief Constructs a weak pointer from a raw pointer to the object.
         *
         * @param object A raw pointer to the object.
         */
        FBWeakPtr( T *object );

        /** Constructor creates a smart pointer from the copy passed in.
         */
        FBWeakPtr( const FBWeakPtr &other );

        /**
         * @brief Constructs a weak pointer from another weak pointer.
         *
         * @param other Another weak pointer.
         */
        FBWeakPtr( const FBSmartPtr<T> &other );

        /**
         * @brief Constructs a weak pointer from a smart pointer.
         *
         * @param other A smart pointer.
         */
        FBWeakPtr( const RawPtr<T> &object );

        /**
         * @brief Constructs a weak pointer from a raw pointer to another object.
         *
         * @tparam B The type of the pointed-to object.
         * @param pointer A raw pointer to the other object.
         */
        template <class B>
        FBWeakPtr( B *pointer );

        /**
         * @brief Constructs a weak pointer from another weak pointer to another object.
         *
         * @tparam B The type of the pointed-to object.
         * @param object Another weak pointer to the other object.
         */
        template <class B>
        FBWeakPtr( const FBWeakPtr<B> &object );

        /**
         * @brief Constructs a weak pointer from a smart pointer to another object.
         *
         * @tparam B The type of the pointed-to object.
         * @param object A smart pointer to the other object.
         */
        template <class B>
        FBWeakPtr( const FBSmartPtr<B> &object );

        /**
         * @brief Non-virtual destructor.
         */
        ~FBWeakPtr();

        /**
         * @brief Returns the number of shared_ptr instances that share ownership of the managed object.
         *
         * @return The number of shared_ptr instances that share ownership of the managed object.
         */
        long use_count() const noexcept
        {
            const auto object = get();
            if( object )
            {
                return object->getWeakReferences();
            }

            return 0;
        }

        /**
         * @brief Checks whether the object pointed to by this weak pointer has expired.
         *
         * An object is expired if it has been deleted by its shared pointer or if its shared pointer has
         * been reset.
         *
         * @return `true` if the object has expired, `false` otherwise.
         */
        bool expired() const noexcept
        {
            return use_count() == -1;
        }

        /**
         * @brief Returns a shared pointer to the managed object, if it still exists.
         *
         * @return FBSmartPtr<T> A shared pointer to the managed object if it still exists,
         *                       or a null shared pointer if the managed object has been destroyed.
         */
        FBSmartPtr<T> lock() const noexcept
        {
            return expired() ? FBSmartPtr<T>() : FBSmartPtr<T>( m_pointer );
        }

        // implicit conversions
        T &operator*() const;
        T *operator->();
        const T *operator->() const;

        bool operator!() const;
        operator bool() const;

        // assignment
        FBWeakPtr &operator=( T *other ) throw();
        FBWeakPtr &operator=( const FBWeakPtr &other ) throw();
        //FBWeakPtr &operator=( const FBSmartPtr<T> &other ) throw();

        // comparisons
        bool operator==( T *other ) const;
        bool operator!=( T *other ) const;

        bool operator==( const FBWeakPtr &other ) const;
        bool operator!=( const FBWeakPtr &other ) const;

        bool operator==( const RawPtr<T> &other ) const;

        template <class B>
        bool operator==( const RawPtr<B> &other ) const;

        /**
         * Returns a raw pointer to the managed object, or nullptr if the object has been destroyed.
         *
         * @return A raw pointer to the managed object, or nullptr if the object has been destroyed.
         */
        T *get() const;

    protected:
        /**
         * Pointer to the owned object of the weak pointer.
         * Will be null if the weak pointer is empty.
         */
        T *m_pointer = nullptr;
    };

    template <class T>
    FBWeakPtr<T>::FBWeakPtr( T *object )
    {
        if( object )
        {
            if( object->getWeakReferences() != -1 )
            {
                m_pointer = object;
                object->addWeakReference();
            }
        }
    }

    template <class T>
    FBWeakPtr<T>::FBWeakPtr( const FBWeakPtr &other )
    {
        auto pObject = other.get();
        if( pObject )
        {
            if( pObject->getWeakReferences() != -1 )
            {
                m_pointer = pObject;
                pObject->addWeakReference();
            }
        }
    }

    template <class T>
    FBWeakPtr<T>::FBWeakPtr( const FBSmartPtr<T> &other )
    {
        auto pObject = other.get();
        if( pObject )
        {
            if( pObject->getWeakReferences() != -1 )
            {
                m_pointer = pObject;
                pObject->addWeakReference();
            }
        }
    }

    template <class T>
    FBWeakPtr<T>::FBWeakPtr( const RawPtr<T> &object )
    {
        *this = object.get();
    }

    template <class T>
    template <class B>
    FBWeakPtr<T>::FBWeakPtr( B *pointer )
    {
        T *object = nullptr;

#        if FB_ENABLE_PTR_EXCEPTIONS
        try
        {
            T *p = static_cast<T *>( pointer );  // static type checking
            p = p;                               // avoid warning

            object = (T *)dynamic_cast<T *>( pointer );
        }
        catch( std::exception & )
        {
            throw;
        }
#        else
        object = static_cast<T *>( pointer );
#        endif

        if( object )
        {
            if( object->getWeakReferences() != -1 )
            {
                object->addWeakReference();
                m_pointer = object;
            }
        }
    }

    template <class T>
    template <class B>
    FBWeakPtr<T>::FBWeakPtr( const FBWeakPtr<B> &rkPointer )
    {
        T *object = nullptr;

#        if FB_ENABLE_PTR_EXCEPTIONS
        try
        {
            B *pointer = rkPointer.get();
            T *p = static_cast<T *>( pointer );  // static type checking
            p = p;                               // avoid warning

            object = static_cast<T *>( dynamic_cast<T *>( pointer ) );
        }
        catch( std::exception & )
        {
            throw;
        }
#        else
        B *pointer = rkPointer.get();
        object = static_cast<T *>( pointer );
#        endif

        if( object )
        {
            if( object->getWeakReferences() != -1 )
            {
                object->addWeakReference();
                m_pointer = object;
            }
        }
    }

    template <class T>
    template <class B>
    FBWeakPtr<T>::FBWeakPtr( const FBSmartPtr<B> &other )
    {
        T *object = nullptr;

#        if FB_ENABLE_PTR_EXCEPTIONS
        try
        {
            B *pointer = other.get();
            T *p = static_cast<T *>( pointer );  // static type checking
            p = p;                               // avoid warning

            object = static_cast<T *>( dynamic_cast<T *>( pointer ) );
        }
        catch( std::exception & )
        {
            throw;
        }
#        else
        B *pointer = other.get();
        object = static_cast<T *>( pointer );
#        endif

        if( object )
        {
            if( object->getWeakReferences() != -1 )
            {
                object->addWeakReference();
                m_pointer = object;
            }
        }
    }

    template <class T>
    FBWeakPtr<T>::~FBWeakPtr()
    {
        auto object = get();
        if( object )
        {
#    if FB_TRACK_REFERENCES
            object->removeWeakReference( this, __FILE__, __LINE__, __FUNCTION__ );
#    else
            object->removeWeakReference();
#    endif

            m_pointer = nullptr;
        }
    }

    template <class T>
    T &FBWeakPtr<T>::operator*() const
    {
        FB_ASSERT( m_pointer );

#    if FB_ENABLE_PTR_EXCEPTIONS
        if( !m_pointer )
        {
            throw Exception( "Null pointer exception." );
        }
#    endif

        return *m_pointer;
    }

    template <class T>
    T *FBWeakPtr<T>::operator->()
    {
        FB_ASSERT( m_pointer );

#    if FB_ENABLE_PTR_EXCEPTIONS
        if( !m_pointer )
        {
            throw Exception( "Null pointer exception." );
        }
#    endif

        return m_pointer;
    }

    template <class T>
    const T *FBWeakPtr<T>::operator->() const
    {
        FB_ASSERT( m_pointer );

#    if FB_ENABLE_PTR_EXCEPTIONS
        if( !m_pointer )
        {
            throw Exception( "Null pointer exception." );
        }
#    endif

        return m_pointer;
    }

    template <class T>
    bool FBWeakPtr<T>::operator!() const
    {
        return m_pointer == nullptr;
    }

    template <class T>
    FBWeakPtr<T>::operator bool() const
    {
        return m_pointer != nullptr;
    }

    template <class T>
    FBWeakPtr<T> &FBWeakPtr<T>::operator=( T *other ) throw()
    {
        if( m_pointer != other )
        {
            T *pNewObject = nullptr;
            if( other )
            {
#    if FB_TRACK_REFERENCES
                other->addWeakReference( this, __FILE__, __LINE__, __FUNCTION__ );
#    else
                other->addWeakReference();
#    endif

                pNewObject = other;
            }

            auto pObject = this->get();
            if( pObject )
            {
#    if FB_TRACK_REFERENCES
                pObject->removeWeakReference( this, __FILE__, __LINE__, __FUNCTION__ );
#    else
                pObject->removeWeakReference();
#    endif
            }

            m_pointer = pNewObject;
        }

        return *this;
    }

    template <class T>
    FBWeakPtr<T> &FBWeakPtr<T>::operator=( const FBWeakPtr &other ) throw()
    {
        T *pNewObject = nullptr;
        
        if( auto otherObject = other.get() )
        {
#    if FB_TRACK_REFERENCES
            otherObject->addWeakReference( this, __FILE__, __LINE__, __FUNCTION__ );
#    else
            otherObject->addWeakReference();
#    endif

            pNewObject = otherObject;
        }
        
        if( auto pObject = get() )
        {
#    if FB_TRACK_REFERENCES
            pObject->removeWeakReference( this, __FILE__, __LINE__, __FUNCTION__ );
#    else
            pObject->removeWeakReference();
#    endif
        }

        m_pointer = pNewObject;

        return *this;
    }

    /*
    template <class T>
    FBWeakPtr<T> &FBWeakPtr<T>::operator=( const FBSmartPtr<T> &other ) throw()
    {
        T *pNewObject = nullptr;
        auto otherObject = other.get();
        if( otherObject )
        {
#    if FB_TRACK_REFERENCES
            otherObject->addWeakReference( this, __FILE__, __LINE__, __FUNCTION__ );
#    else
            otherObject->addWeakReference();
#    endif

            pNewObject = otherObject;
        }

        auto pObject = this->m_object;
        if( pObject )
        {
#    if FB_TRACK_REFERENCES
            pObject->removeWeakReference( this, __FILE__, __LINE__, __FUNCTION__ );
#    else
            pObject->removeWeakReference();
#    endif
        }

        m_object = pNewObject;

        return *this;
    }
    */

    template <class T>
    bool FBWeakPtr<T>::operator==( T *other ) const
    {
        return m_pointer == other;
    }

    template <class T>
    bool FBWeakPtr<T>::operator!=( T *other ) const
    {
        return m_pointer != other;
    }

    template <class T>
    bool FBWeakPtr<T>::operator==( const FBWeakPtr &other ) const
    {
        return m_pointer == other.get();
    }

    template <class T>
    bool FBWeakPtr<T>::operator!=( const FBWeakPtr &other ) const
    {
        return get() != other.get();
    }

    template <class T>
    bool FBWeakPtr<T>::operator==( const RawPtr<T> &other ) const
    {
        return m_pointer == other.get();
    }

    template <class T>
    template <class B>
    bool FBWeakPtr<T>::operator==( const RawPtr<B> &other ) const
    {
        return get() == other.get();
    }

    template <class T>
    T *FBWeakPtr<T>::get() const
    {
        return m_pointer;
    }

    /**
     * Get a raw pointer to the object pointed to by the given weak pointer.
     *
     * @tparam T The type of the object pointed to by the weak pointer.
     * @param pointer The weak pointer to the object.
     * @return A raw pointer to the object pointed to by the weak pointer.
     */
    template <class T>
    T *get_pointer( const FBWeakPtr<T> &pointer )
    {
        return pointer.get();
    }
}  // end namespace fb

#endif
