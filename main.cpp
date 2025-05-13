#include <iostream>
#include <string>

#include <SFML/Window/Window.hpp>
#include <SFML/Window/VideoMode.hpp>

#include <X11/Xlib.h>

#include "include/Obj.hpp"
#include "include/VulkanApplication.hpp"

void handle_key_pressed(const sf::Event::KeyPressed* event, sf::Window& window) {
    if (event->code == sf::Keyboard::Key::Escape)
        window.close();
}

void handle_key_released(const sf::Event::KeyReleased* event, sf::Window& window) {

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

    const sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();

    sf::Window window(desktopMode, "Scope", sf::Style::Close, sf::State::Fullscreen);

	std::optional<VulkanApplication> app;

	try {
        app.emplace(verbose, window);
	} catch (std::exception &error) {
		std::cerr << error.what() << std::endl;
		return (1);
	}

    app->wait();

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
            if (event->is<sf::Event::KeyPressed>())
                handle_key_pressed(event->getIf<sf::Event::KeyPressed>(), window);
            if (event->is<sf::Event::KeyReleased>())
                handle_key_released(event->getIf<sf::Event::KeyReleased>(), window);
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
