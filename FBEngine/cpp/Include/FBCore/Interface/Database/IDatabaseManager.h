#ifndef IDatabaseManager_h__
#define IDatabaseManager_h__

#include <FBCore/Interface/Scene/IComponent.h>
#include <FBCore/Base/StringTypes.h>

namespace fb
{

    /**
     * Interface for a database manager.
     */
    class IDatabaseManager : public scene::IComponent
    {
    public:
        /** Virtual destructor. */
        ~IDatabaseManager() override = default;

        /** Opens the database. */
        virtual void open() = 0;

        /** Closes the database. */
        virtual void close() = 0;

        /** Loads the database from a handle. */
        virtual void loadFromHandle( size_t handle ) = 0;

        /** Loads the database from a file path. */
        virtual void loadFromFile( const String &filePath ) = 0;

        /** Loads the database from a file path. */
        virtual void loadFromFile( const StringW &filePath ) = 0;

        /** Attaches the specified database file. */
        virtual void attach( const String &filePath ) = 0;

        /** Attaches the specified database file. */
        virtual void attach( const StringW &filePath ) = 0;

        /** Executes the specified query on the database. */
        virtual SmartPtr<IDatabaseQuery> executeQuery( const String &query ) = 0;

        /** Executes the specified DML (Data Manipulation Language) query on the database. */
        virtual s32 executeDML( const String &dml ) = 0;

        /** Executes the specified DML (Data Manipulation Language) query on the database. */
        virtual s32 executeDML( const StringW &dml ) = 0;

        /** Executes the specified DML (Data Manipulation Language) query on the database asynchronously. */
        virtual s32 executeAsyncDML( const String &tag, const String &dml ) = 0;

        /** Gets the current database. */
        virtual SmartPtr<IDatabase> getDatabase() const = 0;

        /** Sets the current database. */
        virtual void setDatabase( SmartPtr<IDatabase> database ) = 0;

        /** Optimizes the database. */
        virtual void optmise() = 0;

        /** Gets the attached databases. */
        virtual Array<String> getAttached() const = 0;

        /** Sets the attached databases. */
        virtual void setAttached( const Array<String> &attached ) = 0;

        /** Adds an attached database. */
        virtual void addAttached( const String &attached ) = 0;

        /** Gets the path of the database. */
        virtual String getDatabasePath() const = 0;

        /** Sets the path of the database. */
        virtual void setDatabasePath( const String &databasePath ) = 0;

        FB_CLASS_REGISTER_DECL;
    };

}  // end namespace fb

#endif  // IDatabaseManager_h__
