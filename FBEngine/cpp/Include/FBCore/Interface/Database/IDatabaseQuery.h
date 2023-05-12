#ifndef IDatabaseQuery_h__
#define IDatabaseQuery_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{

    /**
     * Interface for a database query.
     */
    class IDatabaseQuery : public ISharedObject
    {
    public:
        /**
         * Virtual destructor.
         */
        ~IDatabaseQuery() override = default;

        /**
         * Returns the number of fields in the query.
         *
         * @return The number of fields.
         */
        virtual size_t getNumFields() const = 0;

        /**
         * Returns the name of the field at the given index.
         *
         * @param index The index of the field.
         * @return The name of the field.
         */
        virtual String getFieldName( u32 index ) = 0;

        /**
         * Returns the value of the field at the given index.
         *
         * @param index The index of the field.
         * @return The value of the field.
         */
        virtual String getFieldValue( u32 index ) = 0;

        /**
         * Returns the value of the field with the given name.
         *
         * @param field The name of the field.
         * @return The value of the field.
         */
        virtual String getFieldValue( const String &field ) = 0;

        /**
         * Determines whether the value of the field with the given name is null.
         *
         * @param field The name of the field.
         * @return true if the value of the field is null, false otherwise.
         */
        virtual bool isFieldValueNull( const String &field ) = 0;

        /**
         * Advances to the next row in the query result set.
         */
        virtual void nextRow() = 0;

        /**
         * Determines whether the end of the query result set has been reached.
         *
         * @return true if the end of the result set has been reached, false otherwise.
         */
        virtual bool eof() = 0;
    };

}  // end namespace fb

#endif  // IDatabaseQuery_h__
