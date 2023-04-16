#pragma once

class IModel {
public:
    virtual ~IModel() {}

    virtual void initialize() = 0;

    virtual void update() = 0;
    virtual void render() = 0;
};

extern "C" IModel * create_model();
