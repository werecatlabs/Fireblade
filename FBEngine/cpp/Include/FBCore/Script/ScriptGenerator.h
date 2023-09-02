#ifndef ScriptGenerator_h__
#define ScriptGenerator_h__

#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/Array.h>
#include "FBCore/Script/ScriptClass.h"
#include "FBCore/Script/ScriptFunction.h"

namespace fb
{
    
    class ScriptGenerator
    {
    public:
        ScriptGenerator();
        ~ScriptGenerator();

        String getClassCPP( const String &parent );

        void convertCSharp( const String &csharpPath, const String &cppPath );

        String cleanString( const String &str );
        void extract_complete_lines( String buf, Array<String> &lines );
        String removeComments( const String &input );
        bool isCSharpAttrib( const String &line );
        bool isCSharpVariable( const String &line );
        bool isCSharpFunction( const String &line );
        String getFunctionName( const String &line );
        Array<SmartPtr<ScriptFunction>> getScriptFunctions( const String &filePath,
                                                            const String &className );
        void createSource( SmartPtr<IFolderExplorer> folderListing );

        void createSource( const String &filePath );

        void createStruct( const String &line, const String &filePath );

        void createClass( const String &line, const String &filePath );

        String createClass( const String &className );
        String createFunction( const String &className, const String &functionName,
                               const Array<String> &parameters );

        void createCPlusPlusScript( const String &path, const String &className );

        void writeScriptFunctions( Array<SmartPtr<ScriptFunction>> &scriptFunctions,
                                   std::ostream &stream );

        void createClassFunctions( Array<SmartPtr<ScriptFunction>> &scriptFunctions,
                                   const String &className );

        String getReplaceFileName() const;
        void setReplaceFileName( const String &val );

        String getReplacementFileName() const;
        void setReplacementFileName( const String &val );

        void createCPlusPlusClass( const String &path, const String &className );

        void createCPlusPlusClass( const String &path, SmartPtr<ScriptClass> pClass );

        Array<String> getHeaderIncludes() const;
        void setHeaderIncludes( const Array<String> &val );

        Array<String> getSourceIncludes() const;
        void setSourceIncludes( const Array<String> &val );

        String getProjectPath() const;
        void setProjectPath( const String &val );

        Array<String> getNamespaceNames() const;
        void setNamespaceNames( const Array<String> &val );

        void addMapEntry( const String &className, const String &newClassName );

    private:
        String m_projectPath;
        String m_replacementFileName;

        String m_sourcePath;
        String m_destinationPath;

        String m_replaceFileName;

        String m_precompiledHeader;

        Array<String> m_headerIncludes;
        Array<String> m_sourceIncludes;

        Array<std::pair<String, String>> m_classMap;

        Array<String> m_namespaceNames;
    };
}  // end namespace fb

#endif  // ScriptGenerator_h__
