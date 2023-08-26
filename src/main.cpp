// C++ standard libraries
#include <iostream>

// Custom libraries
#include "application.hpp"


int main() {
    Application app(
        1280,
        720,
        "test",
        "fonts/MONOFONT.TTF"
    );

    return app.run();
}