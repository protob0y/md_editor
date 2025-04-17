#include "fileio.h"

bool FileIO::SaveFile(std::vector<std::wstring> * pDocument, std::string filename){
    // create new folder if it doesn't exist
    if(!std::filesystem::exists("./documents")){
        std::filesystem::create_directories("./documents");
    }
    
    std::string filePath = "./documents/" + filename + ".txt";
    
    std::ofstream outFile(filePath);
    if(!outFile.is_open()){
        return false;
    }

    outFile << "Beginning of file " << filename << " stored as " << filePath << std::endl;
    outFile.close();

    // todo: implement check (read back file and compare everything to see if everything was written out)
    return true;
}