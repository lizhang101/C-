#include <iostream>
#include <memory>
class widget {
    class impl;
    std::unique_ptr<impl> pimpl;
public:
    void draw(); // public API that will be forwarded to the implementation
    widget(int); // defined in the implementation file
    ~widget();   // defined in the implementation file, where impl is a complete type
    widget(widget&&) = default;
    widget(const widget&) = delete;
    widget& operator=(widget&&); // defined in the implementation file
    widget& operator=(const widget&) = delete;
};

class widget::impl {
    int n; // private data
public:
    void draw(const widget& w) { std::cout << "Pimpl Draw" << std::endl; }
    impl(int n) : n(n) {}
};

void widget::draw() { pimpl->draw(*this); }
widget::widget(int n) : pimpl{std::make_unique<impl>(n)} {}
widget::~widget() = default;
widget& widget::operator=(widget&&) = default;

int main() {
    widget w(10);
    w.draw();
    return 0;
}
