//
// Created by ilya on 26.06.24.
//

#include <cstring>
#include "AnnotationsProcessor.h"

namespace fs = std::filesystem;

void checkIsDir(const fs::path& outputDir)
{
    if(!fs::is_directory(outputDir))
    {
        std::printf("Output/Input directory is not directory!\n");
        std::exit(0);
    }
}

int main(int argc, const char** argv)
{
    SGAnnotations::AnnotationsProcessor annotationsProcessor;
    
    std::string firstArg;
    
    if(argc >= 2)
    {
        firstArg = argv[1];
    }
    else
    {
        std::printf("Please, enter arguments.\n");
        return 0;
    }
    
    if(firstArg != "analyze-dirs" && firstArg != "analyze-files")
    {
        std::printf("Please, choose analyze-dirs or analyze-files as first argument. Unknown first argument^ %s\n", firstArg.c_str());
        return 0;
    }
    
    std::string secondArg;
    
    if(argc >= 3)
    {
        secondArg = argv[2];
    }
    else
    {
        std::printf("Insufficient number of arguments.\n");
        return 0;
    }
    
    std::vector<std::string> remainingArgs;
    if(argc >= 4)
    {
        for(int i = 3; i < argc; ++i)
        {
            remainingArgs.emplace_back(argv[i]);
        }
    }
    else
    {
        std::printf("Insufficient number of arguments.\n");
        return 0;
    }
    
    if(firstArg == "analyze-dirs")
    {
        if(secondArg == "recursive")
        {
            if(remainingArgs.size() < 2)
            {
                std::printf("Insufficient number of arguments. Make sure you specify the output folder as well as the analysis folder.\n");
                return 0;
            }
            
            fs::path outputDirPath = remainingArgs[0];
            fs::path inputDirPath = remainingArgs[1];
            
            inputDirPath = fs::canonical(inputDirPath);
            checkIsDir(inputDirPath);
            
            try
            {
                outputDirPath = fs::canonical(outputDirPath);
                checkIsDir(outputDirPath);
            }
            catch(const std::exception& e)
            {
            
            }
            
            annotationsProcessor.processAnnotations(inputDirPath, true);
        }
        else
        {
            if(remainingArgs.empty())
            {
                std::printf("Insufficient number of arguments. Make sure you specify the output folder as well as the analysis folder.\n");
                return 0;
            }
            
            fs::path outputDirPath = secondArg;
            fs::path inputDirPath = remainingArgs[0];
            
            inputDirPath = fs::canonical(inputDirPath);
            checkIsDir(inputDirPath);
            
            try
            {
                outputDirPath = fs::canonical(outputDirPath);
                checkIsDir(outputDirPath);
            }
            catch(const std::exception& e)
            {
            
            }
            
            annotationsProcessor.processAnnotations(inputDirPath, false);
        }
    }
    else if(firstArg == "analyze-files")
    {
        if(remainingArgs.empty())
        {
            std::printf("Insufficient number of arguments. Make sure you specify the output folder as well as the analysis folder.\n");
            return 0;
        }
        
        fs::path outputDirPath = secondArg;
        
        outputDirPath = fs::canonical(outputDirPath);
        
        checkIsDir(outputDirPath);
        
        std::vector<fs::path> filesPaths;
        filesPaths.reserve(remainingArgs.size());
        for(const auto& a : remainingArgs)
        {
            filesPaths.emplace_back(a);
        }
        
        annotationsProcessor.processAnnotations(filesPaths);
    }
    
    return 0;
}