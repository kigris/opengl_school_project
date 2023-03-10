#ifndef input_hpp
#define input_hpp

#include "engine/window.hpp"
#include <functional>

using std::function;
class Input {
public:
    // Singleton
    static Input* instance() {
        // Static initialization
        static Input i;
        return &i;
    }
    // Function to check if a key is pressed
    static bool isKeyPressed(int key) {return Window::instance()->isKeyPressed(key);}
    // Functions to set the callbacks
    void setKeyPressedCallback(function<void(int, int)> f) { _keyPressedCallback = f; }
    void setMouseMoveCallback(function<void(float, float)>& f) { _mouseMoveCallback = f; }
    void setMouseScrollCallback(function<void(float, float)>& f) { _mouseScrollCallback = f; }

    // Function to call the key pressed callback
    void keyPressed(int key, int action) const {
        if (_keyPressedCallback) {
            _keyPressedCallback(key, action);
        }
    }
    // Function to call the mouse move callback
    void mouseMoved(float x, float y) const {
        if (_mouseMoveCallback) {
            _mouseMoveCallback(x, y);
        }
    }
    // Function to call the mouse scroll callback
    void mouseScrolled(float x, float y) const {
        if (_mouseScrollCallback) {
            _mouseScrollCallback(x, y);
        }
    }

private:
    // Function fields for callbacks
    function<void(int, int)> _keyPressedCallback{nullptr};
    function<void(float, float)> _mouseMoveCallback{nullptr};
    function<void(float, float)> _mouseScrollCallback{nullptr};
};

#endif /* input_hpp */