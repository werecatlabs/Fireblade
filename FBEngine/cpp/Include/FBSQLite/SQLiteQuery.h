#ifndef FBSQLiteQuery_h__
#define FBSQLiteQuery_h__

#include <FBCore/Interface/Database/IDatabaseQuery.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/Array.h>
#include <FBSQLite/extern/CppSQLite3.h>

namespace fb
{
    class SQLiteQuery : public IDatabaseQuery
    {
    public:
        SQLiteQuery();
        SQLiteQuery( CppSQLite3Query &query );
        ~SQLiteQuery() override;

        void setQuery( CppSQLite3Query &query );

        String getFieldName( u32 index ) override;
        String getFieldValue( u32 index ) override;
        String getFieldValue( const String &field ) override;

        s32 getFieldValueAsInt( const String &fieldName );

        f32 getFieldValueAsFloat( const String &fieldName );

        size_t getNumFields() const override;

        bool isFieldValueNull( const String &field ) override;

        void nextRow() override;
        bool eof() override;

        std::string toXML();
        std::string toXMLGroup();

    protected:
        class Field
        {
        public:
            Field();

            Field( const String &name, const String &value );

            Field( const Field &other );

            ~Field();

            String name;
            String value;
        };

        class Rows
        {
        public:
            Rows();

            Rows( const Rows &other );

            ~Rows();

            Array<Field> fields;
        };

        s32 m_currentRow = 0;
        Array<Rows> m_rows;
    };
}  // end namespace fb

#endif  // FBSQLiteQuery_h__
