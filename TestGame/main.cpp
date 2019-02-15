#include "../GameEngineCore/src/Engine.h"

using namespace engine;
using namespace graphics;

#ifdef ENGINE_EMSCRIPTEN
#include <emscripten/emscripten.h>
#endif

#define 	GLFW_KEY_UP      265
#define 	GLFW_KEY_RIGHT   262
#define 	GLFW_KEY_LEFT    263
#define 	GLFW_KEY_DOWN    264

#ifdef ENGINE_EMSCRIPTEN
static void dispatch_main(void* fp)
{
	std::function<void()>* func = (std::function<void()>*)fp;
	(*func)();
}
#endif

class Game : public engine::Engine
{
private:
	Window * window;
	Layer * layer;
	Sprite * sprite;
	Shader * shader;
	graphics::Window* m_Window;
	Timer* m_Timer;
	unsigned int m_FramesPerSecond, m_UpdatesPerSecond;
public:
	Game()
		:m_FramesPerSecond(0), m_UpdatesPerSecond(0)
	{

	}

	const unsigned int getFPS() const { return m_FramesPerSecond; }
	const unsigned int getUPS() const { return m_UpdatesPerSecond; }

	~Game()
	{
		delete m_Window;
		delete m_Timer;
		delete layer;
	}

	void start()
	{
		init();
		run();
	}

	graphics::Window* createWindow(const char *name, int width, int height)
	{
		m_Window = new graphics::Window(name, width, height);
		return m_Window;
	}

	void init() override
	{
		window = createWindow("Test Game", 960, 540);
#ifdef ENGINE_EMSCRIPTEN		
		shader = new Shader("res/shaders/basic.es3.vert", "res/shaders/basic.es3.frag");
#else		
		shader = new Shader("../res/shaders/basic.vert", "../res/shaders/basic.frag");
#endif

		layer = new Layer(new BatchRenderer2D(), shader, maths::mat4::orthographic(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f));
		//layer->add(new Sprite(0.0f, 0.0f, 4.0f, 4.0f, maths::vec4(1.0f, 0.0f, 0.0f, 1.0f)));
#ifdef ENGINE_EMSCRIPTEN				
		sprite = new Sprite(0.0f, 0.0f, 2.0f, 2.0f, maths::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		//sprite = new Sprite(5.0f, 0.0f, 4.0f, 4.0f, new Texture("res/test.png"));
#else			
		sprite = new Sprite(5.0f, 0.0f, 4.0f, 4.0f, maths::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		//sprite = new Sprite(5.0f, 0.0f, 4.0f, 4.0f, new Texture("test.png"));
#endif
		layer->add(sprite);
	}

	void tick() override
	{
		std::cout << getFPS() << " FPS, " << getUPS() << " UPS" << std::endl;
	}

	void update() override
	{
		float speed = 0.2f;
		if (window->isKeyPressed(GLFW_KEY_UP))
			sprite->position.y += speed;
		else if (window->isKeyPressed(GLFW_KEY_DOWN))
			sprite->position.y -= speed;
		if (window->isKeyPressed(GLFW_KEY_RIGHT))
			sprite->position.x += speed;
		else if (window->isKeyPressed(GLFW_KEY_LEFT))
			sprite->position.x -= speed;

		double x, y;
		window->getMousePosition(x, y);
		shader->setUniform2f("light_pos", maths::vec2((float)(x * 32.0f / window->getWidth() - 16.0f), (float)(9.0f - y * 18.0f / window->getHeight())));

	}

	void run() override
	{
		m_Timer = new Timer();
		float timer = 0.0f;
		float updateTimer = 0.0f;
		float updateTick = 1.0f / 60.0f;
		unsigned int frames = 0;
		unsigned int updates = 0;
#ifdef ENGINE_EMSCRIPTEN
		std::function<void()> mainLoop = [&]() {
#else
		while (!m_Window->closed())
		{
#endif
			m_Window->clear();
			if (m_Timer->elapsed() - updateTimer > updateTick)
			{
				update();
				++updates;
				updateTimer += updateTick;
			}

			render();
			++frames;
			m_Window->update();
			if (m_Timer->elapsed() - timer > 1.0f)
			{
				timer += 1.0f;
				m_FramesPerSecond = frames;
				m_UpdatesPerSecond = updates;
				frames = 0;
				updates = 0;
				tick();
			}
#ifdef ENGINE_EMSCRIPTEN
		};
		emscripten_set_main_loop_arg(dispatch_main, &mainLoop, 0, 1);
#else
		}
#endif
	}

	void render() override
	{
		layer->render();
	}

};

int main()
{
	Game game;
	game.start();

	return 0;
}

#if 0

#include <iostream>

#include "window.h"
#include "src/maths/maths.h"
#include "src/graphics/shader.h"
#include "src/utils/timer.h"

#include "src/graphics//buffers/buffer.h"
#include "src/graphics//buffers/indexbuffer.h"
#include "src/graphics//buffers/vertexarray.h"

#include "src/graphics/renderer2d.h"
#include "src/graphics/batchrenderer2D.h"

#include "src/graphics/sprite.h"

#include "src/graphics/layers/tilelayer.h"

#include "src/graphics/texture.h"
#include "src/graphics/layers/group.h"

#include <time.h>

#define BATCH_RENDERER 1
#define TEST_50K_SPRITES 0

#if 1
int main()
{
	using namespace engine;
	using namespace graphics;
	using namespace maths;

	Window window("Main Window ", 960, 540);
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	mat4 ortho = mat4::orthographic(0.0f, 16.0f, 0.0f, 9.0f, -1.0f, 1.0f);

	Shader* s = new Shader("src/shaders/basic.vert", "src/shaders/basic.frag");
	Shader& shader = *s;
	shader.enable();
	shader.setUniform2f("light_pos", vec2(4.0f, 1.5f));

	TileLayer layer(&shader);

	Texture* textures[]
	{
		new Texture("test.png"),
		new Texture("ta.png"),
		new Texture("tb.png")
	};

	for (float y = -9.0f; y < 9.0f; y++)
	{
		for (float x = -16.0f; x < 16.0f; x++)
		{
			//	layer.add(new Sprite(x, y, 0.9f, 0.9f, maths::vec4(rand() % 1000 / 1000.0f, 0, 1, 1)));
			if (rand() % 4 == 0)
				layer.add(new Sprite(x, y, 0.9f, 0.9f, maths::vec4(rand() % 1000 / 1000.0f, 0, 1, 1)));
			else
				layer.add(new Sprite(x, y, 0.9f, 0.9f, textures[rand() % 3]));
		}
	}


	shader.enable();
	shader.setUniformMat4("pr_matrix", maths::mat4::orthographic(-16.f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f));


	Timer time;
	float timer = 0;
	unsigned int frames = 0;

	while (!window.closed())
	{
		window.clear();
		double x, y;
		window.getMousePosition(x, y);
		shader.setUniform2f("light_pos", vec2((float)(x * 32.0f / window.getWidth() - 16.0f), (float)(9.0f - y * 18.0f / window.getHeight())));
		layer.render();

		if (window.isMouseButtonClicked(GLFW_MOUSE_BUTTON_LEFT))
			std::cout << "Clicked" << std::endl;

		window.update();
		++frames;
		if (time.elapsed() - timer > 1.0f)
		{
			timer += 1.0f;
			printf("%d fps\n", frames);
			frames = 0;
		}
	}
	for (int i = 0; i < 3; ++i)
		delete textures[i];

	return 0;
}
#endif

#if 0
int main()
{
	const char* filename = "test.png";

	//image format
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	//pointer to the image, once loaded
	FIBITMAP *dib(0);
	//pointer to the image data
	BYTE* bits(0);
	//image width and height
	unsigned int width(0), height(0);
	//OpenGL's image ID to map to
	GLuint gl_texID;

	//check the file signature and deduce its format
	fif = FreeImage_GetFileType(filename, 0);
	//if still unknown, try to guess the file format from the file extension
	if (fif == FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(filename);
	//if still unkown, return failure
	if (fif == FIF_UNKNOWN)
		return false;

	//check that the plugin has reading capabilities and load the file
	if (FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, filename);
	//if the image failed to load, return failure
	if (!dib)
		return false;

	//retrieve the image data
	bits = FreeImage_GetBits(dib);
	//get the image width and height
	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);
	//if this somehow one of these failed (they shouldn't), return failure
	if ((bits == 0) || (width == 0) || (height == 0))
		return false;

	std::cout << width << ", " << height << std::endl;

	return 0;
}
#endif
#endif