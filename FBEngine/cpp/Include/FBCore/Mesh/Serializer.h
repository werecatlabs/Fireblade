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

#ifndef __FB_Serializer_H__
#define __FB_Serializer_H__

#include <FBMesh/FBMeshPrerequisites.h>
#include <FBCore/Core/StringTypes.h>
#include <FBCore/Math/Vector3.h>
#include <FBCore/Math/Quaternion.h>
#include <boost/shared_ptr.hpp>

namespace fb
{
    /** \addtogroup Core
     *  @{
     */
    /** \addtogroup General
     *  @{
     */
    /** Generic class for serialising data to / from binary stream-based files.
    @remarks
        This class provides a number of useful methods for exporting / importing data
        from stream-oriented binary files (e.g. .mesh and .skeleton).
    */
    class Serializer
    {
    public:
        Serializer();
        virtual ~Serializer();

        /// The endianness of written files
        enum Endian
        {
            /// Use the platform native endian
            ENDIAN_NATIVE,
            /// Use big endian (0x1000 is serialised as 0x10 0x00)
            ENDIAN_BIG,
            /// Use little endian (0x1000 is serialised as 0x00 0x10)
            ENDIAN_LITTLE
        };

    protected:
        u32 mCurrentstreamLen;
        SmartPtr<IStream> mStream;
        String mVersion;
        bool mFlipEndian;  // default to native endian, derive from header

        // Internal methods
        virtual void writeFileHeader( void );
        virtual void writeChunkHeader( u16 id, u32 size );

        void writeFloats( const float *pfloat, u32 count );
        void writeFloats( const double *pfloat, u32 count );
        void writeShorts( const u16 *pShort, u32 count );
        void writeInts( const u32 *pInt, u32 count );
        void writeBools( const bool *pLong, u32 count );
        void writeObject( const Vector3F &vec );
        void writeObject( const QuaternionF &q );

        void writeString( const String &string );
        void writeData( const void *buf, u32 size, u32 count );

        virtual void readFileHeader( SmartPtr<IStream> &stream );
        virtual unsigned short readChunk( SmartPtr<IStream> &stream );

        void readBools( SmartPtr<IStream> &stream, bool *pDest, u32 count );
        void readFloats( SmartPtr<IStream> &stream, float *pDest, u32 count );
        void readFloats( SmartPtr<IStream> &stream, double *pDest, u32 count );
        void readShorts( SmartPtr<IStream> &stream, u16 *pDest, u32 count );
        void readInts( SmartPtr<IStream> &stream, u32 *pDest, u32 count );
        void readObject( SmartPtr<IStream> &stream, Vector3F &pDest );
        void readObject( SmartPtr<IStream> &stream, QuaternionF &pDest );

        String readString( SmartPtr<IStream> &stream );
        String readString( SmartPtr<IStream> &stream, u32 numChars );

        virtual void flipToLittleEndian( void *pData, u32 size, u32 count = 1 );
        virtual void flipFromLittleEndian( void *pData, u32 size, u32 count = 1 );

        virtual void flipEndian( void *pData, u32 size, u32 count );
        virtual void flipEndian( void *pData, u32 size );

        /// Determine the endianness of the incoming stream compared to native
        virtual void determineEndianness( SmartPtr<IStream> &stream );
        /// Determine the endianness to write with based on option
        virtual void determineEndianness( u32 requestedEndian );
    };

    /** @} */
    /** @} */
}  // namespace fb

#endif
