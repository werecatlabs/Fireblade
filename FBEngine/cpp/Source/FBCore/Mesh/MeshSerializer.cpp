/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2012 Torus Knot Software Ltd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/
#include <FBCore/FBCorePCH.h>
#include "FBCore/Mesh/MeshSerializer.h"
#include "FBCore/Mesh/MeshFileFormat.h"
#include <FBFileSystem/FileDataStream.h>
#include <FBCore/FBCore.h>
#include <FBCore/Mesh/Mesh.h>
#include <fstream>

namespace fb
{
    const unsigned short HEADER_CHUNK_ID = 0x1000;
    //---------------------------------------------------------------------
    MeshSerializer::MeshSerializer() : mListener( nullptr )
    {
        // Init implementations
        // String identifiers have not always been 100% unified with OGRE version

        // Note MUST be added in reverse order so latest is first in the list
        mVersionData.push_back(
            new MeshVersionData( MESH_VERSION_1_8, "[MeshSerializer_v1.8]", new MeshSerializerImpl() ) );

        mVersionData.push_back( new MeshVersionData( MESH_VERSION_1_7, "[MeshSerializer_v1.41]",
                                                     new MeshSerializerImpl_v1_41() ) );

        mVersionData.push_back( new MeshVersionData( MESH_VERSION_1_4, "[MeshSerializer_v1.40]",
                                                     new MeshSerializerImpl_v1_4() ) );

        mVersionData.push_back( new MeshVersionData( MESH_VERSION_1_0, "[MeshSerializer_v1.30]",
                                                     new MeshSerializerImpl_v1_3() ) );
        mVersionData.push_back( new MeshVersionData( MESH_VERSION_LEGACY, "[MeshSerializer_v1.20]",
                                                     new MeshSerializerImpl_v1_2() ) );

        mVersionData.push_back( new MeshVersionData( MESH_VERSION_LEGACY, "[MeshSerializer_v1.10]",
                                                     new MeshSerializerImpl_v1_1() ) );
    }

    //---------------------------------------------------------------------
    MeshSerializer::~MeshSerializer()
    {
        // delete map
        for( auto i = mVersionData.begin(); i != mVersionData.end(); ++i )
        {
            delete *i;
        }
        mVersionData.clear();
    }

    //---------------------------------------------------------------------
    void MeshSerializer::exportMesh( const Mesh *pMesh, const String &filename, u32 endianMode )
    {
        auto f = new std::fstream;
        f->open( filename.c_str(), std::ios::binary | std::ios::out );
        SmartPtr<IStream> stream( new FileDataStream( f ) );

        exportMesh( pMesh, stream, endianMode );

        stream->close();
    }

    //---------------------------------------------------------------------
    void MeshSerializer::exportMesh( const Mesh *pMesh, const String &filename, MeshVersion version,
                                     u32 endianMode )
    {
        auto f = new std::fstream;
        f->open( filename.c_str(), std::ios::binary | std::ios::out );
        SmartPtr<IStream> stream( new FileDataStream( f ) );

        exportMesh( pMesh, stream, version, endianMode );

        stream->close();
    }

    //---------------------------------------------------------------------
    void MeshSerializer::exportMesh( const Mesh *pMesh, SmartPtr<IStream> stream, u32 endianMode )
    {
        exportMesh( pMesh, stream, MESH_VERSION_LATEST, endianMode );
    }

    //---------------------------------------------------------------------
    void MeshSerializer::exportMesh( const Mesh *pMesh, SmartPtr<IStream> stream, MeshVersion version,
                                     u32 endianMode )
    {
        if( version == MESH_VERSION_LEGACY )
        {
            FB_LOG_ERROR(
                "MeshSerializer::exportMesh: You may not supply a legacy version number (pre v1.0) for "
                "writing meshes." );
        }

        MeshSerializerImpl *impl = nullptr;
        if( version == MESH_VERSION_LATEST )
            impl = mVersionData[0]->impl;
        else
        {
            for( auto i = mVersionData.begin(); i != mVersionData.end(); ++i )
            {
                if( version == ( *i )->version )
                {
                    impl = ( *i )->impl;
                    break;
                }
            }
        }

        if( !impl )
        {
            FB_LOG_ERROR( "MeshSerializer::exportMesh: Cannot find serializer implementation" );
        }

        impl->exportMesh( pMesh, stream, endianMode );
    }

    //---------------------------------------------------------------------
    void MeshSerializer::importMesh( SmartPtr<IStream> &stream, Mesh *pDest )
    {
        determineEndianness( stream );

        // Jump back to start
        stream->seek( 0 );

        // Read header and determine the version
        unsigned short headerID;

        // Read header ID
        readShorts( stream, &headerID, 1 );

        if( headerID != HEADER_CHUNK_ID )
        {
            FB_EXCEPTION( "File header not found" );
        }

        // Read version
        String ver = readString( stream );
        // Jump back to start
        stream->seek( 0 );

        // Find the implementation to use
        MeshSerializerImpl *impl = nullptr;
        for( auto i = mVersionData.begin(); i != mVersionData.end(); ++i )
        {
            if( ( *i )->versionString == ver )
            {
                impl = ( *i )->impl;
                break;
            }
        }
        if( !impl )
        {
            FB_EXCEPTION(
                ( String( "Cannot find serializer implementation for mesh version " ) + ver ).c_str() );
        }

        // Call implementation
        impl->importMesh( stream, pDest, mListener );
        // Warn on old version of mesh
        if( ver != mVersionData[0]->versionString )
        {
            FB_LOG_MESSAGE( "MeshSerializer",
                            String( "WARNING: " ) + pDest->getName() +
                                String( " is an older format (" ) + ver +
                                String( "); you should upgrade it as soon as possible" ) +
                                String( " using the OgreMeshUpgrade tool." ) );
        }
    }

    SmartPtr<IMesh> MeshSerializer::loadMesh( SmartPtr<IStream> &stream )
    {
        auto pDest = fb::make_ptr<Mesh>();

        // Read header and determine the version
        unsigned short headerID;

        // Read header ID
        readShorts( stream, &headerID, 1 );

        if( headerID != HEADER_CHUNK_ID )
        {
            FB_EXCEPTION( "File header not found" );
        }

        // Read version
        String ver = readString( stream );
        // Jump back to start
        stream->seek( 0 );

        // Find the implementation to use
        MeshSerializerImpl *impl = nullptr;
        for( auto i = mVersionData.begin(); i != mVersionData.end(); ++i )
        {
            if( ( *i )->versionString == ver )
            {
                impl = ( *i )->impl;
                break;
            }
        }
        if( !impl )
        {
            FB_EXCEPTION(
                ( String( "Cannot find serializer implementation for mesh version " ) + ver ).c_str() );
        }

        // Call implementation
        impl->importMesh( stream, static_cast<Mesh *>( pDest.get() ), mListener );
        // Warn on old version of mesh
        if( ver != mVersionData[0]->versionString )
        {
            FB_LOG_MESSAGE( "MeshSerializer",
                            String( "WARNING: " ) + pDest->getName() +
                                String( " is an older format (" ) + ver +
                                String( "); you should upgrade it as soon as possible" ) +
                                String( " using the OgreMeshUpgrade tool." ) );
        }

        return pDest;
    }

    //---------------------------------------------------------------------
    void MeshSerializer::setListener( MeshSerializerListener *listener )
    {
        mListener = listener;
    }

    //----------------------------------------------
    MeshSerializerListener *MeshSerializer::getListener()
    {
        return mListener;
    }
}  // namespace fb
