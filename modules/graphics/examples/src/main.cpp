// std libs
#include <iostream>
#include <string>
// internal
#include "common/driver.hpp"

extern App* getClearAppInstance();
extern App* getTriangleAppInstance();
extern App* getCubeAppInstance();
extern App* getTextureAppInstance();
extern App* getFramebufferAppInstance();
extern App* getMultithreadedAppInstance();


void forceAppLink() {
    (void)getClearAppInstance();
    // (void)getTriangleAppInstance();
    (void)getCubeAppInstance();
    // (void)getTextureAppInstance();
    // (void)getFramebufferAppInstance();
    (void)getMultithreadedAppInstance();

}

int main(int argc, const char* argv[]) {
    // debug
    for (int i = 0; i < argc; i++) {
        std::cout << "[DEBUG] arg[" << i << "] = " << argv[i] << std::endl;
    }
    forceAppLink();

    // declare driver params
    int sceneIndex = 0;
    RendererType rendererType = VGL_RENDERER_OPENGL; // default

    // required args
    if (argc < 2) {
        std::cerr << "[ERROR] Usage: <scene_index> [-r|--renderer noop|opengl|vulkan]" << std::endl;
        return 1;
    }
    try {
        sceneIndex = std::stoi(argv[1]);
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] Invalid scene index: " << argv[1] << std::endl;
        return 2;
    }

    // optional args
    int idx = 2;
    while (idx < argc) {
        std::string arg = argv[idx];
        if (arg == "-r" || arg == "--renderer") {
            if (idx + 1 < argc) {
                std::string rendererValue = argv[idx + 1];
                if (rendererValue == "noop") {
                    rendererType = VGL_RENDERER_NOOP;
                } else if (rendererValue == "opengl") {
                    rendererType = VGL_RENDERER_OPENGL;
                } else if (rendererValue == "vulkan") {
                    rendererType = VGL_RENDERER_VULKAN;
                } else {
                    std::cerr << "[ERROR] Unknown renderer: " << rendererValue << std::endl;
                    return 3;
                }
            } else {
                std::cerr << "[ERROR] Missing renderer value after -r/--renderer" << std::endl;
                return 4;
            }
        } else {
            std::cerr << "[ERROR] Unknown argument: " << arg << std::endl;
            return 4;
        }
        idx += 2; // Move past the argument and its value
    }

    // run driver
    Driver driver = Driver();
	if (!driver.init(sceneIndex, rendererType)) {
		driver.shutdown();
		return -1;
	}
	driver.run();
	driver.shutdown();
	return 0;
}
