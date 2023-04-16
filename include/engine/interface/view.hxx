#pragma once

class IView {
public:
    virtual ~IView() {}

    virtual void initialize() = 0;

    virtual void update() = 0;
    virtual void render() = 0;
};
