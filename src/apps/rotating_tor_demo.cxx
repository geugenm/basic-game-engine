#include "apps/application.h"
#include "engine/engine.hxx"

#include <chrono>
#include <cmath>
#include <iostream>
#include <thread>

const double PI = 3.14159265358979323846;

class RotatingTorDemo : public Application {
public:
    explicit RotatingTorDemo() {
        initialize();
        render();
    }

    ~RotatingTorDemo() override {}

    void initialize() override {
        radius_ = 10;
        thickness_ = 3;
        frameDelayMs_ = 100;
    }

    void update() override {}

    void render() const override {
        for (int i = 0; i < 360; i++) {
            std::cout << "\033[2J\033[H"; // clear the terminal
            for (int j = -radius_; j <= radius_; j++) {
                for (int k = -radius_; k <= radius_; k++) {
                    double torus_eq =
                        std::pow(std::sqrt(j * j + k * k) - radius_,
                                 2) +
                        std::pow(0 - i, 2);
                    if (std::abs(std::sqrt(torus_eq) - thickness_) <
                        0.5) {
                        std::cout << "*";
                    } else {
                        std::cout << " ";
                    }
                }
                std::cout << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(frameDelayMs_));
            std::cout.flush();
        }    }

private:
    int radius_;
    int thickness_;
    int frameDelayMs_;
};

Application* createApplication() {
    return new RotatingTorDemo;
}

void destroyApplication(Application* application) {
    delete application;
}
