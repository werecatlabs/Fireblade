#ifndef XmlUtil_h__
#define XmlUtil_h__

#include <FBCore/Base/Properties.h>
#include <FBCore/Base/DataBinding.h>
#include <FBCore/Base/DataMacros.h>
#include <FBCore/Base/Array.h>
#include <tinyxml.h>

namespace fb
{
    class XmlUtil
    {
    public:
        template <class X>
        static void bind( TiXmlElement *xmle, bool from, Array<X> *ANI, const char *tagname )
        {
            if( from )
            {
                for( TiXmlElement *child = xmle->FirstChildElement( tagname ); child;
                     child = child->NextSiblingElement( tagname ) )
                {
                    X ani;
                    TXB_binding( &ani, child, 1 );
                    ANI->push_back( ani );
                }
            }
            else
            {
                for( size_t i = 0; i < ANI->size(); i++ )
                {
                    X *ani = &ANI->at( i );

                    TiXmlElement Xani( tagname );
                    TXB_binding( ani, &Xani, 0 );
                    xmle->InsertEndChild( Xani );
                }
            }
        }
    };
}  // end namespace fb

#endif  // XmlUtil_h__
