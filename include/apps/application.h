#ifndef BASIC_GAME_ENGINE_APPLICATION_H
#define BASIC_GAME_ENGINE_APPLICATION_H

struct Application {
    virtual ~Application() = default;
    virtual void initialize() = 0;
    virtual void update() = 0;
    virtual void render() const = 0;
};

extern "C" Application* createApplication();
extern "C" void destroyApplication(Application* application);

Application* reloadApplicationLibrary(const Application* currentApplication,
                                      void*& currentHandle);

#endif // BASIC_GAME_ENGINE_APPLICATION_H
