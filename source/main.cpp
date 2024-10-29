#include <iostream>
#include "logLib.hpp"

int main() {
    setLoggingLevel(DEBUG);
    LOG_DEBUG("Hello world");

    return 0;
}
