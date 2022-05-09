#include <iostream>
#include <cstring>
#include "all_test.h"

int main(int argc, char const *argv[])
{
    bool output = false;
    if (argc > 2)
    {
        if (!strcmp(argv[2], "-h") || !strcmp(argv[2], "--help"))
        {
            std::cout << "Usage: ./main_test [-v|--verbose]" << std::endl;
        }
        if (!strcmp(argv[2], "-v") || !strcmp(argv[2], "--verbose"))
        {
            output = true;
        }
    }
    bool final_result = data_type_test(output);
    final_result = (final_result && edge_case_test(output));
    final_result = (final_result && load_write_file_test(output));
    if (!final_result)
    {
        std::cout << "Tests failed" << std::endl;
    }
}