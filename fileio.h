//class for loading and saving files

#ifndef FILEIO_H
#define FILEIO_H

#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>

class FileIO{
    public:
    bool SaveFile(std::vector<std::wstring> * pDocument, std::string filename);



};


#endif