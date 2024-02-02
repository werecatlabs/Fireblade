class 'ProceduralScene' (BaseComponent)

function ProceduralScene:__init()
	print("ProceduralScene constructor called");
	self.player = nil
	BaseComponent:__init();
end

function ProceduralScene:loadData(filePath)
	print("ProceduralScene:test called");
	
	local applicationManager = IApplicationManager.instance();
	local sceneManager = applicationManager:getSceneManager();
	local scene = sceneManager:getCurrentScene();	
end