#include "apps/application.h"
#include "engine/engine.hxx"

#include <iostream>
#include <cmath>
#include <chrono>
#include <thread>

const double PI = 3.14159265358979323846;

class RotatingTorDemo : public Application {
public:
    explicit RotatingTorDemo() {
        const int torus_radius = 10;
        const int torus_thickness = 3;
        const int frame_delay_ms = 100;

        while (true)
        {
            for (int i = 0; i < 360; i++) {
                std::cout << "\033[2J\033[H"; // clear the terminal
                for (int j = -torus_radius; j <= torus_radius; j++) {
                    for (int k = -torus_radius; k <= torus_radius; k++) {
                        double torus_eq =
                            std::pow(std::sqrt(j * j + k * k) - torus_radius,
                                     2) +
                            std::pow(0 - i, 2);
                        if (std::abs(std::sqrt(torus_eq) - torus_thickness) <
                            0.5) {
                            std::cout << "*";
                        } else {
                            std::cout << " ";
                        }
                    }
                    std::cout << std::endl;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(frame_delay_ms));
            }
        }

    }

    ~RotatingTorDemo() override {

    }

    void initialize() override {
    }

    void update() override {
    }

    void render() const override {
    }
};

Application* createApplication() {
    return new RotatingTorDemo;
}

void destroyApplication(Application* application) {
    delete application;
}

