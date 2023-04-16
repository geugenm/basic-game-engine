#pragma once

class IModel {
public:
    virtual ~IModel() {}

    virtual void Initialize() = 0;

    virtual void Update() = 0;
    virtual void Render() = 0;
};
