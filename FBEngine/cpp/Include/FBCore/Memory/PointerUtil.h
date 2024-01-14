#ifndef PointerUtil_h__
#define PointerUtil_h__

#include <FBCore/Memory/SmartPtr.h>
#include <FBCore/Memory/RawPtr.h>
#include <iostream>

#if FB_USE_BOOST
#    include <boost/make_shared.hpp>
#else
#    include <memory>
#endif

namespace fb
{

    template <class T>
    SmartPtr<T> make_ptr()
    {
        auto ptr = FB_NEW T();
        auto p = SmartPtr<T>( ptr );
        ptr->removeReference();
        return p;
    }

    template <class T, class... _Types>
    SmartPtr<T> make_ptr( _Types &&..._Args )
    {
        auto ptr = FB_NEW T( std::forward<_Types>( _Args )... );
        auto p = SmartPtr<T>( ptr );
        ptr->removeReference();
        return p;
    }

    template <class T>
    SharedPtr<T> make_shared()
    {
#if FB_USE_BOOST
        return boost::make_shared<T>();
#else
        return std::make_shared<T>();
#endif
    }

    template <class T, class... _Types>
    SharedPtr<T> make_shared( _Types &&..._Args )
    {
#if FB_USE_BOOST
        return boost::make_shared<T>( std::forward<_Types>( _Args )... );
#else
        return std::make_shared<T>( std::forward<_Types>( _Args )... );
#endif
    }

    template <class T>
    SharedPtr<Array<T>> make_shared_array( size_t size, SharedPtr<Array<T>> data, const T &defaultValue )
    {
        auto p = fb::make_shared<Array<T>>();
        auto &newArray = *p;
        newArray.resize( size, defaultValue );

        if( data )
        {
            auto &dataArray = *data;

            for( size_t i = 0; i < size && i < dataArray.size(); ++i )
            {
                newArray[i] = dataArray[i];
            }
        }

        return p;
    }

    template <class T, class B>
    SmartPtr<T> &reinterpret_pointer_cast( SmartPtr<B> &obj )
    {
        return *reinterpret_cast<SmartPtr<T> *>( &obj );
    }

    template <class T, class B>
    const SmartPtr<T> &reinterpret_pointer_cast( const SmartPtr<B> &obj )
    {
        return *reinterpret_cast<const SmartPtr<T> *>( &obj );
    }

    template <class T, class B>
    SmartPtr<T> static_pointer_cast( SmartPtr<B> obj )
    {
        auto ptr = static_cast<T *>( obj.get() );
        return SmartPtr<T>( ptr );
    }

    template <class T, class B>
    SmartPtr<T> dynamic_pointer_cast( SmartPtr<B> obj )
    {
        try
        {
            auto ptr = dynamic_cast<T *>( obj.get() );
            if( ptr )
            {
                return SmartPtr<T>( ptr );
            }
        }
        catch( std::exception &e )
        {
            std::cout << e.what() << std::endl;
        }

        return SmartPtr<T>();
    }

    template <class T, class B>
    SmartPtr<T> dynamic_pointer_cast( RawPtr<B> &obj )
    {
        try
        {
            auto ptr = dynamic_cast<T *>( obj.get() );
            if( ptr )
            {
                return SmartPtr<T>( ptr );
            }
        }
        catch( std::exception &e )
        {
            std::cout << e.what() << std::endl;
        }

        return SmartPtr<T>();
    }

    template <class T>
    RawPtr<T> ptr_to_raw( SmartPtr<T> &other )
    {
        return RawPtr<T>( other.get() );
    }

    template <class T>
    SmartPtr<T> ptr_to_smart( RawPtr<T> &other )
    {
        return SmartPtr<T>( other.get() );
    }

    template <class T>
    void FB_SAFE_DELETE( RawPtr<T> &ptr )
    {
        T *p = ptr.get();
        if( p )
        {
            delete p;
            p = nullptr;
        }

        ptr = nullptr;
    }

    template <class T>
    void FB_SAFE_DELETE_ARRAY( RawPtr<T> &ptr )
    {
        T *p = ptr.get();
        if( p )
        {
            delete[] p;
            p = nullptr;
        }

        ptr = nullptr;
    }

}  // end namespace fb

#endif  // PointerUtil_h__
