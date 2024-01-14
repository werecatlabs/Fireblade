#ifndef IOSystem_h__
#define IOSystem_h__

#include <FBAssimp/FBAssimpPrerequisites.h>
#include <FBCore/Core/StringTypes.h>
#include <assimp/IOSystem.hpp>

namespace fb
{

    class IOSystem : public Assimp::IOSystem
    {
    public:
        IOSystem();
        IOSystem( const SmartPtr<IStream> &_source, String group );

        bool Exists( const char *pFile ) const;
        char getOsSeparator() const override;

        Assimp::IOStream *Open( const char *pFile, const char *pMode );
        void Close( Assimp::IOStream *pFile );

    protected:
        SmartPtr<IStream> source;
        std::vector<IOStream *> streams;
        String _group;
    };

}  // namespace fb

#endif  // IOSystem_h__
