#pragma once

#include <chrono>

class IView {
public:
    virtual ~IView() {}

    virtual void Initialize() = 0;

    virtual void Update(std::chrono::duration<double> deltaTime) = 0;
    virtual void Render() = 0;
};
