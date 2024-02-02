class 'Road' (BaseComponent)

function Road:__init()
	print("Road constructor called");
	self.player = nil
	BaseComponent:__init();
end

function Road:test()
	print("Road:test called");
end
