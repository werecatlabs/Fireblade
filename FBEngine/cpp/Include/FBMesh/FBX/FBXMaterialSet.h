#ifndef _MATERIALSET_H
#define _MATERIALSET_H


#include <FBMesh/FBMeshPrerequisites.h>
#include <FBCore/Base/Singleton.h>
#include "material.h"
#include "FxOgreFBXLog.h"



namespace fb
{
    class FBXMaterialSet : public Singleton<FBXMaterialSet>
    {
    public:
        //constructor
        FBXMaterialSet();
        
        //destructor
        ~FBXMaterialSet();

        //clear
        void clear();

        //add material
        void addMaterial(Material* pMat);
        
        //get material
        Material* getMaterial(const std::string& name);
        
        //get default material
        Material* getDefaultMaterial();
        
        //get material set
        static FBXMaterialSet& getSingleton();
        static FBXMaterialSet* getSingletonPtr();

        //write materials to Ogre XML
        bool writeOgreScript(ParamList &params);
        
        // Copy textures to path specified by params
        void copyTextures(ParamList &params);

    protected:
        std::vector<Material*> m_materials;
        Material* m_pDefaultMat;
    };

};	//end namespace

#endif
