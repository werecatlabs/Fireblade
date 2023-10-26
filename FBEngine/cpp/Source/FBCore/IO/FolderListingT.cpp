#include <FBCore/FBCorePCH.h>
#include <FBCore/IO/FolderListingT.h>

namespace fb
{

    template <class TInterface, class T>
    FolderListingT<TInterface, T>::FolderListingT()
    {
    }

    template <class TInterface, class T>
    FolderListingT<TInterface, T>::~FolderListingT()
    {
    }

    // explicit instantiation
    template class FolderListingT<IFolderExplorer, String>;
    template class FolderListingT<IFolderExplorerW, StringW>;
}  // end namespace fb