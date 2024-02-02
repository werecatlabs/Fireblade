class 'BaseEditor'

function BaseEditor:__init(window)
	print("Application constructor called");
	self.window = window;
	
	self.albedoText = nil;
end

function BaseEditor:__finalize()
	print("BaseEditor __finalize called");
end

function BaseEditor:load()
	print("BaseEditor load called");
	
	local applicationManager = IApplicationManager.instance();
	local ui = applicationManager:getUI();
	
	local parentWindow = self.window:getParentWindow();
	parentWindow:setSize(Vector2F(500.0, 300.0));
	
	self.albedoText = ui:addElement(textTypeInfo); 
	self.albedoText:setText("Albedo");
	self.albedoText:setSameLine(true);
	parentWindow:addChild(self.albedoText);
end

function BaseEditor:unload()
end

function BaseEditor:initialise()
end

function BaseEditor:update()
end

function BaseEditor:show()
end

function BaseEditor:hide()
end