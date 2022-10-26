#include <SDL.h>
#include <Window.hpp>
#include <stdexcept>
#include <Game.hpp>
#include <OpenGL.hpp>
#include <Scenes\Main.hpp>

#ifndef FPS6_NO_AUDIO
#include <SDL_mixer.h>
#endif

#ifdef __MINGW32__
// Import WinMain symbol from libSDL2main, so libmingw32 can find it
// This is needed, because when the code doesn't reference it, it won't get loaded
// It also needs to be extern, so the compiler doesn't optimize it away
extern decltype(&WinMain) WinMainFix;
decltype(&WinMain) WinMainFix = &WinMain;
#endif

static SDL_GLContext g_context = nullptr;
static bool g_isMouseDown = false;
static bool g_lastIsMouseDown = false;
static bool g_isRunning = true;
static int g_relX = 0, g_relY = 0;
static int g_mouseX = 0, g_mouseY = 0;
static SDL_Window* g_window = nullptr;

static void PrepareOpenGL() {
	if (!(g_context = SDL_GL_CreateContext(g_window)))
		throw std::runtime_error("failed to initialize OpenGL");

	if (SDL_GL_MakeCurrent(g_window, g_context) != 0)
		throw std::runtime_error("failed to initialize OpenGL");

	#ifdef __glad_h_
	if (!gladLoadGL())
		throw std::runtime_error("failed to initialize OpenGL");
	#endif
}

static void Cleanup() {
	Game::Cleanup();

	SDL_GL_MakeCurrent(g_window, nullptr);

	if (g_context)
		SDL_GL_DeleteContext(g_context);

	if (g_window)
		SDL_DestroyWindow(g_window);

	#ifndef FPS6_NO_AUDIO
	Mix_CloseAudio();
	Mix_Quit();
	#endif
	
	SDL_Quit();
}

int main(int argc, char** argv) {
	try {
		int flags = SDL_INIT_VIDEO;
		
		#ifndef FPS6_NO_AUDIO
		flags |= SDL_INIT_AUDIO;
		#endif
		
		if (SDL_Init(flags) != 0)
			throw std::runtime_error("failed to initialize SDL");

		#ifndef FPS6_NO_AUDIO
		const auto audioFlags = MIX_INIT_OGG;
		
		if ((Mix_Init(audioFlags) & audioFlags) != audioFlags)
			throw std::runtime_error("failed to initialize audio");

		if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) != 0)
			throw std::runtime_error("failed to initialize audio");
		#endif

		g_window = SDL_CreateWindow("fps6", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_OPENGL);// | SDL_WINDOW_FULLSCREEN);

		if (!g_window)
			throw std::runtime_error("failed to initialize window");

		PrepareOpenGL();

		Game::Init();

		while (g_isRunning) {
			SDL_Event ev;

			g_lastIsMouseDown = g_isMouseDown;

			while (SDL_PollEvent(&ev)) {
				switch (ev.type) {
				case SDL_QUIT:
					Window::quit();
					break;
				case SDL_KEYDOWN:
					if (ev.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
						Window::quit();
					else
						Game::keyDown(ev.key.keysym.scancode, ev.key.keysym.sym);

					break;
				case SDL_KEYUP:
					Game::keyUp(ev.key.keysym.scancode, ev.key.keysym.sym);
					break;
				case SDL_MOUSEBUTTONDOWN:
					if (ev.button.button == SDL_BUTTON_LEFT)
						g_isMouseDown = true;
					
					break;
				case SDL_MOUSEBUTTONUP:
					if (ev.button.button == SDL_BUTTON_LEFT)
						g_isMouseDown = false;

					break;
				case SDL_MOUSEMOTION:
					g_relX += ev.motion.xrel;
					g_relY += ev.motion.yrel;

					g_mouseX = ev.motion.x;
					g_mouseY = ev.motion.y;
					break;
				}
			}

			Game::Update(0);
			Game::Render();
			
			#ifdef __EMSCRIPTEN__
			emscripten_sleep(25);
			#else
			SDL_Delay(25);
			#endif
			
			SDL_GL_SwapWindow(g_window);
		}

		Cleanup();
		return 0;
	}
	catch (std::exception& ex) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "fps6", ex.what(), nullptr);
		Cleanup();
		return 1;
	}
}

void Window::quit() {
	g_isRunning = false;
}

void Window::enableMouseWarp(bool enable) {
	g_relX = 0;
	g_relY = 0;

	SDL_SetRelativeMouseMode(enable ? SDL_TRUE : SDL_FALSE);
}

int Window::mouseRelativeX() {
	auto tmp = g_relX;
	g_relX = 0;
	return tmp;
}

int Window::mouseRelativeY() {
	auto tmp = g_relY;
	g_relY = 0;
	return tmp;
}

int Window::mouseX() {
	return g_mouseX;
}

int Window::mouseY() {
	return g_mouseY;
}

bool Window::isMouseDown() {
	return g_isMouseDown;
}

bool Window::wasMousePressed() {
	return g_isMouseDown && !g_lastIsMouseDown;
}

bool Window::wasMouseReleased() {
	return g_lastIsMouseDown && !g_isMouseDown;
}
