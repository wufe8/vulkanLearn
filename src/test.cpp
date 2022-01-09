#include "frame.h"

int main() {
    //printVkExtensionList();
    //printVkLayerList();
    Frame app; //��ʼ����������
    try
    {
        app.run();
    }
    catch (const std::exception& e) //�������
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
