#include "FBGraphics/MeshConverter.h"
#include <FBCore/Scene/Components/Mesh.h>
#include <FBCore/Core/Path.h>

#ifdef COLLADA_SUPPORT
#    include "FUtils/FUtils.h"
#    include "FCollada.h"
#    include "FCDocument/FCDocument.h"
#    include "FCDocument/FCDAnimationCurve.h"
#    include "FCDocument/FCDAnimationClip.h"
#    include "FCDocument/FCDAnimationChannel.h"
#    include "FCDocument/FCDAnimation.h"
#    include "FCDocument/FCDAsset.h"
#    include "FCDocument/FCDSceneNode.h"
#    include "FCDocument/FCDTransform.h"
#    include "FCDocument/FCDGeometry.h"
#    include "FCDocument/FCDGeometryMesh.h"
#    include "FCDocument/FCDGeometrySource.h"
#    include "FCDocument/FCDGeometryPolygons.h"
#    include "FCDocument/FCDGeometryPolygonsInput.h"
#endif

#if FB_GRAPHICS_SYSTEM_OGRENEXT
#    include "Ogre.h"
#    include "OgreMesh.h"
#    include "OgreMeshManager.h"
#    include "OgreMeshSerializer.h"
#    include "OgreDefaultHardwareBufferManager.h"
#    include "OgreVertexIndexData.h"
#    include "OgreHardwareIndexBuffer.h"
#elif FB_GRAPHICS_SYSTEM_OGRE
#    include "Ogre.h"
#    include <OgreMeshSerializer.h>
#    include <OgreSkeletonSerializer.h>
#    include <OgreSubMesh.h>
#    include <OgreMaterialManager.h>
#    include <OgreMaterialSerializer.h>
#    include <OgreLogManager.h>
#    include <OgreSkeleton.h>
#    include <OgreStringConverter.h>
#    include <OgreDefaultHardwareBufferManager.h>
#endif

#include <FBCore/Interface/Mesh/IMesh.h>
#include <FBCore/FBCoreHeaders.h>
#include <FBMesh/FBMeshHeaders.h>

//#include <irrlicht.h>
#include <memory>

const int MAX_MESHMAPS = 8;

namespace fb
{

    //--------------------------------------------
    MeshConverter::MeshConverter()
    {
    }

    //--------------------------------------------
    MeshConverter::~MeshConverter()
    {
    }

    //--------------------------------------------
    bool MeshConverter::ExportSelected( const char *fileName )
    {
        return true;
    }

    //--------------------------------------------
    void MeshConverter::writeMesh( const SmartPtr<scene::IActor> actor )
    {
        try
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();

                auto meshComponent = actor->getComponent<scene::Mesh>();
                if( meshComponent )
                {
                    auto mesh = meshComponent->getMesh();
                    auto pMesh = mesh.get();
                    if( pMesh )
                    {
                        MeshSerializer serializer;

                        auto cacheFolder = applicationManager->getCachePath();
                        auto workingDir = Path::getWorkingDirectory();
                        Path::setWorkingDirectory( cacheFolder );
                        auto fileName = pMesh->getName() + ".fbmeshbin";
                        serializer.exportMesh( (Mesh *)pMesh, fileName );
                        Path::setWorkingDirectory( workingDir );
                    }
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }

            if( auto p = actor->getChildrenPtr() )
            {
                auto &children = *p;
                for( auto child : children )
                {
                    writeMesh( child );
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    //--------------------------------------------
    void MeshConverter::writeOgreMesh( const SmartPtr<scene::IActor> actor )
    {
        try
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();

                auto meshComponent = actor->getComponent<scene::Mesh>();
                if( meshComponent )
                {
                    auto mesh = meshComponent->getMesh();
                    if( mesh )
                    {
                        auto cacheFolder = applicationManager->getCachePath();
                        auto workingDir = Path::getWorkingDirectory();
                        Path::setWorkingDirectory( cacheFolder );

                        convertFBMeshToOgreMesh( mesh->getName(), mesh );

                        Path::setWorkingDirectory( workingDir );
                    }
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }

            if( auto p = actor->getChildrenPtr() )
            {
                auto &children = *p;
                for( auto child : children )
                {
                    writeOgreMesh( child );
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void MeshConverter::covertIrrMeshToOgreMesh()
    {
        /*
        CompSceneNode* compSceneNode = (CompSceneNode*)entity->getComponent(CompSceneNode::Type);
        if(!compSceneNode)
            return;

        String keyValue;
        entity->getPropertyGroup()->GetPropertyValue("Name", keyValue);

        Ogre::String meshName = keyValue.c_str();
        Ogre::MeshManager::ResourceCreateOrRetrieveResult createOrRetriveMesh = Ogre::MeshManager::getSingletonPtr()->createOrRetrieve(meshName, Ogre::String("General"), true);
        if(!createOrRetriveMesh.second)
        {
            Ogre::LogManager::getSingleton().logMessage("maxExporter::doMesh resource already exists");
            Ogre::LogManager::getSingleton().logMessage("maxExporter::doMesh end");
            return;
        }

        Ogre::MeshPtr pMesh = createOrRetriveMesh.first;
        Ogre::SubMesh *pSubMesh	= NULL;
        Ogre::VertexData *pData	= NULL;
        Ogre::VertexBufferBinding *pBind = NULL;
        Ogre::VertexDeclaration *pDecl = NULL;
        Ogre::HardwareVertexBufferSharedPtr pBuf;// = NULL;
        Ogre::Real *pReal= NULL;
        Ogre::Vector3 vMin, vMax, currPos;
        Ogre::Real maxSquaredRadius;
        bool firstVert = true;

        scene::IMesh* irrMesh = 0;

        SmartPtr<ISceneNode> irrSceneNode = compSceneNode->getSceneNode();
        switch(irrSceneNode->getType())
        {
        case scene::ESNT_MESH:
            {
                scene::IMeshSceneNode* irrMeshSceneNode = (scene::IMeshSceneNode*)irrSceneNode.getPtr();
                irrMesh = irrMeshSceneNode->getMesh();
            }
            break;
        default:
            {
                return;
            }
        };



        u32 numMaterials = irrMesh->getMeshBufferCount();
        for(u32 matIdx=0; matIdx<numMaterials; ++matIdx )
        {
            Ogre::String matName = pMesh->getName() + "_SubMesh_" + Ogre::StringConverter::toString(matIdx);

            scene::IMeshBuffer* meshBuffer = irrMesh->getMeshBuffer(matIdx);

            // create subMesh
            pSubMesh = pMesh->createSubMesh(matName);
            pSubMesh->setMaterialName(matName);
            pSubMesh->useSharedVertices = false;
            pSubMesh->vertexData = new Ogre::VertexData();
            pSubMesh->indexData = new Ogre::IndexData();
            size_t numVert = pSubMesh->vertexData->vertexCount = meshBuffer->getVertexCount();
            pDecl = pSubMesh->vertexData->vertexDeclaration;
            pBind = pSubMesh->vertexData->vertexBufferBinding;
            //
            unsigned short iBinding = 0;
            // do positions first
            pDecl->addElement(iBinding, 0, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
            pBuf = Ogre::DefaultHardwareBufferManager::getSingletonPtr()->createVertexBuffer(pDecl->getVertexSize(iBinding), numVert, Ogre::HardwareBuffer::HBU_DYNAMIC, false);
            pBind->setBinding(iBinding, pBuf);
            pReal = static_cast<Ogre::Real*>(pBuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));

            // fill vertices
            //for (int vertIdx = 0; vertIdx < numVert; vertIdx++)
            for (int vertIdx = numVert-1; vertIdx >= 0; vertIdx--)
            {
                Vector3F v;

                switch(meshBuffer->getVertexType())
                {
                case video::EVT_STANDARD:
                    {
                        video::S3DVertex* mb_vertices=(irr::video::S3DVertex*)meshBuffer->getVertices();
                        v = mb_vertices[vertIdx].Pos;
                    }
                    break;
                case video::EVT_2TCOORDS:
                    {
                        video::S3DVertex2TCoords* mb_vertices=(irr::video::S3DVertex2TCoords*)meshBuffer->getVertices();
                        v = mb_vertices[vertIdx].Pos;
                    }
                    break;
                default:
                    {
                    }
                };

                Ogre::Vector3 pos = Ogre::Vector3(v.X, v.Y, -v.Z);
                Ogre::LogManager::getSingleton().logMessage("maxExporter::doMesh Mesh vert: " + Ogre::StringConverter::toString(pos));

                *pReal++ = pos.x;
                *pReal++ = pos.y;
                *pReal++ = pos.z;
                // Deal with bounds
                currPos = pos;
                if (firstVert)
                {
                    vMin = vMax = currPos;
                    maxSquaredRadius = currPos.squaredLength();
                    firstVert = false;
                }
                else
                {
                    vMin.makeFloor(currPos);
                    vMax.makeCeil(currPos);
                    maxSquaredRadius = max_(maxSquaredRadius, currPos.squaredLength());
                }
            }

            // do normals
            iBinding++;
            pDecl->addElement(iBinding, 0, Ogre::VET_FLOAT3, Ogre::VES_NORMAL);
            pBuf = Ogre::DefaultHardwareBufferManager::getSingletonPtr()->createVertexBuffer(pDecl->getVertexSize(iBinding), numVert, Ogre::HardwareBuffer::HBU_DYNAMIC, false);
            pBind->setBinding(iBinding, pBuf);
            pReal = static_cast<Ogre::Real*>(pBuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));
            //for (int vertIdx = 0; vertIdx < numVert; vertIdx++)
            for (int vertIdx = numVert-1; vertIdx >= 0; vertIdx--)
            {
                Vector3F irrNor;

                switch(meshBuffer->getVertexType())
                {
                case video::EVT_STANDARD:
                    {
                        video::S3DVertex* mb_vertices=(irr::video::S3DVertex*)meshBuffer->getVertices();
                        irrNor = mb_vertices[vertIdx].Normal;
                    }
                    break;
                case video::EVT_2TCOORDS:
                    {
                        video::S3DVertex2TCoords* mb_vertices=(irr::video::S3DVertex2TCoords*)meshBuffer->getVertices();
                        irrNor = mb_vertices[vertIdx].Normal;
                    }
                    break;
                default:
                    {
                    }
                };

                Ogre::Vector3 nor;
                nor.x = irrNor.X;
                nor.y = irrNor.Y;
                nor.z = irrNor.Z;
                //nor =  world2objN * nor;
                *pReal++ = nor.x;
                *pReal++ = nor.y;
                *pReal++ = nor.z;
            }

            iBinding++;
            pDecl->addElement(iBinding, 0, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES, 0);
            pBuf = Ogre::DefaultHardwareBufferManager::getSingletonPtr()->createVertexBuffer(pDecl->getVertexSize(iBinding), numVert, Ogre::HardwareBuffer::HBU_DYNAMIC, false);
            pBind->setBinding(iBinding, pBuf);
            pReal = static_cast<Ogre::Real*>(pBuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));
            //for (int vertIdx = 0; vertIdx < numVert; vertIdx++)
            for (int vertIdx = numVert-1; vertIdx >= 0; vertIdx--)
            {
                Vector2F tc;

                switch(meshBuffer->getVertexType())
                {
                case video::EVT_STANDARD:
                    {
                        video::S3DVertex* mb_vertices=(irr::video::S3DVertex*)meshBuffer->getVertices();
                        tc = mb_vertices[vertIdx].TCoords;
                    }
                    break;
                case video::EVT_2TCOORDS:
                    {
                        video::S3DVertex2TCoords* mb_vertices=(irr::video::S3DVertex2TCoords*)meshBuffer->getVertices();
                        tc = mb_vertices[vertIdx].TCoords;
                    }
                    break;
                default:
                    {
                    }
                };

                *pReal++ = tc.X;
                *pReal++ = tc.Y;

            }// for vertex
        }

        // do face indices
        for (int n = 0; n < pMesh->getNumSubMeshes(); ++n)
        {
            // update progress
            //showProgress("MESH '" + pMesh->getName() + "' : Indices/SubMesh ", n, pMesh->getNumSubMeshes());

            pSubMesh = pMesh->getSubMesh(n);
            bool useWords = (pSubMesh->vertexData->vertexCount >= 65535) ? false : true;
            pSubMesh->indexData->indexCount = pSubMesh->vertexData->vertexCount;
            Ogre::HardwareIndexBufferSharedPtr pIBuf =
                Ogre::DefaultHardwareBufferManager::getSingletonPtr()->createIndexBuffer(	useWords ? Ogre::HardwareIndexBuffer::IT_16BIT : Ogre::HardwareIndexBuffer::IT_32BIT,
                pSubMesh->indexData->indexCount,
                Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
            pSubMesh->indexData->indexBuffer = pIBuf;

            unsigned short *pWords = useWords ? static_cast<unsigned short*>(pIBuf->lock(Ogre::HardwareBuffer::HBL_DISCARD)) : NULL;
            unsigned int *pDWords = useWords ? NULL : static_cast<unsigned int*>(pIBuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));
            //for (int i = 0; i < (int)pSubMesh->indexData->indexCount; i++)
            for (int i = (int)pSubMesh->indexData->indexCount - 1; i >= 0; i--)
            {
                if (useWords)
                    *pWords++ = i;
                else
                    *pDWords++ = i;
            }
        }

        // Set bounds
        pMesh->_setBoundingSphereRadius(Ogre::Math::Sqrt(maxSquaredRadius));
        pMesh->_setBounds(Ogre::AxisAlignedBox(vMin, vMax));
        Ogre::LogManager::getSingleton().logMessage("maxExporter::doMesh Mesh bounding box min: " + Ogre::StringConverter::toString(vMin) + " max: " + Ogre::StringConverter::toString(vMax));

        // do tangents generation if asked to
        try
        {
            unsigned short src = 0, dest = 0;
            if (!pMesh->suggestTangentVectorBuildParams(Ogre::VES_TANGENT, src, dest))
            {
                Ogre::LogManager::getSingleton().logMessage("maxExporter::doMesh generating tangents");
                pMesh->buildTangentVectors();
            }
        }
        catch (Ogre::Exception& e)
        {
            MessageBox( NULL, e.what().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
        }

        // save mesh and finish ...
        Ogre::MeshSerializer *meshSer = new Ogre::MeshSerializer;
        meshSer->exportMesh(pMesh.getPointer(), pMesh->getName() + ".mesh");
        delete meshSer;
        */
    }

    //--------------------------------------------
    void MeshConverter::convertFBMeshToFColladaMesh( SmartPtr<IMesh> mesh, FCDGeometryMesh *colladaMesh )
    {
#ifdef COLLADA_SUPPORT
        u32 totalNumVerts = 0;
        u32 totalNumIndices = 0;

        Array<Vector3F> positions;
        Array<Vector3F> normals;
        Array<Vector2F> texCoords0;
        Array<Vector2F> texCoords1;
        Array<u32> indicesAll;

        const FBSubMeshList subMeshList = mesh->getSubMeshes();
        for( u32 subMeshIdx = 0; subMeshIdx < subMeshList.size(); ++subMeshIdx )
        {
            const FBSubMeshPtr &subMesh = subMeshList[subMeshIdx];

            const FBIndexBufferPtr &indexBuffer = subMesh->getIndexBuffer();
            const FBVertexBufferPtr &vertexBuffer = subMesh->getVertexBuffer();

            u32 vertexCount = vertexBuffer->getNumVerticies();
            u32 indexCount = indexBuffer->getNumIndices();

            totalNumVerts += vertexCount;
            totalNumIndices += indexCount;
        }

        positions.reallocate( totalNumVerts );
        normals.reallocate( totalNumVerts );
        texCoords0.reallocate( totalNumVerts );
        texCoords1.reallocate( totalNumVerts );

        indicesAll.reallocate( totalNumIndices );

        u32 indiceOffset = 0;

        for( u32 subMeshIdx = 0; subMeshIdx < subMeshList.size(); ++subMeshIdx )
        {
            const FBSubMeshPtr &subMesh = subMeshList[subMeshIdx];

            FBIndexBufferPtr indexBuffer = subMesh->getIndexBuffer();
            FBVertexBufferPtr vertexBuffer = subMesh->getVertexBuffer();

            int vertexCount = vertexBuffer->getNumVerticies();
            int indexCount = indexBuffer->getNumIndices();

            indiceOffset += vertexCount;

            const FBVertexElement *posElem =
                vertexBuffer->getVertexDeclaration()->findElementBySemantic( fb::VES_POSITION );
            const FBVertexElement *normalElem =
                vertexBuffer->getVertexDeclaration()->findElementBySemantic( fb::VES_NORMAL );
            const FBVertexElement *texCoordElem0 =
                vertexBuffer->getVertexDeclaration()->findElementBySemantic( fb::VES_TEXTURE_COORDINATES,
                                                                             0 );
            const FBVertexElement *texCoordElem1 =
                vertexBuffer->getVertexDeclaration()->findElementBySemantic( fb::VES_TEXTURE_COORDINATES,
                                                                             1 );

            u32 fbVertexSize = vertexBuffer->getVertexDeclaration()->getSize();
            u8 *fbVertexDataPtr = (u8 *)vertexBuffer->getVertexData();
            f32 *fbElementData = 0;

            //save positions
            for( u32 j = 0; j < (u32)vertexCount; ++j, fbVertexDataPtr += fbVertexSize )
            {
                posElem->getElementData( fbVertexDataPtr, &fbElementData );
                Vector3F position( fbElementData[0], fbElementData[1], fbElementData[2] );
                positions.push_back( position );
            }

            //save normals
            if( normalElem )
            {
                fbVertexDataPtr = (u8 *)vertexBuffer->getVertexData();
                for( u32 j = 0; j < (u32)vertexCount; ++j, fbVertexDataPtr += fbVertexSize )
                {
                    normalElem->getElementData( fbVertexDataPtr, &fbElementData );
                    Vector3F normal( fbElementData[0], fbElementData[1], fbElementData[2] );
                    normals.push_back( normal );
                }
            }

            //save texcoord0
            if( texCoordElem0 )
            {
                fbVertexDataPtr = (u8 *)vertexBuffer->getVertexData();
                for( u32 j = 0; j < (u32)vertexCount; ++j, fbVertexDataPtr += fbVertexSize )
                {
                    texCoordElem0->getElementData( fbVertexDataPtr, &fbElementData );
                    Vector2F texCoord( fbElementData[0], fbElementData[1] );
                    //texCoord.Y() = 1.0f - texCoord.Y(); //this works don't know why

                    texCoords0.push_back( texCoord );
                }
            }

            //save texcoord1
            if( texCoordElem1 )
            {
                fbVertexDataPtr = (u8 *)vertexBuffer->getVertexData();
                for( u32 j = 0; j < (u32)vertexCount; ++j, fbVertexDataPtr += fbVertexSize )
                {
                    texCoordElem1->getElementData( fbVertexDataPtr, &fbElementData );
                    Vector2F texCoord( fbElementData[0], fbElementData[1] );
                    //texCoord.Y() = 1.0f - texCoord.Y(); //this works don't know why

                    texCoords1.push_back( texCoord );
                }
            }

            //
            //write indices
            //
            u32 *indexDataPtr = (u32 *)indexBuffer->getIndexData();
            for( int indexIdx = (int)indexCount - 1; indexIdx >= 0; indexIdx-- )
            //for(u32 indexIdx=0; indexIdx<indexCount; ++indexIdx)
            {
                u32 indice = *indexDataPtr++;
                indice += indiceOffset;
                indicesAll.push_back( indice );
            }
        }

        //write vertex data
        {
            //save positions
            FCDGeometrySource *source = colladaMesh->AddVertexSource( FUDaeGeometryInput::POSITION );
            source->SetStride( 3 );
            source->SetDaeId( colladaMesh->GetDaeId() + "-positions" );

            source->SetValueCount( totalNumVerts );
            float *data = source->GetData();

            for( u32 j = 0; j < totalNumVerts; ++j )
            {
                Vector3F p = positions[j];

                ( *data++ ) = p.X();
                ( *data++ ) = p.Y();
                ( *data++ ) = p.Z();
            }

            source = colladaMesh->AddSource( FUDaeGeometryInput::NORMAL );
            source->SetStride( 3 );
            source->SetDaeId( colladaMesh->GetDaeId() + "-normals" );

            source->SetValueCount( totalNumVerts );
            data = source->GetData();

            for( u32 j = 0; j < totalNumVerts; ++j )
            {
                Vector3F p = normals[j];

                ( *data++ ) = p.X();
                ( *data++ ) = p.Y();
                ( *data++ ) = p.Z();
            }

            //save texcoord0
            {
                source = colladaMesh->AddSource( FUDaeGeometryInput::TEXCOORD );
                source->SetStride( 2 );
                source->SetDaeId( colladaMesh->GetDaeId() + "-texcoord0" );

                int channelIndex = 0;
                source->SetUserHandle( (void *)(size_t)channelIndex );

                source->SetValueCount( totalNumVerts );
                data = source->GetData();

                for( u32 j = 0; j < (u32)totalNumVerts; ++j )
                {
                    Vector2F p = texCoords0[j];
                    p.Y() = 1.0f - p.Y();  //this works don't know why

                    ( *data++ ) = p.X();
                    ( *data++ ) = p.Y();
                }
            }

            //save texcoord1
            {
                source = colladaMesh->AddSource( FUDaeGeometryInput::TEXCOORD );
                source->SetStride( 2 );
                source->SetDaeId( colladaMesh->GetDaeId() + "-texcoord1" );

                int channelIndex = 1;
                source->SetUserHandle( (void *)(size_t)channelIndex );

                source->SetValueCount( totalNumVerts );
                data = source->GetData();

                for( u32 j = 0; j < (u32)totalNumVerts; ++j )
                {
                    Vector2F p = texCoords1[j];
                    p.Y() = 1.0f - p.Y();  //this works don't know why

                    ( *data++ ) = p.X();
                    ( *data++ ) = p.Y();
                }
            }
        }

        //write polygons
        indiceOffset = 0;
        for( u32 subMeshIdx = 0; subMeshIdx < subMeshList.size(); ++subMeshIdx )
        {
            FBSubMeshPtr subMesh = subMeshList[subMeshIdx];
            const String &materialName = subMesh->getMaterialName();

            FBIndexBufferPtr indexBuffer = subMesh->getIndexBuffer();
            FBVertexBufferPtr vertexBuffer = subMesh->getVertexBuffer();

            u32 vertexCount = vertexBuffer->getNumVerticies();
            u32 indexCount = indexBuffer->getNumIndices();

            //
            //write indices
            //
            Array<u32> indices;
            indices.set_used( indexCount );

            u32 *indexDataPtr = (u32 *)indexBuffer->getIndexData();

            for( int indexIdx = (int)indexCount - 1; indexIdx >= 0; indexIdx-- )
            //for(u32 indexIdx=0; indexIdx<indexCount; ++indexIdx)
            {
                u32 indice = *indexDataPtr++;
                indice += indiceOffset;
                indices[indexIdx] = indice;
            }

            u32 numFaces = indexCount / 3;
            if( numFaces == 0 )
                continue;

            FCDGeometryPolygons *polygons = colladaMesh->AddPolygons();
            FCDGeometryPolygonsInput *vertexInput = polygons->GetInput( 0 );
            vertexInput->ReserveIndexCount( numFaces * 4 );

            if( materialName.length() > 0 )
            {
                polygons->SetMaterialSemantic( materialName.c_str() );
            }

            int idx = 1;
            int sourceCount = colladaMesh->GetSourceCount();

            // Make a list of the polygon inputs to process.
            // Idx 0 is already used by the vertex inputs
            FCDGeometryPolygonsInputList inputs;
            inputs.reserve( 8 );
            inputs.push_back( vertexInput );
            for( size_t i = 0; i < sourceCount; ++i )
            {
                FCDGeometrySource *source = colladaMesh->GetSource( i );
                if( colladaMesh->IsVertexSource( source ) )
                    continue;

                FCDGeometryPolygonsInput *input = polygons->AddInput( source, idx++ );
                inputs.push_back( input );
                input->ReserveIndexCount( numFaces * 3 * 4 );
                if( source->GetType() == FUDaeGeometryInput::COLOR ||
                    source->GetType() == FUDaeGeometryInput::TEXCOORD )
                {
                    input->SetSet( (int32)(size_t)source->GetUserHandle() );
                    if( input->GetSet() < 0 )
                    {
                        input->SetSet( MAX_MESHMAPS - input->GetSet() );
                    }
                }
            }

            size_t inputCount = inputs.size();

            // Add the tessellation indices to the inputs
            int indiceIdx = 0;
            for( int i = 0; i < numFaces; ++i )
            {
                polygons->AddFaceVertexCount( 3 );

                int i0 = indiceIdx++;
                int i1 = indiceIdx++;
                int i2 = indiceIdx++;

                // Write out the input indices
                for( size_t k = 0; k < inputCount; ++k )
                {
                    FCDGeometryPolygonsInput *input = inputs[k];
                    FCDGeometrySource *source = input->GetSource();
                    switch( source->GetType() )
                    {
                    case FUDaeGeometryInput::POSITION:
                    case FUDaeGeometryInput::NORMAL:
                    case FUDaeGeometryInput::TEXCOORD:
                    case FUDaeGeometryInput::COLOR:
                    {
                        input->AddIndex( indices[i0] );
                        input->AddIndex( indices[i1] );
                        input->AddIndex( indices[i2] );
                    }
                    break;
                    default:
                    {
                    }
                    }
                }
            }

            indiceOffset += vertexCount;
        }

#endif
    }

    //--------------------------------------------
    Ogre::Mesh *MeshConverter::convertFBMeshToOgreMesh( const String &newMeshName, SmartPtr<IMesh> mesh )
    {
#if FB_GRAPHICS_SYSTEM_OGRENEXT
        if( !mesh )
        {
            return nullptr;
        }

        static int meshIdx = 0;

        auto meshManager = Ogre::v1::MeshManager::getSingletonPtr();
        FB_ASSERT( meshManager );

        auto logManager = Ogre::LogManager::getSingletonPtr();
        FB_ASSERT( logManager );

        //auto hardwareBufferManager = Ogre::v1::HardwareBufferManager::getSingletonPtr();
        //FB_ASSERT(hardwareBufferManager);

        auto defaultHardwareBufferManager = Ogre::v1::DefaultHardwareBufferManager();
        //FB_ASSERT(defaultHardwareBufferManager);

        //Ogre::String meshName = "OutMesh" + Ogre::StringConverter::toString(meshIdx++);
        Ogre::String meshName = newMeshName.c_str();
        auto createOrRetriveMesh =
            meshManager->createOrRetrieve( meshName, Ogre::String( "General" ), true );
        if( !createOrRetriveMesh.second )
        {
            logManager->logMessage( "maxExporter::doMesh resource already exists" );
            logManager->logMessage( "maxExporter::doMesh end" );
            return NULL;
        }

        Ogre::v1::MeshPtr pMesh = createOrRetriveMesh.first.staticCast<Ogre::v1::Mesh>();
        Ogre::v1::VertexData *pData = NULL;

        Ogre::v1::HardwareVertexBufferSharedPtr pBuf;
        Ogre::Real *pReal = NULL;
        Ogre::Vector3 currPos;
        Ogre::Real maxSquaredRadius;
        bool firstVert = true;

        Ogre::Vector3 aabbMin( 1e10, 1e10, 1e10 );
        Ogre::Vector3 aabbMax( -1e10, -1e10, -1e10 );

        auto subMeshList = mesh->getSubMeshes();
        u32 numSubMeshes = subMeshList.size();

        for( u32 subMeshIdx = 0; subMeshIdx < numSubMeshes; ++subMeshIdx )
        {
            auto fbSubMesh = subMeshList[subMeshIdx];

            auto fbVertexBuffer = fbSubMesh->getVertexBuffer();
            auto fbIndexBuffer = fbSubMesh->getIndexBuffer();

            u32 fbVertexCount = fbVertexBuffer->getNumVerticies();
            u32 fbIndexCount = fbIndexBuffer->getNumIndices();

            if( fbVertexCount <= 1 )
            {
                continue;
            }

            if( fbIndexCount <= 1 )
            {
                continue;
            }

            Ogre::String matName = fbSubMesh->getMaterialName().c_str();

            // create subMesh
            auto ogreSubMesh = pMesh->createSubMesh( matName );
            ogreSubMesh->setMaterialName( matName );
            ogreSubMesh->useSharedVertices = false;
            ogreSubMesh->vertexData[0] = new Ogre::v1::VertexData( nullptr );
            ogreSubMesh->indexData[0] = new Ogre::v1::IndexData();

            size_t ogreNumVertices = ogreSubMesh->vertexData[0]->vertexCount = fbVertexCount;
            auto decl = ogreSubMesh->vertexData[0]->vertexDeclaration;

            FB_ASSERT_TRUE( ogreSubMesh->vertexData[0]->vertexCount == 0 );

            auto fbVertexDeclaration = fbVertexBuffer->getVertexDeclaration();
            const auto posElem = fbVertexDeclaration->findElementBySemantic(
                IVertexDeclaration::VertexElementSemantic::VES_POSITION );
            const auto normalElem = fbVertexDeclaration->findElementBySemantic(
                IVertexDeclaration::VertexElementSemantic::VES_NORMAL );
            const auto texCoordElem0 = fbVertexDeclaration->findElementBySemantic(
                IVertexDeclaration::VertexElementSemantic::VES_TEXTURE_COORDINATES, 0 );
            const auto texCoordElem1 = fbVertexDeclaration->findElementBySemantic(
                IVertexDeclaration::VertexElementSemantic::VES_TEXTURE_COORDINATES, 1 );

            unsigned short iBinding = 0;
            u32 offset = 0;
            decl->addElement( iBinding, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION );
            offset += sizeof( f32 ) * 3;

            if( normalElem )
            {
                decl->addElement( iBinding, offset, Ogre::VET_FLOAT3, Ogre::VES_NORMAL );
                offset += sizeof( f32 ) * 3;
            }

            if( texCoordElem0 )
            {
                decl->addElement( iBinding, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES, 0 );
                offset += sizeof( f32 ) * 2;
            }

            if( texCoordElem1 )
            {
                decl->addElement( iBinding, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES, 1 );
                offset += sizeof( f32 ) * 2;
            }

            u32 ogreVertexSize = decl->getVertexSize( 0 );

            // Create new vertex buffer
            auto ogreVertexBuffer = defaultHardwareBufferManager.createVertexBuffer(
                ogreVertexSize, ogreNumVertices, Ogre::v1::HardwareBuffer::HBU_STATIC );

            // Bind buffer
            auto pBind = ogreSubMesh->vertexData[0]->vertexBufferBinding;
            pBind->setBinding( 0, ogreVertexBuffer );

            Ogre::Real *ogreVertexDataPtr = static_cast<Ogre::Real *>(
                ogreVertexBuffer->lock( Ogre::v1::HardwareBuffer::HBL_DISCARD ) );

            u32 fbVertexSize = fbVertexBuffer->getVertexDeclaration()->getSize();
            u8 *fbVertexDataPtr = (u8 *)fbVertexBuffer->getVertexData();
            f32 *fbElementData = 0;

            for( u32 vertIdx = 0; vertIdx < fbVertexCount; ++vertIdx, fbVertexDataPtr += fbVertexSize )
            {
                posElem->getElementData( fbVertexDataPtr, &fbElementData );

                Ogre::Vector3 ogrePosition( fbElementData[0], fbElementData[1], fbElementData[2] );

                // Deal with bounds
                currPos = ogrePosition;
                if( firstVert )
                {
                    aabbMin = aabbMax = currPos;
                    maxSquaredRadius = currPos.squaredLength();
                    firstVert = false;
                }
                else
                {
                    aabbMin.makeFloor( currPos );
                    aabbMax.makeCeil( currPos );
                    maxSquaredRadius = MathF::max( maxSquaredRadius, currPos.squaredLength() );
                }

                if( ogrePosition.x < aabbMin.x )
                    aabbMin.x = ogrePosition.x;
                if( ogrePosition.y < aabbMin.y )
                    aabbMin.y = ogrePosition.y;
                if( ogrePosition.z < aabbMin.z )
                    aabbMin.z = ogrePosition.z;

                if( ogrePosition.x > aabbMax.x )
                    aabbMax.x = ogrePosition.x;
                if( ogrePosition.y > aabbMax.y )
                    aabbMax.y = ogrePosition.y;
                if( ogrePosition.z > aabbMax.z )
                    aabbMax.z = ogrePosition.z;

                *ogreVertexDataPtr++ = ogrePosition.x;
                *ogreVertexDataPtr++ = ogrePosition.y;
                *ogreVertexDataPtr++ = ogrePosition.z;

                if( normalElem )
                {
                    normalElem->getElementData( fbVertexDataPtr, &fbElementData );
                    Ogre::Vector3 ogreNormal( fbElementData[0], fbElementData[1], fbElementData[2] );

                    *ogreVertexDataPtr++ = ogreNormal.x;
                    *ogreVertexDataPtr++ = ogreNormal.y;
                    *ogreVertexDataPtr++ = ogreNormal.z;
                }

                if( texCoordElem0 )
                {
                    texCoordElem0->getElementData( fbVertexDataPtr, &fbElementData );
                    *ogreVertexDataPtr++ = fbElementData[0];
                    *ogreVertexDataPtr++ = fbElementData[1];
                }

                if( texCoordElem1 )
                {
                    texCoordElem1->getElementData( fbVertexDataPtr, &fbElementData );
                    *ogreVertexDataPtr++ = fbElementData[0];
                    *ogreVertexDataPtr++ = fbElementData[1];
                }
            }

            ogreVertexBuffer->unlock();

            auto indexData = ogreSubMesh->indexData[0];
            indexData->indexCount = fbIndexCount;
            FB_ASSERT_TRUE( indexData->indexCount == 0 );

            bool useWords = ( fbVertexCount >= std::numeric_limits<u16>::max() ) ? false : true;

            auto ogreIndexBuffer = defaultHardwareBufferManager.createIndexBuffer(
                useWords ? Ogre::v1::HardwareIndexBuffer::IT_16BIT
                         : Ogre::v1::HardwareIndexBuffer::IT_32BIT,
                indexData->indexCount, Ogre::v1::HardwareBuffer::HBU_STATIC_WRITE_ONLY );
            indexData->indexBuffer = ogreIndexBuffer;

            u16 *pWords = useWords ? static_cast<u16 *>(
                                         ogreIndexBuffer->lock( Ogre::v1::HardwareBuffer::HBL_DISCARD ) )
                                   : NULL;
            u32 *pDWords = useWords ? NULL
                                    : static_cast<u32 *>( ogreIndexBuffer->lock(
                                          Ogre::v1::HardwareBuffer::HBL_DISCARD ) );

            if( fbIndexBuffer->getIndexType() == IIndexBuffer::Type::IT_32BIT )
            {
                const u32 *fbIndexData = reinterpret_cast<u32 *>( fbIndexBuffer->getIndexData() );

                //for (s32 i = (s32)fbIndexCount - 1; i >= 0; --i)
                for( u32 i = 0; i < fbIndexCount; ++i )
                {
                    auto index = fbIndexData[i];
                    if( useWords )
                        *pWords++ = index;
                    else
                        *pDWords++ = index;
                }
            }
            else
            {
                const u16 *fbIndexData = reinterpret_cast<u16 *>( fbIndexBuffer->getIndexData() );
                //for (s32 i = (s32)fbIndexCount - 1; i >= 0; --i)
                for( u32 i = 0; i < fbIndexCount; ++i )
                {
                    auto index = fbIndexData[i];
                    if( useWords )
                        *pWords++ = index;
                    else
                        *pDWords++ = index;
                }
            }

            ogreIndexBuffer->unlock();
        }

        // Set bounds
        if( !subMeshList.empty() )
        {
            pMesh->_setBoundingSphereRadius( Ogre::Math::Sqrt( maxSquaredRadius ) );
            pMesh->_setBounds( Ogre::AxisAlignedBox( aabbMin, aabbMax ) );
        }
        else
        {
            pMesh->_setBoundingSphereRadius( 0.1f );
            pMesh->_setBounds( Ogre::AxisAlignedBox( Ogre::Vector3::UNIT_SCALE * -1.0f,
                                                     Ogre::Vector3::UNIT_SCALE * 1.0f ) );
        }

        /*Ogre::Mesh::LodValueList lodList;
        lodList.push_back(500.0f);
        lodList.push_back(750.0f);
        lodList.push_back(900.0f);
        lodList.push_back(1100.0f);
        pMesh->generateLodLevels(lodList, Ogre::ProgressiveMesh::VertexReductionQuota::VRQ_PROPORTIONAL, 0.2f);*/

        Ogre::v1::MeshSerializer serializer;
        auto fileName = newMeshName + ".mesh";
        serializer.exportMesh( pMesh.get(), fileName );

        meshManager->unload( meshName );
#elif FB_GRAPHICS_SYSTEM_OGRE
        if( !mesh )
        {
            return nullptr;
        }

        static int meshIdx = 0;

        auto meshManager = Ogre::MeshManager::getSingletonPtr();
        FB_ASSERT( meshManager );

        auto logManager = Ogre::LogManager::getSingletonPtr();
        FB_ASSERT( logManager );

        auto hardwareBufferManager = Ogre::HardwareBufferManager::getSingletonPtr();
        FB_ASSERT( hardwareBufferManager );

        auto defaultHardwareBufferManager = Ogre::DefaultHardwareBufferManager::getSingletonPtr();
        FB_ASSERT( defaultHardwareBufferManager );

        //Ogre::String meshName = "OutMesh" + Ogre::StringConverter::toString(meshIdx++);
        Ogre::String meshName = newMeshName.c_str();
        auto createOrRetriveMesh =
            meshManager->createOrRetrieve( meshName, Ogre::String( "General" ), true );
        if( !createOrRetriveMesh.second )
        {
            logManager->logMessage( "maxExporter::doMesh resource already exists" );
            logManager->logMessage( "maxExporter::doMesh end" );
            return NULL;
        }

        Ogre::MeshPtr pMesh = std::static_pointer_cast<Ogre::Mesh>( createOrRetriveMesh.first );
        Ogre::VertexData *pData = NULL;

        Ogre::HardwareVertexBufferSharedPtr pBuf;
        Ogre::Real *pReal = NULL;
        Ogre::Vector3 currPos;
        Ogre::Real maxSquaredRadius;
        bool firstVert = true;

        Ogre::Vector3 aabbMin( 1e10, 1e10, 1e10 );
        Ogre::Vector3 aabbMax( -1e10, -1e10, -1e10 );

        auto subMeshList = mesh->getSubMeshes();
        auto numSubMeshes = subMeshList.size();

        for( size_t subMeshIdx = 0; subMeshIdx < numSubMeshes; ++subMeshIdx )
        {
            auto fbSubMesh = subMeshList[subMeshIdx];

            auto fbVertexBuffer = fbSubMesh->getVertexBuffer();
            auto fbIndexBuffer = fbSubMesh->getIndexBuffer();

            u32 fbVertexCount = fbVertexBuffer->getNumVerticies();
            u32 fbIndexCount = fbIndexBuffer->getNumIndices();

            if( fbVertexCount <= 1 )
            {
                continue;
            }

            if( fbIndexCount <= 1 )
            {
                continue;
            }

            Ogre::String matName = fbSubMesh->getMaterialName().c_str();

            // create subMesh
            Ogre::SubMesh *ogreSubMesh = pMesh->createSubMesh( matName );
            ogreSubMesh->setMaterialName( matName );
            ogreSubMesh->useSharedVertices = false;
            ogreSubMesh->vertexData = new Ogre::VertexData();
            ogreSubMesh->indexData = new Ogre::IndexData();

            size_t ogreNumVertices = ogreSubMesh->vertexData->vertexCount = fbVertexCount;
            Ogre::VertexDeclaration *decl = ogreSubMesh->vertexData->vertexDeclaration;

            FB_ASSERT_TRUE( ogreSubMesh->vertexData->vertexCount == 0 );

            auto fbVertexDeclaration = fbVertexBuffer->getVertexDeclaration();
            const auto posElem = fbVertexDeclaration->findElementBySemantic(
                IVertexDeclaration::VertexElementSemantic::VES_POSITION );
            const auto normalElem = fbVertexDeclaration->findElementBySemantic(
                IVertexDeclaration::VertexElementSemantic::VES_NORMAL );
            const auto texCoordElem0 = fbVertexDeclaration->findElementBySemantic(
                IVertexDeclaration::VertexElementSemantic::VES_TEXTURE_COORDINATES, 0 );
            const auto texCoordElem1 = fbVertexDeclaration->findElementBySemantic(
                IVertexDeclaration::VertexElementSemantic::VES_TEXTURE_COORDINATES, 1 );

            unsigned short iBinding = 0;
            u32 offset = 0;
            decl->addElement( iBinding, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION );
            offset += sizeof( f32 ) * 3;

            if( normalElem )
            {
                decl->addElement( iBinding, offset, Ogre::VET_FLOAT3, Ogre::VES_NORMAL );
                offset += sizeof( f32 ) * 3;
            }

            if( texCoordElem0 )
            {
                decl->addElement( iBinding, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES, 0 );
                offset += sizeof( f32 ) * 2;
            }

            if( texCoordElem1 )
            {
                decl->addElement( iBinding, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES, 1 );
                offset += sizeof( f32 ) * 2;
            }

            u32 ogreVertexSize = (u32)decl->getVertexSize( 0 );

            // Create new vertex buffer
            auto ogreVertexBuffer = hardwareBufferManager->createVertexBuffer(
                ogreVertexSize, ogreNumVertices, Ogre::HardwareBuffer::HBU_STATIC );

            // Bind buffer
            Ogre::VertexBufferBinding *pBind = pBind = ogreSubMesh->vertexData->vertexBufferBinding;
            pBind->setBinding( 0, ogreVertexBuffer );

            Ogre::Real *ogreVertexDataPtr =
                static_cast<Ogre::Real *>( ogreVertexBuffer->lock( Ogre::HardwareBuffer::HBL_DISCARD ) );

            u32 fbVertexSize = fbVertexBuffer->getVertexDeclaration()->getSize();
            u8 *fbVertexDataPtr = (u8 *)fbVertexBuffer->getVertexData();
            f32 *fbElementData = 0;

            for( u32 vertIdx = 0; vertIdx < fbVertexCount; ++vertIdx, fbVertexDataPtr += fbVertexSize )
            {
                posElem->getElementData( fbVertexDataPtr, &fbElementData );

                Ogre::Vector3 ogrePosition( fbElementData[0], fbElementData[1], fbElementData[2] );

                // Deal with bounds
                currPos = ogrePosition;
                if( firstVert )
                {
                    aabbMin = aabbMax = currPos;
                    maxSquaredRadius = currPos.squaredLength();
                    firstVert = false;
                }
                else
                {
                    aabbMin.makeFloor( currPos );
                    aabbMax.makeCeil( currPos );
                    maxSquaredRadius = MathF::max( maxSquaredRadius, currPos.squaredLength() );
                }

                if( ogrePosition.x < aabbMin.x )
                    aabbMin.x = ogrePosition.x;
                if( ogrePosition.y < aabbMin.y )
                    aabbMin.y = ogrePosition.y;
                if( ogrePosition.z < aabbMin.z )
                    aabbMin.z = ogrePosition.z;

                if( ogrePosition.x > aabbMax.x )
                    aabbMax.x = ogrePosition.x;
                if( ogrePosition.y > aabbMax.y )
                    aabbMax.y = ogrePosition.y;
                if( ogrePosition.z > aabbMax.z )
                    aabbMax.z = ogrePosition.z;

                *ogreVertexDataPtr++ = ogrePosition.x;
                *ogreVertexDataPtr++ = ogrePosition.y;
                *ogreVertexDataPtr++ = ogrePosition.z;

                if( normalElem )
                {
                    normalElem->getElementData( fbVertexDataPtr, &fbElementData );
                    Ogre::Vector3 ogreNormal( fbElementData[0], fbElementData[1], fbElementData[2] );

                    *ogreVertexDataPtr++ = ogreNormal.x;
                    *ogreVertexDataPtr++ = ogreNormal.y;
                    *ogreVertexDataPtr++ = ogreNormal.z;
                }

                if( texCoordElem0 )
                {
                    texCoordElem0->getElementData( fbVertexDataPtr, &fbElementData );
                    *ogreVertexDataPtr++ = fbElementData[0];
                    *ogreVertexDataPtr++ = fbElementData[1];
                }

                if( texCoordElem1 )
                {
                    texCoordElem1->getElementData( fbVertexDataPtr, &fbElementData );
                    *ogreVertexDataPtr++ = fbElementData[0];
                    *ogreVertexDataPtr++ = fbElementData[1];
                }
            }

            ogreVertexBuffer->unlock();

            Ogre::IndexData *indexData = ogreSubMesh->indexData;
            indexData->indexCount = fbIndexCount;
            FB_ASSERT_TRUE( ogreSubMesh->indexData->indexCount == 0 );

            bool useWords = ( fbVertexCount >= std::numeric_limits<u16>::max() ) ? false : true;

            auto ogreIndexBuffer = defaultHardwareBufferManager->createIndexBuffer(
                useWords ? Ogre::HardwareIndexBuffer::IT_16BIT : Ogre::HardwareIndexBuffer::IT_32BIT,
                ogreSubMesh->indexData->indexCount, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY );
            ogreSubMesh->indexData->indexBuffer = ogreIndexBuffer;

            u16 *pWords =
                useWords
                    ? static_cast<u16 *>( ogreIndexBuffer->lock( Ogre::HardwareBuffer::HBL_DISCARD ) )
                    : NULL;
            u32 *pDWords =
                useWords
                    ? NULL
                    : static_cast<u32 *>( ogreIndexBuffer->lock( Ogre::HardwareBuffer::HBL_DISCARD ) );

            if( fbIndexBuffer->getIndexType() == IIndexBuffer::Type::IT_32BIT )
            {
                const u32 *fbIndexData = reinterpret_cast<u32 *>( fbIndexBuffer->getIndexData() );

                //for (s32 i = (s32)fbIndexCount - 1; i >= 0; --i)
                for( u32 i = 0; i < fbIndexCount; ++i )
                {
                    auto index = fbIndexData[i];
                    if( useWords )
                        *pWords++ = index;
                    else
                        *pDWords++ = index;
                }
            }
            else
            {
                const u16 *fbIndexData = reinterpret_cast<u16 *>( fbIndexBuffer->getIndexData() );
                //for (s32 i = (s32)fbIndexCount - 1; i >= 0; --i)
                for( u32 i = 0; i < fbIndexCount; ++i )
                {
                    auto index = fbIndexData[i];
                    if( useWords )
                        *pWords++ = index;
                    else
                        *pDWords++ = index;
                }
            }

            ogreIndexBuffer->unlock();
        }

        // Set bounds
        if( !subMeshList.empty() )
        {
            pMesh->_setBoundingSphereRadius( Ogre::Math::Sqrt( maxSquaredRadius ) );
            pMesh->_setBounds( Ogre::AxisAlignedBox( aabbMin, aabbMax ) );
        }
        else
        {
            pMesh->_setBoundingSphereRadius( 0.1f );
            pMesh->_setBounds( Ogre::AxisAlignedBox( Ogre::Vector3::UNIT_SCALE * -1.0f,
                                                     Ogre::Vector3::UNIT_SCALE * 1.0f ) );
        }

        /*Ogre::Mesh::LodValueList lodList;
        lodList.push_back(500.0f);
        lodList.push_back(750.0f);
        lodList.push_back(900.0f);
        lodList.push_back(1100.0f);
        pMesh->generateLodLevels(lodList, Ogre::ProgressiveMesh::VertexReductionQuota::VRQ_PROPORTIONAL, 0.2f);*/

        Ogre::MeshSerializer serializer;
        auto fileName = newMeshName + ".mesh";
        serializer.exportMesh( pMesh.get(), fileName );

        meshManager->unload( meshName );

        return pMesh.get();
#else
        return NULL;
#endif
    }

    //--------------------------------------------
    Ogre::Mesh *MeshConverter::convertIrrToOgreMesh( irr::scene::IAnimatedMesh *mesh )
    {
        return 0;
    }

    //--------------------------------------------
    Ogre::Mesh *MeshConverter::convertIrrToOgreMesh( irr::scene::IMesh *mesh )
    {
        return 0;
    }

    //--------------------------------------------
    irr::scene::IAnimatedMesh *MeshConverter::convertOgreToIrrMesh( Ogre::Mesh *mesh )
    {
        return 0;
    }

    //--------------------------------------------
    SmartPtr<IMesh> MeshConverter::convertOgreMeshToFBMesh( Ogre::Mesh *mesh )
    {
        //SmartPtr<IMesh> fbMesh(new FBMesh, true);

        //u32 numSubMeshes = mesh->getNumSubMeshes();
        //for (u32 subMeshIdx = 0; subMeshIdx < numSubMeshes; ++subMeshIdx)
        //{
        //	Ogre::SubMesh* subMesh = mesh->getSubMesh(subMeshIdx);

        //	FBSubMeshPtr fbSubMesh(new FBSubMesh, true);
        //	fbMesh->addSubMesh(fbSubMesh);

        //	String materialName(subMesh->getMaterialName().c_str());
        //	fbSubMesh->setMaterialName(materialName);

        //	//ogre mesh data
        //	Ogre::VertexDeclaration* ogreVertexDec = subMesh->vertexData->vertexDeclaration;
        //	const Ogre::VertexElement* posElem = ogreVertexDec->findElementBySemantic(Ogre::VES_POSITION);
        //	const Ogre::VertexElement* normalElem = ogreVertexDec->findElementBySemantic(Ogre::VES_NORMAL);
        //	const Ogre::VertexElement* texCoordElem0 = ogreVertexDec->findElementBySemantic(Ogre::VES_TEXTURE_COORDINATES, 0);
        //	const Ogre::VertexElement* texCoordElem1 = ogreVertexDec->findElementBySemantic(Ogre::VES_TEXTURE_COORDINATES, 1);

        //	//create sub mesh data
        //	FBVertexDecPtr vertexDeclaration(new FBVertexDeclaration, true);
        //	vertexDeclaration->addElement(sizeof(Vector3F), VES_POSITION, VET_FLOAT3);
        //	vertexDeclaration->addElement(sizeof(Vector3F), VES_NORMAL, VET_FLOAT3);
        //	vertexDeclaration->addElement(sizeof(Vector2F), VES_TEXTURE_COORDINATES, VET_FLOAT2, 0);
        //	if (texCoordElem1)
        //		vertexDeclaration->addElement(sizeof(Vector2F), VES_TEXTURE_COORDINATES, VET_FLOAT2, 1);

        //	FBVertexBufferPtr vertexBuffer(new FBVertexBuffer, true);
        //	vertexBuffer->setVertexDeclaration(vertexDeclaration);

        //	u32 numVertices = subMesh->vertexData->vertexCount;
        //	vertexBuffer->setNumVerticies(numVertices);

        //	f32* vertexData = (f32*)vertexBuffer->createVertexData();
        //	f32* vertexDataPtr = vertexData;

        //	Ogre::VertexBufferBinding* pBind = subMesh->vertexData->vertexBufferBinding;
        //	u32 bufferCount = pBind->getBufferCount();
        //	if (bufferCount > 1)
        //	{
        //		LOG_MESSAGE("MeshConverter", "Buffer count greater than 1");
        //		return SmartPtr<IMesh>(); //not handled return null mesh
        //	}

        //	Ogre::HardwareVertexBufferSharedPtr vbuf = pBind->getBuffer(0);
        //	Ogre::Real* ogreVertexData = static_cast<Ogre::Real*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
        //	Ogre::Real* ogreVertexDataPtr = ogreVertexData;
        //	u32 vertexIncrement = vbuf->getVertexSize() / sizeof(Ogre::Real);

        //	for (u32 vertIdx = 0; vertIdx < numVertices; ++vertIdx, ogreVertexDataPtr += vertexIncrement)
        //	{
        //		f32* pReal = 0;

        //		posElem->baseVertexPointerToElement(ogreVertexDataPtr, &pReal);
        //		Vector3F position(pReal[0], pReal[1], pReal[2]);
        //		*vertexDataPtr++ = position.X();
        //		*vertexDataPtr++ = position.Y();
        //		*vertexDataPtr++ = position.Z();

        //		normalElem->baseVertexPointerToElement(ogreVertexDataPtr, &pReal);
        //		Vector3F normal(pReal[0], pReal[1], pReal[2]);
        //		*vertexDataPtr++ = normal.X();
        //		*vertexDataPtr++ = normal.Y();
        //		*vertexDataPtr++ = normal.Z();

        //		texCoordElem0->baseVertexPointerToElement(ogreVertexDataPtr, &pReal);
        //		Vector2F texCoord0(pReal[0], pReal[1]);
        //		*vertexDataPtr++ = texCoord0.X();
        //		*vertexDataPtr++ = texCoord0.Y();

        //		if (texCoordElem1)
        //		{
        //			texCoordElem1->baseVertexPointerToElement(ogreVertexDataPtr, &pReal);
        //			Vector2F texCoord1(pReal[0], pReal[1]);
        //			*vertexDataPtr++ = texCoord1.X();
        //			*vertexDataPtr++ = texCoord1.Y();
        //		}
        //	}

        //	vbuf->unlock();

        //	Ogre::IndexData* indexData = subMesh->indexData;
        //	u32 indexCount = indexData->indexCount;
        //	_FB_DEBUG_BREAK_IF(indexCount == 0);

        //	bool useWords = (numVertices >= std::numeric_limits<unsigned short>::max()) ? false : true;

        //	Ogre::HardwareIndexBufferSharedPtr ibuf = subMesh->indexData->indexBuffer;

        //	unsigned short* pWords = useWords ? static_cast<u16*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY)) : NULL;
        //	unsigned int* pDWords = useWords ? NULL : static_cast<u32*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

        //	FBIndexBufferPtr indexBuffer(new FBIndexBuffer, true);
        //	indexBuffer->setNumIndices(indexCount);
        //	u32* fbIndexData = reinterpret_cast<u32*>(indexBuffer->createIndexData());
        //	u32* fbIndexDataPtr = fbIndexData;

        //	for (int i = (int)indexCount - 1; i >= 0; i--)
        //		//for (int i=0; i<indexCount; ++i)
        //	{
        //		if (useWords)
        //			*fbIndexData++ = (u32)pWords[i];
        //		else
        //			*fbIndexData++ = (u32)pDWords[i];
        //	}

        //	ibuf->unlock();

        //	fbSubMesh->setVertexBuffer(vertexBuffer);
        //	fbSubMesh->setIndexBuffer(indexBuffer);
        //}

        //return fbMesh;

        return nullptr;
    }

    //--------------------------------------------
    bool MeshConverter::isMeshValid( Ogre::Mesh *mesh )
    {
        //u32 numSubMeshes = mesh->getNumSubMeshes();
        //for (u32 subMeshIdx = 0; subMeshIdx < numSubMeshes; ++subMeshIdx)
        //{
        //	Ogre::SubMesh* subMesh = mesh->getSubMesh(subMeshIdx);

        //	Array<FBVertex> vertices;

        //	//ogre mesh data
        //	Ogre::VertexDeclaration* ogreVertexDec = subMesh->vertexData->vertexDeclaration;
        //	const Ogre::VertexElement* posElem = ogreVertexDec->findElementBySemantic(Ogre::VES_POSITION);
        //	const Ogre::VertexElement* normalElem = ogreVertexDec->findElementBySemantic(Ogre::VES_NORMAL);
        //	const Ogre::VertexElement* texCoordElem0 = ogreVertexDec->findElementBySemantic(Ogre::VES_TEXTURE_COORDINATES, 0);
        //	const Ogre::VertexElement* texCoordElem1 = ogreVertexDec->findElementBySemantic(Ogre::VES_TEXTURE_COORDINATES, 1);

        //	Ogre::VertexBufferBinding* pBind = subMesh->vertexData->vertexBufferBinding;
        //	u32 bufferCount = pBind->getBufferCount();
        //	if (bufferCount > 1)
        //	{
        //		LOG_MESSAGE("MeshConverter", "Buffer count greater than 1");
        //		return SmartPtr<IMesh>(); //not handled return null mesh
        //	}

        //	Ogre::HardwareVertexBufferSharedPtr vertexBuffer = pBind->getBuffer(0);
        //	Ogre::Real* ogreVertexData = static_cast<Ogre::Real*>(vertexBuffer->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
        //	Ogre::Real* ogreVertexDataPtr = ogreVertexData;
        //	f32* fbElementData = 0;
        //	u32 vertexIncrement = vertexBuffer->getVertexSize() / sizeof(Ogre::Real);

        //	u32 numVertices = subMesh->vertexData->vertexCount;
        //	vertices.set_used(numVertices);

        //	for (u32 vertIdx = 0; vertIdx < numVertices; ++vertIdx, ogreVertexDataPtr += vertexIncrement)
        //	{
        //		FBVertex vertex;

        //		posElem->baseVertexPointerToElement(ogreVertexDataPtr, &fbElementData);
        //		Vector3F position(fbElementData[0], fbElementData[1], fbElementData[2]);
        //		vertex.Position = position;

        //		normalElem->baseVertexPointerToElement(ogreVertexDataPtr, &fbElementData);
        //		Vector3F normal(fbElementData[0], fbElementData[1], fbElementData[2]);
        //		vertex.Normal = normal;

        //		if (texCoordElem0)
        //		{
        //			texCoordElem0->baseVertexPointerToElement(ogreVertexDataPtr, &fbElementData);
        //			Vector2F texCoord0(fbElementData[0], fbElementData[1]);
        //			vertex.TexCoord0 = texCoord0;
        //		}

        //		if (texCoordElem1)
        //		{
        //			texCoordElem1->baseVertexPointerToElement(ogreVertexDataPtr, &fbElementData);
        //			Vector2F texCoord1(fbElementData[0], fbElementData[1]);
        //			vertex.TexCoord1 = texCoord1;
        //		}

        //		vertices[vertIdx] = vertex;
        //	}

        //	//check if vertices are finite
        //	for (u32 i = 0; i < vertices.size(); ++i)
        //	{
        //		FBVertex& vertex = vertices[i];
        //		if (!vertex.isFinite())
        //		{
        //			_FB_DEBUG_BREAK_IF(true);
        //			LOG_MESSAGE("MeshUtil", "Error");
        //		}
        //	}

        //	Ogre::IndexData* indexData = subMesh->indexData;
        //	u32 indexCount = indexData->indexCount;
        //	_FB_DEBUG_BREAK_IF(indexCount == 0);

        //	bool useWords = (numVertices >= std::numeric_limits<unsigned short>::max()) ? false : true;

        //	Ogre::HardwareIndexBufferSharedPtr indexBuffer = subMesh->indexData->indexBuffer;
        //	unsigned short* pWords = useWords ? static_cast<u16*>(indexBuffer->lock(Ogre::HardwareBuffer::HBL_READ_ONLY)) : NULL;
        //	unsigned int* pDWords = useWords ? NULL : static_cast<u32*>(indexBuffer->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

        //	for (int i = (int)indexCount - 1; i >= 0; --i)
        //	{
        //		u32 index = 0;
        //		if (useWords)
        //			index = (u32)pWords[i];
        //		else
        //			index = (u32)pDWords[i];

        //		if (index < 0 || index >= vertices.size())
        //		{
        //			_FB_DEBUG_BREAK_IF(true);
        //			LOG_MESSAGE("MeshUtil", "Error: invalid index.");
        //		}

        //		FBVertex& vertex = vertices[index];
        //		if (!vertex.isFinite())
        //		{
        //			_FB_DEBUG_BREAK_IF(true);
        //			LOG_MESSAGE("MeshUtil", "Error: Invalid vertex.");
        //		}
        //	}
        //}

        return false;
    }

}  // end namespace fb
