#include "FBLuabind/FBLuabindPCH.h"
#include "FBLuabind/Bindings/Vector3Bind.h"
#include <luabind/luabind.hpp>
#include "FBLuabind/SmartPtrConverter.h"
#include "FBLuabind/ParamConverter.h"
#include <FBCore/FBCore.h>
#include "luabind/operator.hpp"
#include <tinyxml.h>

namespace fb
{

    TiXmlNode *_FirstChild( TiXmlNode *node )
    {
        return node->FirstChild();
    }

    TiXmlNode *_FirstChildNamed( TiXmlNode *node, const char *name )
    {
        return node->FirstChild( name );
    }

    TiXmlNode *_Parent( TiXmlNode *node )
    {
        return node->Parent();
    }

    void *_GetUserData( TiXmlBase *base )
    {
        return base->GetUserData();
    }

    bool _SaveFile( TiXmlDocument *doc )
    {
        return doc->SaveFile();
    }

    bool _SaveFileName( TiXmlDocument *doc, const char *name )
    {
        return doc->SaveFile( name );
    }

    bool _LoadFile( TiXmlDocument *doc, const char *name )
    {
        return doc->LoadFile( name );
    }

    void bindXML( lua_State *L )
    {
        using namespace luabind;

        module( L )[class_<TiXmlBase>( "TiXmlBase" )
                        .def( "Row", &TiXmlBase::Row )
                        .def( "Column", &TiXmlBase::Column )
                        .def( "SetUserData", &TiXmlBase::SetUserData )
                        .def( "GetUserData", _GetUserData )];

        module(
            L )[class_<TiXmlNode, TiXmlBase>( "TiXmlNode" )
                    .def( "Value", &TiXmlNode::Value )
                    .def( "SetValue", ( void( TiXmlNode::* )( const char * ) ) & TiXmlNode::SetValue )
                    .def( "Clear", &TiXmlNode::Clear )
                    .def( "Parent", _Parent )
                    .def( "FirstChild", _FirstChild )
                    .def( "FirstChild", _FirstChildNamed )];

        module( L )[class_<TiXmlElement, TiXmlNode>( "TiXmlElement" )];

        module( L )[class_<TiXmlDocument, TiXmlNode>( "TiXmlDocument" )
                        .def( "SaveFile", _SaveFile )
                        .def( "SaveFile", _SaveFileName )
                        .def( "LoadFile", _LoadFile )];
    }

}  // end namespace fb
