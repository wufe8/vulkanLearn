#include "frame.h"

int main() {
    //printVkExtensionList();
    //printVkLayerList();
    Frame app; //初始化主窗口类
    try
    {
        app.run();
    }
    catch (const std::exception& e) //捕获错误
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
