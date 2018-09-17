#pragma once

#include "renderable2d.h"

namespace engine { namespace graphics {

	class Sprite : public Renderable2D
	{
	public:
		maths::vec3& position;

		Sprite(float x, float y, float width, float height, const maths::vec4 color);
		Sprite(float x, float y, float width, float height, Texture* texture);

		~Sprite();
	};

} }
