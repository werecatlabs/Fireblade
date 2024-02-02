class 'MaterialEditor' (BaseEditor)

MaterialEditorTypes = 
{
	MATERIAL_TYPE = 10,	
	
	Albedo	= 0,
	Normal = 1,
	Metallic = 2,
}

function MaterialEditor:__init(window)
	print("MaterialEditor constructor called");
	BaseEditor:__init(window);
	
	self.window = window;

	self.editorWindow = nil;

	self.text = nil;
	
	self.albedoImage = nil;
	self.albedoButton = nil;
	self.albedoText = nil;
	
	self.normalButton = nil;
	self.normalText = nil;
	
	self.material = nil;
	self.materialType = nil;
	
	self.albedo = nil;
	self.normal = nil;
	self.metallic = nil;
	
	self.transparent = nil;
	self.cutout = nil;
	
	self.selectedMaterial = nil;
end

function MaterialEditor:__finalize()
	print("MaterialEditor __finalize called");
	self:unload();

	local parentWindow = self.window:getParentWindow();
	--parentWindow:removeAllChildren();
	
	self.window = nil;
	self.editorWindow = nil;

	self.text = nil;
	
	self.albedoImage = nil;
	self.albedoButton = nil;
	self.albedoText = nil;
	
	self.normalButton = nil;
	self.normalText = nil;
	
	self.material = nil;
	self.materialComponent = nil;
	self.materialType = nil;
	
	self.albedo = nil;
	self.normal = nil;
	self.metallic = nil;
	
	self.transparent = nil;
	self.cutout = nil;

	self.selectedMaterialFile = nil;
	self.selectedMaterial = nil;
end

function MaterialEditor:load()
	print("MaterialEditor load called");
	
	local windowTypeInfo = IUIWindow.typeInfo();
	local dropdownTypeInfo = IUIDropdown.typeInfo();
	local buttonTypeInfo = IUIButton.typeInfo();
	local imageTypeInfo = IUIImage.typeInfo();
	local textTypeInfo = IUIText.typeInfo();
	local textEntryTypeInfo = IUITextEntry.typeInfo();
	local checkboxTypeInfo = IUILabelTogglePair.typeInfo();
	
	local applicationManager = IApplicationManager.instance();
	local ui = applicationManager:getUI();
	
	local parent = self.window:getParent();
	
	local parentWindow = self.window:getParentWindow();
	parentWindow:setSize(Vector2F(500.0, 300.0));
	
	self.editorWindow = ui:addElement(windowTypeInfo);
	self.editorWindow:setSize(Vector2F(500.0, 200.0));
	parentWindow:addChild(self.editorWindow);

	self.selectedMaterialFile = ui:addElement(textEntryTypeInfo);
	self.selectedMaterialFile:setText("None");
	self.window:setDroppable(self.selectedMaterialFile, true);
	self.editorWindow:addChild(self.selectedMaterialFile);

	local options = Parameters();
	
	options:push_back("Metallic");
	options:push_back("Specular");
	options:push_back("UI");
	options:push_back("Custom");
	
	self.selectedMaterial = ui:addElement(dropdownTypeInfo); 
	self.selectedMaterial:setOptions(options:getAsStringArray());
	self.selectedMaterial:setSelectedOption(0);
	self.editorWindow:addChild(self.selectedMaterial);
	
	self.transparent = ui:addElement(checkboxTypeInfo); 
	self.transparent:setLabel("Transparent");
	self.editorWindow:addChild(self.transparent);
	
	self.cutout = ui:addElement(checkboxTypeInfo); 
	self.cutout:setLabel("Cutout");
	self.editorWindow:addChild(self.cutout);

	self.albedoImage = ui:addElement(imageTypeInfo); 
	--self.editorWindow:addChild(self.albedoImage);

	self.albedoButton = ui:addElement(buttonTypeInfo); 
	self.window:setHandleEvents(self.albedoButton, true);
	--self.editorWindow:addChild(self.albedoButton);
	
	self.albedoText = ui:addElement(textTypeInfo); 
	self.albedoText:setText("Albedo");
	self.albedoText:setSameLine(true);
	--self.editorWindow:addChild(self.albedoText);
	
	self.normalButton = ui:addElement(buttonTypeInfo); 
	--self.editorWindow:addChild(self.normalButton);
	
	self.normalText = ui:addElement(textTypeInfo); 
	self.normalText:setText("Normal");
	self.normalText:setSameLine(true);
	--self.editorWindow:addChild(self.normalText);
	
	--local text = ui:addElement(textTypeInfo); 
	--self.text = text; 	
	--text:setText("Hello world");	
	--self.editorWindow:addChild(text);
	
	self.albedo = ResourceSelect(self.window, self.editorWindow, MaterialEditorTypes.Albedo, "Albedo");
	self.albedo:setSameLine(false);
	self.albedo:setLabel("Albedo");
	
	self.normal = ResourceSelect(self.window, self.editorWindow, MaterialEditorTypes.Normal, "Normal");
	self.normal:setSameLine(true);
	self.normal:setLabel("Normal");
	
	self.metallic = ResourceSelect(self.window, self.editorWindow, MaterialEditorTypes.Metallic, "Metallic");
	self.metallic:setSameLine(true);
	self.metallic:setLabel("Metallic");
end

function MaterialEditor:unload()
	print("MaterialEditor unload called");
	
	local applicationManager = IApplicationManager.instance();
	local ui = applicationManager:getUI();
	
	self.albedo = nil;
	self.normal = nil;
	self.metalic = nil;
	
	local parentWindow = self.window:getParentWindow();
	
	if parentWindow then
		parentWindow:setVisible(false, false);
	end	
	
	if self.editorWindow then
		print("MaterialEditor unload self.editorWindow");
	
		self.editorWindow:setVisible(false, false);
		self.editorWindow:destroyAllChildren();
		
		local editorParent = self.editorWindow:getParent();
		if (editorParent) then
			editorParent:removeChild(self.editorWindow);			
		end	
		
		ui:removeElement(self.editorWindow);
		
		self.editorWindow = nil;
	end	
	
	parentWindow:destroyAllChildren();
end

function MaterialEditor:show()
	print("MaterialEditor show called");
	
	local parentWindow = self.window:getParentWindow();
	local debugWindow = self.window:getDebugWindow();
	
	--self:unload();
	--self:load();
	
	--parentWindow:setVisible(true, false);
	debugWindow:setVisible(true, false);
	
	--if self.editorWindow then
	--	self.editorWindow:setVisible(true, false);
	--end
	
	parentWindow:setVisible(false, false); -- todo temp code to hide new ui
end

function MaterialEditor:hide()
	print("MaterialEditor hide called");
	local debugWindow = self.window:getDebugWindow();
	
	debugWindow:setVisible(false, false);
	--self:unload();
	
	--if self.editorWindow then
	--	self.editorWindow:setVisible(false, false);
	--end
	
	--local parentWindow = self.window:getParentWindow();
	--parentWindow:setVisible(false, false);
end

function MaterialEditor:initialise()
end

function MaterialEditor:update()
end

function MaterialEditor:updateSelection()
	print("MaterialEditor updateSelection called");
	
	self.material = nil;
	
	if self.selectedMaterialFile then
		self.selectedMaterialFile:setText("None");
    end

	local applicationManager = IApplicationManager.instance();
	local resourceDatabase = applicationManager:getResourceDatabase();
	local selectionManager = applicationManager:getSelectionManager();
	local selection = selectionManager:getSelection();
	
	local materialComponentTypeInfo = Material.typeInfo();
	local materialTypeInfo = IMaterial.typeInfo();
	local fileSelectionTypeInfo = FileSelection.typeInfo();
	
	local size = selection:size();
	for i = 0, size - 1 do
		local item = selection:at(i);
		
		if item:derived(materialTypeInfo) then 
			self.material = item;
		elseif item:derived(materialComponentTypeInfo) then 
			self.material = item:getMaterial();
			self.materialComponent = item;
		elseif item:derived(fileSelectionTypeInfo) then 
			local path = item:getFilePath();
			if StringUtil.contains(path, ".mat") then
				self.material = resourceDatabase:loadResource(path);
			end
		else
			print("type:"..item:getTypeInfo());
		end
	end
	
	self:updateSelectedMaterial();
end

function MaterialEditor:updateSelectedMaterial()
	if self.material then
	    local filePath = self.material:getFilePath();

	    if self.selectedMaterialFile then
	        self.selectedMaterialFile:setText(filePath);
	    end

		self.materialType = self.material:getMaterialType();
	end
		
    if self.albedo then
        self.albedo:setMaterial(self.material);
    end

    if self.normal then
        self.normal:setMaterial(self.material);
    end

    if self.metallic then
        self.metallic:setMaterial(self.material);
    end
end

function MaterialEditor:handleEvent(parameters, results)
	print("MaterialEditor handleEvent called");
	
	local applicationManager = IApplicationManager.instance();
	local resourceDatabase = applicationManager:getResourceDatabase();
	
	if (parameters == nil) then 
		print("MaterialEditor parameters nil");
	end 
	
	local eventHash = parameters:at(1);
	local sender = parameters:at(3);
	
	local elementId = sender:getElementId();
	
	if (eventHash == IEvent.handleSelection) then
	end
	
	if (eventHash == IEvent.handleDrag) then
		print("MaterialEditor handleDrag called");
	end
	
	if (eventHash == IEvent.handleDrop) then
		print("MaterialEditor handleDrop called");
		
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
				
				if (elementId == MaterialEditorTypes.Albedo) then
					if not(self.albedo == nil) then 
						self.albedo:setTexture(texture);
					end					
				elseif (elementId == MaterialEditorTypes.Normal) then
					self.normal:setTexture(texture);					
				elseif (elementId == MaterialEditorTypes.Metallic) then
					self.metallic:setTexture(texture);
				end
			elseif StringUtil.contains(path, ".mat") then
			    if self.selectedMaterialFile then
            	    self.selectedMaterialFile:setText(path);
            	end
				
				if self.materialComponent then 
					self.materialComponent:setMaterialPath(path);
					self.material = self.materialComponent:getMaterial();
				else 	
					local material = resourceDatabase:loadResource(path);
					self.material = material;
				end
				
				self:updateSelectedMaterial();
			end
		end
	end
end




