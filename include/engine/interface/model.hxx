#pragma once

namespace Game {

}

class IModel {
public:
    virtual ~IModel() {}

    virtual void initialize() = 0;

    virtual void update() = 0;
    virtual void render() = 0;
};
