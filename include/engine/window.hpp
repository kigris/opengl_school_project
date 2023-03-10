#ifndef window_hpp
#define window_hpp

#include <cstdint>

class GLFWwindow;

class Window {
public:
    static Window* instance;

    [[nodiscard]] bool isAlive() const;
    void frame() const;

    [[nodiscard]] GLFWwindow* getNativeWindow() const { return _window;}

private:
    // Constructor
    Window();
    // Destructor
    ~Window();
    // Copy constructor
    Window(const Window&) = default;
    // Move constructor
    Window(Window&&) = default;
    // Copy assignment operator
    Window& operator=(const Window&) = default;
    // Move assignment operator
    Window& operator=(Window&&) = default;

    GLFWwindow* _window;
    uint32_t _width;
    uint32_t _height;
};


#endif /* window_hpp */