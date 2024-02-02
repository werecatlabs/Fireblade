class 'ResourceSelect'

function ResourceSelect:__init(editorWindow, window, elementId, label)
	print("ResourceSelect constructor called");
	
	self.editorWindow = editorWindow;
	self.parent = window;
	self.parentWindow = nil;
	self.text = nil;
	self.image = nil;
	self.button = nil;
	self.label = nil;
	self.material = nil;
	self.terrain = nil;
	self.index = elementId;	
	
	local applicationManager = IApplicationManager.instance();
	local resourceDatabase = applicationManager:getResourceDatabase();
	local ui = applicationManager:getUI();	
	
	local defaultTexture = resourceDatabase:loadResource("checker.png");
	
	local windowTypeInfo = IUIWindow.typeInfo();
	local buttonTypeInfo = IUIButton.typeInfo();
	local imageTypeInfo = IUIImage.typeInfo();
	local textTypeInfo = IUIText.typeInfo();

	self.parentWindow = ui:addElement(windowTypeInfo);
	self.parentWindow:setSize(Vector2F(100.0, 100.0));
	self.parent:addChild(self.parentWindow);

	self.text = ui:addElement(textTypeInfo); 
	self.text:setText(label);
	self.parentWindow:addChild(self.text);	

	self.image = ui:addElement(imageTypeInfo); 
	self.image:setElementId(elementId);
	self.image:setTexture(defaultTexture);
	self.image:setSize(Vector2F(50.0, 50.0));
	self.parentWindow:addChild(self.image);
	
	self.editorWindow:setDroppable(self.image, true);

	self.button = ui:addElement(buttonTypeInfo);
	self.button:setLabel("Select");
	self.parentWindow:addChild(self.button);
end

function ResourceSelect:__finalize()
	print("ResourceSelect __finalize called");

	if (not(self.parentWindow == nil)) then
		self.parentWindow:setVisible(false, false);		
		self.parentWindow:destroyAllChildren();			
		self.parentWindow = nil;
	end	
end

function ResourceSelect:setLabel(label)
	self.text:setText(label);
end

function ResourceSelect:setSameLine(sameLine)
	self.parentWindow:setSameLine(sameLine);
end

function ResourceSelect:setTerrain(terrain)
    self.terrain = terrain;
end

function ResourceSelect:setMaterial(material)
    local defaultTextureName = "checker.png";

	self.material = material;
	
	if self.material then
		local index = self.index;
		print("index "..index);
		
		local texture = self.material:getTexture(index);
		if texture then
			if self.image then
				self.image:setTexture(texture);
			end
		else
			local applicationManager = IApplicationManager.instance();
			local resourceDatabase = applicationManager:getResourceDatabase();
	
			local defaultTexture = resourceDatabase:loadResource(defaultTextureName);
		
			if self.image then
				self.image:setTexture(defaultTexture);
			end
		end
	else
	    --print("material null");

	    local applicationManager = IApplicationManager.instance();
        local resourceDatabase = applicationManager:getResourceDatabase();

        local defaultTexture = resourceDatabase:loadResource(defaultTextureName);

        if self.image then
            self.image:setTexture(defaultTexture);
        end
	end	
end

function ResourceSelect:setTexture(texture)
    print("ResourceSelect setTexture called");

	if texture then
		if self.image then
			self.image:setTexture(texture);	
		end
	else
		print("texture null");
	end
	
	if self.material then
		if texture then
			self.material:setTexture(texture, self.index);
			self.material:save();		
		end
	else
		print("material null");
	end

	if self.terrain then
		if texture then
			self.terrain:setHeightMap(texture);
		end
	else
		print("terrain null");
	end
end

function ResourceSelect:setIndex(index)
	self.index = index;
end

	