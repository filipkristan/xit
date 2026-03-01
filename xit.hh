#ifndef XIT_XIT_HH
#define XIT_XIT_HH
#include <string>

class xit
{
    public:
    static void createNewPatch(std::string commitMessage);
    static void appendPatchMessage(std::string arg);
    static void writeDataToFile(const std::string &location, const std::string &data, bool appendMode);
    static std::string readDataFromFile(std::string location);
    static void makeFirstPatch();
    static void initXitRepo(std::string arg);
    static void handleDisplayingTheHelpPage();
    static void handleMakingPatches(std::string commitMessage);
    static void setupXitRepo(std::string arg);
    static void revertPatches(const std::string& argv2);
    static void makeNewBranch(const std::string &arg);
    static void changeBranch(const std::string &arg);
    static void removeBranch(const std::string &arg);
    static void renameBranch(const std::string &arg, const std::string &arg2);
};


#endif //XIT_XIT_HH