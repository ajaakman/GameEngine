#pragma once

#ifdef ENGINE_EMSCRIPTEN
#include <emscripten/emscripten.h>
#endif

#include "graphics/window.h"
#include "graphics/batchrenderer2D.h"
#include "graphics/renderer2D.h"
#include "graphics/sprite.h"
#include "graphics/layers/layer.h"

#include "maths/maths.h"
#include "utils/timer.h"

namespace engine {

	class Engine
	{
	protected:		
		Engine()			
		{}

		virtual ~Engine()
		{}

	protected:

		// Runs once upon initialization.
		virtual void init() = 0;
		// Runs once per second.
		virtual void tick() = 0;
		// Runs 60 times per second.
		virtual void update() = 0;
		// Runs as fast as possible (unless vsync is enabled).
		virtual void render() = 0;

		virtual void run() = 0;		

	};

}