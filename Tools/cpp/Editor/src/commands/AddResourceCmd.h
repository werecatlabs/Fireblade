//
// Created by Zane Desir on 11/11/2021.
//

#ifndef FB_ADDRESOURCECMD_H
#define FB_ADDRESOURCECMD_H

#include <GameEditorPrerequisites.h>
#include <FBCore/Interface/System/ICommand.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Atomics/AtomicValue.h>
#include <commands/Command.h>

namespace fb
{
    namespace editor
    {
        class AddResourceCmd : public Command
        {
        public:
            enum class ResourceType
            {
                None,
                Script,
                Material,
                Scene,
                Director,
            };

            AddResourceCmd();
            ~AddResourceCmd() override;

            void redo() override;
            void execute() override;
            void undo() override;

            String getFilePath() const;
            void setFilePath( const String &filePath );

            ResourceType getResourceType() const;
            void setResourceType( ResourceType resourceType );

            SmartPtr<ISharedObject> getResource() const;

            void setResource( SmartPtr<ISharedObject> resource );

        protected:
            SmartPtr<ISharedObject> m_resource;
            AtomicValue<String> m_filePath;
            AtomicValue<ResourceType> m_resourceType = ResourceType::None;
        };
    }  // end namespace editor
}  // end namespace fb

#endif  // FB_ADDRESOURCECMD_H
