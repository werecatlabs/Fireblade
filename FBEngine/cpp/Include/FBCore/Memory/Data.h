#ifndef __CData_h__
#define __CData_h__

#include <FBCore/Memory/SharedObject.h>
#include <FBCore/Interface/Memory/IData.h>

namespace fb
{

    template <class T>
    class Data : public SharedObject<IData>
    {
    public:
        Data() : SharedObject<IData>()
        {
        }

        Data( const T &data ) : SharedObject<IData>()
        {
            m_data = data;
        }

        ~Data() override = default;

        void setData( void *data ) override
        {
            m_data = *static_cast<T *>( data );
        }

        void *getData() override
        {
            return static_cast<void *>( &m_data );
        }

        const void *getData() const override
        {
            auto p = const_cast<T *>( &m_data );
            return static_cast<void *>( p );
        }

    private:
        T m_data;
    };
}  // end namespace fb

#endif  // CData_h__
