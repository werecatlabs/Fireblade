#ifndef FBSQLiteQuery_h__
#define FBSQLiteQuery_h__

#include <FBCore/Interface/Database/IDatabaseQuery.h>
#include <FBCore/Memory/CSharedObject.h>
#include <FBCore/Base/Array.h>
#include <FBSQLite/extern/CppSQLite3.h>

namespace fb
{
    class SQLiteQuery : public CSharedObject<IDatabaseQuery>
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
            String name;
            String value;
        };

        class Rows
        {
        public:
            Array<Field> fields;
        };

        s32 m_currentRow = 0;
        Array<Rows> m_rows;
    };
}  // end namespace fb

#endif  // FBSQLiteQuery_h__
