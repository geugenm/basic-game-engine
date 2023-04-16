#pragma once

class IView {
public:
    virtual ~IView() = default;

    virtual void initialize() = 0;

    virtual void update() = 0;
    virtual void render() = 0;
};
