#pragma once

class IModel {
public:
    virtual ~IModel() = default;

    virtual void initialize() = 0;

    virtual void update() = 0;
    virtual void render() = 0;
    virtual void destroy() = 0;
};
