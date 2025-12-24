#include <iostream>
#include <Standard_Version.hxx>

int main() 
{
    std::cout << "Hello OCCT !" << std::endl;
    std::cout << "OCCT Version: " 
              << OCC_VERSION_STRING 
              << " (Major: " << OCC_VERSION_MAJOR 
              << ", Minor: " << OCC_VERSION_MINOR 
              << ", Maintenance: " << OCC_VERSION_MAINTENANCE 
              << ")" << std::endl;
    return 0;  
}