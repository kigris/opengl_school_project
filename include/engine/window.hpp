#ifndef window_hpp
#define window_hpp

#include <cstdint>

class GLFWwindow;

class Window {
public:
    // Singleton
    static Window* instance();

    [[nodiscard]] bool isAlive() const;
    void frame() const;

    [[nodiscard]] GLFWwindow* getNativeWindow() const { return _window;}

    void setWidth(uint32_t width) { _width = width; }
    [[nodiscard]] uint32_t getWidth() const { return _width; }

    void setHeight(uint32_t height) { _height = height; }
    [[nodiscard]] uint32_t getHeight() const { return _height; }

    static bool isKeyPressed(int key);
    void setCaptureMouse(bool toggle) const;

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

    // Fields
    GLFWwindow* _window;
    uint32_t _width = 800;
    uint32_t _height = 600;
};


#endif /* window_hpp */