#ifndef __IData_h__
#define __IData_h__

#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Memory/RawPtr.h>

namespace fb
{
    /**
     * @brief An interface for passing data.
     */
    class IData : public ISharedObject
    {
    public:
        /**
         * @brief Destructor.
         */
        ~IData() override = default;

        /**
         * @brief Set the object data.
         * @param data A pointer to the data.
         */
        virtual void setData( void *data ) = 0;

        /**
         * @brief Get the object data.
         * @return A pointer to the data.
         */
        virtual void *getData() = 0;

        /**
         * @brief Get the object data.
         * @return A pointer to the data.
         */
        virtual const void *getData() const = 0;

        /**
         * @brief Get the object data as a type.
         * @tparam B The data type.
         * @return The data as a type.
         */
        template <typename B>
        RawPtr<B> getDataAsType() const
        {
            auto p = getData();
            auto data = const_cast<void *>( p );
            auto pData = static_cast<B *>( data );
            return RawPtr<B>( pData );
        }

        FB_CLASS_REGISTER_DECL;
    };

}  // end namespace fb

#endif  // IData_h__
