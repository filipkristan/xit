#include "xit.hh"
#include <fstream>
#include <algorithm>
#include <string>
#include <cctype>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <vector>

std::string branch = "master";
std::string branchPath = ".xit/branch/" + branch;
std::string lastPatchPath = ".xit/branch/" + branch + "/lastPatchPath.txt";

void xit::writeDataToFile(const std::string& location, const std::string& data, bool appendMode) {
    std::ofstream file;
    if (appendMode)
    {
        file.open(location, std::ios::app);
    }
    else
    {
        file.open(location, std::ios::out);
    }

    if (file.is_open())
    {
        file << data << "\n";
        file.close();
    }
    else
        std::cout << "File was closed!" << "\n";
}

std::string xit::readDataFromFile(std::string location) {
    std::ifstream stream(location);
    std::string data;
    if (stream) {
        std::ostringstream ss;
        ss << stream.rdbuf();
        data = ss.str();
        std::erase(data, '\n');
    } else {
        std::cout << "Unable to read data from the file!" <<'\n';
        std::cout << "File location: " << location <<'\n';
    }
    return data;
}

void xit::makeFirstPatch() {
    // Create a diff between src dir and xit src dir and save patch in patches folder
    std::ofstream file;
    file.open (lastPatchPath.c_str());
    std::string value = "0001";
    if (file.is_open())
    {
        file << value << "\n";
        std::string command1 = "diff -ruN " + branchPath + "/original_src/ src/ > " + branchPath + "/patches/0001.patch";
        std::string command2 = "cp -r src/ " + branchPath + "/new_src";
        system(command1.c_str());
        system(command2.c_str());
        file.close();
    }
}

void xit::initXitRepo(std::string arg) {

    branch = arg;
    branchPath = ".xit/branch/" + branch;
    std::string command1 = "rm -rf src && mkdir src && mkdir -p .xit/branch && mkdir -p " + branchPath + "/original_src && mkdir -p " + branchPath + "/old_src && mkdir -p " + branchPath + "/new_src && mkdir -p " + branchPath + "/patches && cp -r src " + branchPath + "/original_src/";
    system(command1.c_str());
    xit::makeFirstPatch();
    xit::writeDataToFile(".xit/branch.txt", "master", false);
    std::cout << "Xit repo initialized!" << std::endl;
}

void xit::handleDisplayingTheHelpPage() {
    std::cout << "Use the command 'xit init' to initialize Xit." << std::endl;
    std::cout << "Use the command 'xit new' to create a new patch." << std::endl;
    std::cout << "Use the command 'xit revert 00XX' to revert back to before that patch." << std::endl;
}

void xit::handleMakingPatches(std::string commitMessage) {
    std::string command = branchPath + "/patches/0001.patch";
    std::ifstream first_patch_location_path(command.c_str());
    std::ifstream XitLocationPath(".xit");
    if (XitLocationPath.is_open())
    {
        if(first_patch_location_path.is_open())
        {
            xit::createNewPatch(commitMessage);
            first_patch_location_path.close();
        }
        else
        {
            xit::makeFirstPatch();
        }
        XitLocationPath.close();
    }
    else
        std::cout << "Xit not yet initialized!" << std::endl;
}

void xit::setupXitRepo(std::string arg) {
    std::ifstream DotXitFolderLocationPath(".xit");
    std::ifstream firstPatchLocationPath(branchPath + "patches/0001.patch");
    if(!DotXitFolderLocationPath.is_open()){
        xit::initXitRepo(arg);
        DotXitFolderLocationPath.close();
        system("echo \"author = \"John Smith\"\nwebsite = \"www.example.com\" \nprogram_name = \"app\" \nprogram_name_fancy = \"The Example Application\" \nlanguage = \"C++\" \ntags = \"example, portfolio, learning"" > info.txt");
    }
    else
        std::cout << "Xit repo already initialized!" << std::endl;
}

void xit::revertPatches(const std::string& argv2) {
    std::ifstream DotXitFolderLocationPath(".xit");
    std::ifstream file;
    file.open(lastPatchPath);
    if (file.is_open())
    {
        std::vector<std::string> data;
        std::string line;
        while ( getline (file,line) )
        {
            data.push_back(line);
        }
        file.close();
        int start = atoi(data[0].c_str());
        int currentValue = std::stoi(data[0]) + 1;
        std::string command;
        std::string zeroPadding;
        for (int i = atoi(argv2.c_str()) - 1; i < start; ++i)
        {
            currentValue--;
            std::string newValue = std::to_string(currentValue);
            if (newValue.length() == 1)
            {
                zeroPadding = "000";
            }
            else
                if (newValue.length() == 2)
                {
                    zeroPadding = "00";
                }
                else
                    if (newValue.length() == 3)
                    {
                        zeroPadding = "0";
                    }
                    else
                    {
                        zeroPadding = "";
                    }
            std::string value = zeroPadding + newValue;
            command = "patch -s -p1 -d " + branchPath + "/new_src -R < " + branchPath + "/patches/" + value + ".patch";
            system(command.c_str());
            std::string command2 = "rm " + branchPath + "/patches/" + value + ".patch";
            system((command2).c_str());
            int subtract = atoi(value.c_str()) - 1;
            system(("echo " + std::to_string(subtract) + " > .xit/lastPatchPath.txt").c_str());
        }
        std::string command2 = "cp -r .xit/new_src/ src/";
        system("rm -rf src");
        system(command2.c_str());
        system("rm -rf src/*.orig");
        std::cout << "Reverted patches!" << std::endl;
    }
    else
        std::cout << "Idk, this shouldn't happen!" << std::endl;
}


void xit::makeNewBranch(const std::string(&arg)) {
    std::string branchNameToCopy = branch;
    branch = arg;
    std::string command1 = "mkdir .xit/branch/" + branch;
    std::string command2 = "cp -r .xit/branch/" + branchNameToCopy + "/* " + ".xit/branch/" + branch;
    system(command1.c_str());
    system(command2.c_str());
    std::cout << "Created new branch: " << arg << std::endl;
}

void xit::changeBranch(const std::string(&arg)) {
    std::string currentBranch = xit::readDataFromFile(".xit/branch.txt");
    branchPath = ".xit/branch/" + arg;
    std::string command1 = "rm -rf src && mkdir src";  // Recreates src folder
    std::string command2 = "cp -r " + branchPath + "/new_src/*" + " src";  // Copies branch content to src folder
    system(command1.c_str());
    system(command2.c_str());
    xit::writeDataToFile(".xit/branch.txt", arg, false);
    std::cout << "Changed branch to: " << arg << std::endl;
}

void xit::removeBranch(const std::string(&arg)) {
    std::string command = "rm -rf .xit/branch/" + arg;
    system(command.c_str());
    std::cout << "Removed branch: " << arg << std::endl;
}

void xit::renameBranch(const std::string(&arg), const std::string& arg2) {
    std::string command = "mv .xit/branch/" + arg + " .xit/branch/" + arg2;
    system(command.c_str());
    std::cout << "Renamed branch from " << arg << " to: " << arg2 << std::endl;
}

void xit::createNewPatch(std::string commitMessage) {
    std::ifstream lastPatchPathLocationRead;
    lastPatchPathLocationRead.open(lastPatchPath.c_str());
    if (lastPatchPathLocationRead.is_open())
    {
        std::string zeroPadding;
        std::vector<std::string> data;
        std::string line;

        while ( getline (lastPatchPathLocationRead,line) )
        {
            data.push_back(line);
        }

        int num = std::stoi(data[0]) + 1;
        std::string newValue = std::to_string(num);
        std::ofstream lastPatchLocationInsert;
        lastPatchLocationInsert.open (lastPatchPath.c_str());
        if (newValue.length() == 1)
        {
            zeroPadding = "000";
        }
        else
            if (newValue.length() == 2)
            {
                zeroPadding = "00";
            }
            else
                if (newValue.length() == 3)
                {
                    zeroPadding = "0";
                }
                else
                {
                    zeroPadding = "";
                }
        std::string value = zeroPadding + newValue;

        if (lastPatchLocationInsert.is_open()) {
            // Update the last patch
            lastPatchLocationInsert << value << "\n";

            // Make a new patch
            branch = readDataFromFile(".xit/branch.txt");
            branchPath = ".xit/branch/" + branch;
            std::string command = "diff -ruN " + branchPath + "/new_src/ src > " + branchPath + "/patches/" + value + ".patch";
            std::string command2 = "cp -r src " + branchPath + "/new_src/";
            std::string patchMessagesLocation = ".xit/branch/" + branch + "/patchMessages/";
            std::string command3 = "mkdir " + patchMessagesLocation;
            system(command.c_str());
            system(command2.c_str());
            if (std::filesystem::exists(patchMessagesLocation)) {
                std::cout << "File exists \n";
            }
            else
            {
                system(command3.c_str());
            }
            std::string dataLocation = ".xit/branch/" + branch + "/patchMessages/" + value;
            xit::writeDataToFile(dataLocation, std::move(commitMessage), false);
            lastPatchLocationInsert.close();
        }
        else
            lastPatchPathLocationRead.close();
        std::cout << "Created a new patch!"<< std::endl;
    }
    else
        std::cout << "Xit repo not yet initialized!" << "\n";
}