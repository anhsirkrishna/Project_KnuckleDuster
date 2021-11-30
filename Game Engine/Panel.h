#pragma once
#include "Component.h"

class Texture;

class Panel : public Component {
	Panel();
	void Draw();
	void Toggle();
private:
	SDL_Rect dimensions;
	bool visible;
	Texture* p_texture;
};
/*
Panel = Class{}

function Panel : init(x, y, width, height, borderColor)
self.x = x
self.y = y
self.width = width
self.height = height
self.panelType = "Panel" ..borderColor
self.scaleX = width / gTextures[self.panelType]:getWidth()
self.scaleY = height / gTextures[self.panelType] : getHeight()
self.visible = true
end

function Panel : update(dt)

end

function Panel : render()
love.graphics.draw(gTextures[self.panelType], self.x, self.y, 0, self.scaleX, self.scaleY)
end

function Panel : toggle()
self.visible = not self.visible
end
*/