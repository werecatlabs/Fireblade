#ifndef DirectoryListing_h__
#define DirectoryListing_h__


#include <FBCore/Interface/IO/IFolderExplorer.h>
#include <FBCore/Interface/IO/IFolderExplorerW.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/Array.h>

namespace fb
{

    template <class TInterface, class T>
    class FolderListingT : public TInterface
    {
    public:
        FolderListingT();
        ~FolderListingT() override;

        T getFolderName() const override
        {
            return m_folderName;
        }

        void setFolderName( const T &folderName ) override
        {
            m_folderName = folderName;
        }

        Array<T> &getFiles() override
        {
            return m_files;
        }

        const Array<T> &getFiles() const override
        {
            return m_files;
        }

        void setFiles( const Array<T> &files ) override
        {
            m_files = files;
        }

        Array<SmartPtr<TInterface>> getSubFolders() const override
        {
            return m_subFolders;
        }

        void setSubFolders( const Array<SmartPtr<TInterface>> &subFolders ) override
        {
            m_subFolders = subFolders;
        }

        void addSubFolder( SmartPtr<TInterface> subFolder ) override
        {
            m_subFolders.push_back( subFolder );
        }

    protected:
        T m_folderName;
        Array<T> m_files;
        Array<SmartPtr<TInterface>> m_subFolders;
    };

    using DirectoryListing = FolderListingT<IFolderExplorer, std::string>;
    using DirectoryListingW = FolderListingT<IFolderExplorerW, StringW>;
}  // namespace fb

#endif  // DirectoryListing_h__
