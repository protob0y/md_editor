#include "fileio.h"

bool FileIO::SaveFile(std::vector<std::wstring> * pDocument, std::string filename){
    std::ofstream outFile(filename);
    outFile << "Beginning of file " << filename << std::endl;
    outFile.close();
    return false;
}