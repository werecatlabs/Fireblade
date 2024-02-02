class 'TerrainEditor' (BaseEditor)

TerrainEditorTypes = 
{
	None = 0,
	HeightMap = 1,
	AddLayer	= 2,
	RemoveLayer = 3,
	AddTreeLayer	= 4,
	RemoveTreeLayer = 5,
	AddGrassLayer	= 6,
	RemoveGrassLayer = 7,

	diffuseMap = 0,
	splatMap = 1,

	detail0 = 2,
	detail1 = 3,
	detail2 = 4,
	detail3 = 5,

	detail0NormalMap = 6,
	detail1NormalMap = 7,
	detail2NormalMap = 8,
	detail3NormalMap = 9,
}

function TerrainEditor:__init(window)
	print("TerrainEditor constructor called");
	
	self.window = window;
	
	-- the selected terrain component
	self.terrain = nil; 

	self.editorWindow = nil;
	self.heightMap = nil;
	self.albedoText = nil;
	
	self.addLayerButton = nil;
	self.removeLayerButton = nil;

	self.addTreeLayerButton = nil;
	self.removeTreeLayerButton = nil;

	self.addGrassLayerButton = nil;
	self.removeGrassLayerButton = nil;

	self.terrainTreeLayersWindow = nil;

	self.diffuseMap = nil;
	self.splatMap = nil;
	self.detail0 = nil;
    self.detail1 = nil;
    self.detail2 = nil;

    self.diffuseNormalMap = nil;
    self.detail0NormalMap = nil;
    self.detail1NormalMap = nil;
    self.detail2NormalMap = nil;
end

function TerrainEditor:populateTreeLayers()
	print("TerrainEditor populateTreeLayers start");

	local collapsingHeaderTypeInfo = IUICollapsingHeader.typeInfo();
end

function TerrainEditor:load()
	print("TerrainEditor load start");
	
	local windowTypeInfo = IUIWindow.typeInfo();
	local dropdownTypeInfo = IUIDropdown.typeInfo();
	local buttonTypeInfo = IUIButton.typeInfo();
	local imageTypeInfo = IUIImage.typeInfo();
	local textTypeInfo = IUIText.typeInfo();
	local checkboxTypeInfo = IUICheckbox.typeInfo();
	local tabBarTypeInfo = IUITabBar.typeInfo();
	local collapsingHeaderTypeInfo = IUICollapsingHeader.typeInfo();
	
	local applicationManager = IApplicationManager.instance();
	local ui = applicationManager:getUI();
	
	local parent = self.window:getParent();
	
	local parentWindow = self.window:getParentWindow();
	parentWindow:setSize(Vector2F(500.0, 300.0));
	
	local editorWindow = ui:addElement(windowTypeInfo);
	editorWindow:setSize(Vector2F(500.0, 300.0));
	
	self.editorWindow = editorWindow;
	parentWindow:addChild(self.editorWindow);

	-- Terrain tab bar
	local tabBar = ui:addElement(tabBarTypeInfo);
	
	local heightMapTabItem = tabBar:addTabItem();
	heightMapTabItem:setLabel("Terrain");
	
	local blendMapTabItem = tabBar:addTabItem();
	blendMapTabItem:setLabel("Layers");

	local treesTabItem = tabBar:addTabItem();
    treesTabItem:setLabel("Trees");

	local grassTabItem = tabBar:addTabItem();
    grassTabItem:setLabel("Grass");

    local materialTabItem = tabBar:addTabItem();
    materialTabItem:setLabel("Material");
	
	self.editorWindow:addChild(tabBar);
	
	-- Terrain tab
	self.albedoText = ui:addElement(textTypeInfo); 
	self.albedoText:setText("Height Map");
	self.albedoText:setSameLine(true);
	--heightMapTabItem:addChild(self.albedoText);
	
	self.heightMap = ResourceSelect(self.window, heightMapTabItem, 0, "Height Map");
	self.heightMap:setSameLine(false);
	self.heightMap:setLabel("Height Map");
	
	-- Layers tabs
	self.addLayerButton = ui:addElement(buttonTypeInfo); 
	self.removeLayerButton = ui:addElement(buttonTypeInfo);

	self.addTreeLayerButton = ui:addElement(buttonTypeInfo);
	self.removeTreeLayerButton = ui:addElement(buttonTypeInfo);

	self.addGrassLayerButton = ui:addElement(buttonTypeInfo);
	self.removeGrassLayerButton = ui:addElement(buttonTypeInfo);
	
	self.addLayerButton:setLabel("Add");
	self.removeLayerButton:setLabel("Remove");

	self.addTreeLayerButton:setLabel("Add");
	self.removeTreeLayerButton:setLabel("Remove");

	self.addGrassLayerButton:setLabel("Add");
	self.removeGrassLayerButton:setLabel("Remove");
	
	self.addLayerButton:setElementId(TerrainEditorTypes.AddLayer);
	self.removeLayerButton:setElementId(TerrainEditorTypes.RemoveLayer);

	self.addTreeLayerButton:setElementId(TerrainEditorTypes.AddTreeLayer);
	self.removeTreeLayerButton:setElementId(TerrainEditorTypes.RemoveTreeLayer);

	self.addGrassLayerButton:setElementId(TerrainEditorTypes.AddGrassLayer);
	self.removeGrassLayerButton:setElementId(TerrainEditorTypes.RemoveGrassLayer);
	
	self.addLayerButton:setSameLine(false);
	self.removeLayerButton:setSameLine(true);

	self.addTreeLayerButton:setSameLine(false);
	self.removeTreeLayerButton:setSameLine(true);

	self.addGrassLayerButton:setSameLine(false);
	self.removeGrassLayerButton:setSameLine(true);
	
	blendMapTabItem:addChild(self.addLayerButton);
	blendMapTabItem:addChild(self.removeLayerButton);

	treesTabItem:addChild(self.addTreeLayerButton);
	treesTabItem:addChild(self.removeTreeLayerButton);

	grassTabItem:addChild(self.addGrassLayerButton);
	grassTabItem:addChild(self.removeGrassLayerButton);

	self.window:setHandleEvents(self.addLayerButton, true);
	self.window:setHandleEvents(self.removeLayerButton, true);

	self.window:setHandleEvents(self.addTreeLayerButton, true);
	self.window:setHandleEvents(self.removeTreeLayerButton, true);

	self.window:setHandleEvents(self.addGrassLayerButton, true);
	self.window:setHandleEvents(self.removeGrassLayerButton, true);

	-- material tab
	self.splatMap = ResourceSelect(self.window, materialTabItem, TerrainEditorTypes.splatMap, "Splat Map");

	self.diffuseMap = ResourceSelect(self.window, materialTabItem, TerrainEditorTypes.diffuseMap, "Diffuse");

	self.detail0 = ResourceSelect(self.window, materialTabItem, 0, "detail0");
	self.detail0NormalMap = ResourceSelect(self.window, materialTabItem, 0, "detail0NormalMap");

	self.detail1 = ResourceSelect(self.window, materialTabItem, 0, "detail1");
	self.detail1NormalMap = ResourceSelect(self.window, materialTabItem, 0, "detail1NormalMap");

	self.detail2 = ResourceSelect(self.window, materialTabItem, 0, "detail2");
	self.detail2NormalMap = ResourceSelect(self.window, materialTabItem, 0, "detail2NormalMap");

	self.detail3 = ResourceSelect(self.window, materialTabItem, 0, "detail3");
	self.detail3NormalMap = ResourceSelect(self.window, materialTabItem, 0, "detail3NormalMap");

	self.diffuseMap:setSameLine(true);
	self.detail0NormalMap:setSameLine(true);
	self.detail1NormalMap:setSameLine(true);
	self.detail2NormalMap:setSameLine(true);
	self.detail3NormalMap:setSameLine(true);

	local terrainTreeLayersWindow = ui:addElement(windowTypeInfo);
	terrainTreeLayersWindow:setSize(Vector2F(500.0, 300.0));
	self.terrainTreeLayersWindow = terrainTreeLayersWindow;

	treesTabItem:addChild(self.terrainTreeLayersWindow);

	-- test
	local treeLayer = ui:addElement(collapsingHeaderTypeInfo);
	self.terrainTreeLayersWindow:addChild(treeLayer);

    local testButton = ui:addElement(buttonTypeInfo);
	treeLayer:addChild(testButton);

	self:populateTreeLayers();
	
	if self.editorWindow then 
		print("TerrainEditor load editorWindow set");
	else
		print("TerrainEditor load editorWindow null");
	end
	
	print("TerrainEditor load end");
end

function TerrainEditor:unload()
	print("TerrainEditor unload called");
	
	local applicationManager = IApplicationManager.instance();
	local ui = applicationManager:getUI();
	
	if (self.editorWindow) then
		print("TerrainEditor unload self.editorWindow");
	
		self.editorWindow:setVisible(false, false);
		self.editorWindow:destroyAllChildren();
		
		local editorParent = self.editorWindow:getParent();
		if (editorParent) then
			editorParent:removeChild(self.editorWindow);			
		end	
		
		ui:removeElement(self.editorWindow);
		
		self.editorWindow = nil;
	end	
	
	self.editorWindow = nil;
	self.heightMap = nil;
	self.albedoText = nil;
end

function TerrainEditor:show()
	print("TerrainEditor show called");
	
	local parentWindow = self.window:getParentWindow();
	local debugWindow = self.window:getDebugWindow();
	
	--self:unload();
	--self:load();
	
	--if parentWindow then
	--	parentWindow:setVisible(true, false);
	--end
	
	if debugWindow then
		--debugWindow:setVisible(false, false);
		debugWindow:setVisible(true, false); -- for testing
	end
	
	--if self.editorWindow then
	--	self.editorWindow:setVisible(true, false);
	--	print("TerrainEditor show setVisible");
	--end
	
	print("TerrainEditor show end");
end

function TerrainEditor:hide()
	print("TerrainEditor hide called");
	
	local debugWindow = self.window:getDebugWindow();
		
	if debugWindow then
		debugWindow:setVisible(false, false);
	end
	
	--if self.editorWindow then
	--	self.editorWindow:setVisible(false, false);
	--end
	
	--local parentWindow = self.window:getParentWindow();
	--parentWindow:setVisible(false, false);
end

function TerrainEditor:initialise()
end

function TerrainEditor:update()
end

function TerrainEditor:updateSelection()
	print("TerrainEditor updateSelection called");
	
	self.terrain = nil;

	local applicationManager = IApplicationManager.instance();
	local resourceDatabase = applicationManager:getResourceDatabase();
	local selectionManager = applicationManager:getSelectionManager();
	local selection = selectionManager:getSelection();
	
	local materialTypeInfo = IMaterial.typeInfo();
	local fileSelectionTypeInfo = FileSelection.typeInfo();
	local terrainSystemTypeInfo = TerrainSystem.typeInfo();
	
	local size = selection:size();
	for i = 0, size - 1 do
		local item = selection:at(i)
		if item:derived(terrainSystemTypeInfo) then
		    print("TerrainEditor updateSelection set terrain");
			self.terrain = item;

            if self.heightMap then
                print("TerrainEditor updateSelection set heightMap");
			    self.heightMap:setTerrain(self.terrain);
			end
		end
	end
end

function TerrainEditor:handleEvent(parameters, results)
	print("TerrainEditor handleEvent called");

	local applicationManager = IApplicationManager.instance();
	local resourceDatabase = applicationManager:getResourceDatabase();
	
	if (parameters == nil) then 
		print("TerrainEditor parameters nil");
	end 
	
	local eventHash = parameters:at(1);
	local sender = parameters:at(3);
	
	local elementId = sender:getElementId();

	local layers = self.terrain:getSubComponentByIndex(0);
	local treeLayers = self.terrain:getSubComponentByIndex(0);
	local treeLayers = self.terrain:getSubComponentByIndex(0);
	
	if (eventHash == IEvent.handleSelection) then
		if elementId == TerrainEditorTypes.AddLayer then
		    print("TerrainEditor handleEvent AddLayer");
		elseif elementId == TerrainEditorTypes.RemoveLayer then
		    print("TerrainEditor handleEvent RemoveLayer");
		elseif elementId == TerrainEditorTypes.AddTreeLayer then
		    print("TerrainEditor handleEvent AddTreeLayer");

		elseif elementId == TerrainEditorTypes.RemoveTreeLayer then
		    print("TerrainEditor handleEvent RemoveTreeLayer");
		elseif elementId == TerrainEditorTypes.AddGrassLayer then
		    print("TerrainEditor handleEvent AddGrassLayer");
		elseif elementId == TerrainEditorTypes.RemoveGrassLayer then
		    print("TerrainEditor handleEvent RemoveGrassLayer");
		end
	end

    if (eventHash == IEvent.handleDrop) then
		print("TerrainEditor handleDrop called");

		local cjson = require("cjson")

		-- Encode a Lua table as JSON
		--local data = { name = "John Doe", age = 30 }
		--local json = cjson.encode(data)

		local args = parameters:at(2);
		local dataStr = args:at(0);

		print(dataStr)

		if not(dataStr == nil) then
			-- Decode a JSON string into a Lua table
			local decoded = cjson.decode(dataStr)
			print(decoded.filePath) -- Output

			local path = decoded.filePath;

			if StringUtil.contains(path, ".png") or
				StringUtil.contains(path, ".tga") or
				StringUtil.contains(path, ".tiff") or
				StringUtil.contains(path, ".jpeg") or
				StringUtil.contains(path, ".jpg") then
				local texture = resourceDatabase:loadResource(path);

                if self.heightMap then
				    self.heightMap:setTexture(texture);
				end
			end
		end
	end
end
