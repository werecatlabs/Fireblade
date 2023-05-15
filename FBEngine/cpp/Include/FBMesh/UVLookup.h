#ifndef _UVLookup_H
#define _UVLookup_H

#include <FBMesh/FBMeshPrerequisites.h>
#include <FBCore/Memory/CSharedObject.h>
#include "FBCore/Base/Grid2.h"
#include "FBCore/Base/StringTypes.h"
#include "FBCore/Math/Vector2.h"

namespace fb
{
    //---------------------------------------------
    class UVLookup
    {
    public:
        UVLookup();
        UVLookup( const Vector2F &dimensions );
        ~UVLookup();

        void load( const String &filePath );
        void save( const String &filePath );

        inline Vector2F getUV( const Vector2F &position ) const;

        void add( const Vector2F &position, const Vector2F &uv );
        bool remove( const Vector2F &position );

        void setDimensions( const Vector2F &dimensions );
        Vector2F getDimensions() const;

    protected:
        Grid2 m_grid;

        using UVLookupMap = Array<Vector2F>;
        UVLookupMap m_uvLookupMap;
    };
} // end namespace fb

#endif
