#include "differentiator.h"
#include <stdlib.h>

FILE *open_file(const char *file_name, const char *key)
{
    assert(file_name != nullptr);
    
    FILE *file = fopen(file_name, key);
    
    assert(file != nullptr);
    
    return file;
}

size_t get_file_size(FILE *file)
{
    assert(file != nullptr);

    long int prev = ftell(file);

    fseek(file, 0L, SEEK_END);
    
    long int file_size = ftell(file);
    
    fseek(file, prev, SEEK_SET);

    return (size_t)file_size;
}

size_t read_file(const char *file_name, char **string)
{
    assert(string != nullptr);

    char key[] = "rb";
    
    FILE *file = open_file(file_name, key);
    
    return read_file(file, string);
}
    
size_t read_file(FILE *file, char **string)
{
    assert(file != nullptr);
    assert(string != nullptr);

    size_t file_size = get_file_size(file);

    *string = (char *)calloc((file_size + 1), sizeof(char));
    
    assert(*string != nullptr);

    fread(*string, sizeof(char), file_size, file);

    return file_size;
}