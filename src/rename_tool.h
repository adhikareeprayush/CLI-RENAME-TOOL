#pragma once

#include <string>
#include <vector>
#include <functional>

class RenameTool {
public:
    struct Options {
        bool dryRun = false;
        bool verbose = false;
        bool quiet = false;
        bool includeFiles = false;
        bool includeFolders = true;
    };

    static std::string executableName;

    static void renameSingle(const std::string& oldName, const std::string& newName, const Options& options);
    static void renameMultiple(const std::string& directory, const std::string& pattern,
                               const std::function<std::string(const std::string&)>& renameFunc,
                               const Options& options);

    // New function to rename all files/folders
    static void renameAll(const std::string& directory,
                          const std::function<std::string(const std::string&, int)>& renameFunc,
                          const Options& options);

    static std::string addPrefix(const std::string& name, const std::string& prefix);
    static std::string addSuffix(const std::string& name, const std::string& suffix);
    static std::string replaceText(const std::string& name, const std::string& oldText, const std::string& newText);
    static std::string regexReplace(const std::string& name, const std::string& regex, const std::string& replacement);
};