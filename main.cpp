#include <iostream>
#include <string>

#include <SFML/Window/Window.hpp>
#include <SFML/Window/VideoMode.hpp>

#include <X11/Xlib.h>

#include "include/Obj.hpp"
#include "include/MaterialLoader.hpp"
#include "include/VulkanApplication.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"

bool run = true;

void handle_key_pressed(const sf::Event::KeyPressed* event, sf::Window& window, VulkanApplication& app) {
    switch (event->code) {
        default:
            break;
        case sf::Keyboard::Key::Escape:
            run = false;
            break;
        case sf::Keyboard::Key::W:
            app.zoom += 0.1f;
            break;
        case sf::Keyboard::Key::S:
            app.zoom -= 0.1f;
            break;
        case sf::Keyboard::Key::Space:
            app.useTexture = !app.useTexture;
            app.updateTexture = true;
            break;
    }
}

void handle_key_released(const sf::Event::KeyReleased* event, sf::Window& window, VulkanApplication& app) {
    switch (event->code) {
        default:
            break;
    }
}

int main(const int argc, const char *argv[]) {
    if (argc == 1) {
        std::cerr << "Usage: " << argv[0] << " [filename] [option]" << std::endl;
        return 1;
    }

    if (sf::Vulkan::isAvailable(true) == false) {
        std::cerr << "Vulkan is not available" << std::endl;
        return 2;
    }

    if (!XInitThreads()) {
        std::cerr << "Failed to initialize X11 threads!" << std::endl;
        return 3;
    }

    const Obj object(argv[1]);
    const bool verbose = argc == 3 && std::string(argv[2]) == "--verbose";

    if (verbose) {
        std::cout << "Data loaded : " << std::endl;
        std::cout << object << std::endl;
    }

    std::optional<MaterialLoader> material;

    if (object.hasImage()) {
        material.emplace(object.getMaterialPath());
        if (verbose) {
            std::cout << "Material loaded : " << std::endl;
            std::cout << material.value() << std::endl;
        }
    }

    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();

    desktopMode.size.x /= 2;
    desktopMode.size.y /= 2;

    sf::Window window(desktopMode, "Scope", sf::Style::Close, sf::State::Windowed);

	std::optional<VulkanApplication> app;

	try {
	    if (object.hasImage() && material.value().getMaterials()[0].map_Kd.empty() == false)
            app.emplace(verbose, window, material.value().getMaterials()[0].map_Kd, object);
	    else if (object.hasImage())
	        app.emplace(verbose, window, cookie::Vector3D(material.value().getMaterials()[0].Kd[0] * 255.0f, material.value().getMaterials()[0].Kd[1] * 255.0f, material.value().getMaterials()[0].Kd[2] * 255.0f), object);
	    else
	        app.emplace(verbose, window, "", object);
	} catch (std::exception &error) {
	    std::cerr << "creating application failed" << std::endl;
		std::cerr << error.what() << std::endl;
		return (1);
	}

    app->wait();

    while (window.isOpen() && run) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
            if (event->is<sf::Event::KeyPressed>())
                handle_key_pressed(event->getIf<sf::Event::KeyPressed>(), window, app.value());
            if (event->is<sf::Event::KeyReleased>())
                handle_key_released(event->getIf<sf::Event::KeyReleased>(), window, app.value());
            if (event->is<sf::Event::Resized>())
                app->triggerResize();
        }

        try {
            app->drawFrame();
        } catch (std::exception &error) {
            if (window.isOpen() == false)
                app->wait();
            else
                std::cerr << error.what() << std::endl;
            return window.isOpen();
        }
    }

    app->wait();

    return 0;
}
