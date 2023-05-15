////////////////////////////////////////////////////////////////////////////////
// materialSet.cpp
// Author     : Francesco Giordana
// Start Date : January 13, 2005
// Copyright  : (C) 2006 by Francesco Giordana
// Email      : fra.giordana@tiscali.it
////////////////////////////////////////////////////////////////////////////////

/*********************************************************************************
*                                                                                *
*   This program is free software; you can redistribute it and/or modify         *
*   it under the terms of the GNU Lesser General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or            *
*   (at your option) any later version.                                          *
*                                                                                *
**********************************************************************************/
#include <FBMesh/FBMeshPCH.h>
#include "FBXMaterialSet.h"
#include <FBCore/Base/LogManager.h>



namespace fb
{



    template<> FBXMaterialSet* Singleton<FBXMaterialSet>::m_singleton = 0;



	FBXMaterialSet::FBXMaterialSet()
	{
		try
		{
			//create a default material
			m_pDefaultMat = new Material();
			m_pDefaultMat->m_type = MT_LAMBERT;
			m_pDefaultMat->m_name = "defaultLambert";
			m_pDefaultMat->m_ambient = Point4(0, 0, 0, 1);
			m_pDefaultMat->m_emissive = Point4(0, 0, 0, 1);
			m_pDefaultMat->m_diffuse = Point4(0.5f, 0.5f, 0.5f, 1.0f);
		}
		catch (std::exception& e)
		{
			FB_LOG_EXCEPTION(e);
		}
	}

	FBXMaterialSet::~FBXMaterialSet()
	{
		try
		{
			clear();
			
			if (m_pDefaultMat)
			{
				delete m_pDefaultMat;
				m_pDefaultMat = nullptr;
			}
		}
		catch (std::exception& e)
		{
			FB_LOG_EXCEPTION(e);
		}
	}

	void FBXMaterialSet::clear()
	{
		for (size_t i = 0; i < m_materials.size(); i++)
		{
			delete m_materials[i];
		}

		m_materials.clear();
	}

	void FBXMaterialSet::addMaterial(Material* pMat)
	{
		bool found = false;
		for (size_t i = 0; i < m_materials.size() && !found; i++)
		{
			if (!m_materials[i]->name().compare(pMat->name()))
			{
				found = true;
				delete pMat;
			}
		}

		if (!found)
		{
			m_materials.push_back(pMat);
		}
	}

	Material* FBXMaterialSet::getMaterial(const std::string& name)
	{
		try
		{
			for (size_t i = 0; i < m_materials.size(); i++)
			{
				if (!m_materials[i]->name().compare(name))
					return m_materials[i];
			}

			return nullptr;
		}
		catch (std::exception& e)
		{
			FB_LOG_EXCEPTION(e);
		}

		return nullptr;
	}

	fb::Material* FBXMaterialSet::getDefaultMaterial()
	{
		return m_pDefaultMat;
	}

	fb::FBXMaterialSet& FBXMaterialSet::getSingleton()
	{
		assert(m_singleton);
		return (*m_singleton);
	}

	fb::FBXMaterialSet* FBXMaterialSet::getSingletonPtr()
	{
		return m_singleton;
	}

	bool FBXMaterialSet::writeOgreScript(ParamList& params)
	{
		bool stat;
		for (size_t i = 0; i < m_materials.size(); i++)
		{
			stat = m_materials[i]->writeOgreScript(params);

			if (true != stat)
			{
				std::string msg = "Error writing material ";
				msg += m_materials[i]->name();
				msg += ", aborting operation\n";
				FxOgreFBXLog(msg.c_str());
			}

		}
		copyTextures(params);
		return true;
	}

	void FBXMaterialSet::copyTextures(ParamList& params)
	{
		if (params.copyTextures)
		{
			std::vector<std::string> copiedTextures;
			for (size_t matIt = 0; matIt < m_materials.size(); matIt++)
			{

				for (size_t i = 0; i < m_materials[matIt]->m_textures.size(); i++)
				{
					std::string texture = m_materials[matIt]->m_textures[i].absFilename;
					bool bAlreadyCopied = false;
					for (size_t j = 0; j < copiedTextures.size(); ++j)
					{
						if (texture == copiedTextures[j])
						{
							bAlreadyCopied = true;
							break;
						}
					}
					if (!bAlreadyCopied && FileExists(texture))
					{
						copiedTextures.push_back(texture);
						// Copy file texture to output dir
						std::string command = "copy \"";
						command += texture;
						command += "\" \"";
						command += params.texOutputDir;

						if (params.prefixTextures)
						{
							command += params.matPrefix;
						}
						command += m_materials[matIt]->m_textures[i].filename;
						command += "\"";
						FxOgreFBXLog("Copying texture: %s\n", command.c_str());
						if (0 != system(command.c_str()))
						{
							FxOgreFBXLog("Failed to copy texture!\n");
						}
					}
				}
			}
		}
	}

};	//end namespace
