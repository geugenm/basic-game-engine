#include "apps/mvp/mvp_application.hxx"
#include "engine.hxx"

class ExampleGame : public AbstractApplication
{
  public:
    explicit ExampleGame()
    {
    }

    ~ExampleGame() override
    {
    }

    void initialize() override
    {
        UIEngineSingleton::get_instance()->initialize();
    }

    void update() override
    {
        UIEngineSingleton::get_instance()->update();
    }

    void render() const override
    {
        UIEngineSingleton::get_instance()->render();
    }
};

AbstractApplication *create_application()
{
    if (UIEngineSingleton::get_instance() != nullptr)
    {
        return new ExampleGame();
    }
    return nullptr;
}

void destroy_application(AbstractApplication *application)
{
    UIEngineSingleton::get_instance()->destroy();
    delete application;
}
