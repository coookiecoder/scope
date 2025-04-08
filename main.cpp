#include "include/Obj.hpp"

#include <iostream>
#include <string>

#include <SFML/Window/Window.hpp>
#include <SFML/Window/VideoMode.hpp>

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


    const Obj object(argv[1]);
    const bool verbose = argc == 3 && std::string(argv[2]) == "--verbose";

    if (verbose) {
        std::cout << "Data loaded : " << std::endl;
        std::cout << object << std::endl;
    }

    const sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();

    sf::Window window(desktopMode, "Scope", sf::Style::Close, sf::State::Fullscreen);

    VulkanApplication app(verbose, window);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
            if (event->is<sf::Event::KeyPressed>())
                handle_key_pressed(event->getIf<sf::Event::KeyPressed>(), window);
            if (event->is<sf::Event::KeyReleased>())
                handle_key_released(event->getIf<sf::Event::KeyReleased>(), window);
        }
    }


    return 0;
}