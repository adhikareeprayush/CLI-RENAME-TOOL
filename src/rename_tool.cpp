#include "rename_tool.h"
#include <filesystem>
#include <iostream>
#include <regex>

namespace fs = std::filesystem;
void RenameTool::renameSingle(const std::string& oldName, const std::string& newName, const Options& options) {
    try {
        fs::path oldPath(oldName);
        fs::path newPath(newName);

        if (options.dryRun) {
            if (!options.quiet) std::cout << "Would rename '" << oldPath.string() << "' to '" << newPath.string() << "'" << std::endl;
        } else {
            fs::rename(oldPath, newPath);
            if (options.verbose) std::cout << "Renamed '" << oldPath.string() << "' to '" << newPath.string() << "'" << std::endl;
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error renaming: " << e.what() << std::endl;
    }
}

void RenameTool::renameMultiple(const std::string& directory, const std::string& pattern,
                                const std::function<std::string(const std::string&)>& renameFunc,
                                const Options& options) {
    try {
        std::regex patternRegex(pattern);
        for (const auto& entry : fs::directory_iterator(directory)) {
            if ((options.includeFiles && fs::is_regular_file(entry)) ||
                (options.includeFolders && fs::is_directory(entry))) {
                std::string oldName = entry.path().filename().string();
                if (std::regex_match(oldName, patternRegex)) {
                    std::string newName = renameFunc(oldName);
                    if (oldName != newName) {
                        // Convert paths to strings before passing to renameSingle
                        renameSingle(entry.path().string(),
                                     (entry.path().parent_path() / newName).string(),
                                     options);
                    }
                }
                }
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error renaming: " << e.what() << std::endl;
    }
}

std::string RenameTool::addPrefix(const std::string& name, const std::string& prefix) {
    return prefix + name;
}

std::string RenameTool::addSuffix(const std::string& name, const std::string& suffix) {
    auto dotPos = name.find_last_of('.');
    if (dotPos == std::string::npos) return name + suffix;
    return name.substr(0, dotPos) + suffix + name.substr(dotPos);
}

std::string RenameTool::replaceText(const std::string& name, const std::string& oldText, const std::string& newText) {
    std::string result = name;
    size_t pos = 0;
    while ((pos = result.find(oldText, pos)) != std::string::npos) {
        result.replace(pos, oldText.length(), newText);
        pos += newText.length();
    }
    return result;
}

std::string RenameTool::regexReplace(const std::string& name, const std::string& regex, const std::string& replacement) {
    return std::regex_replace(name, std::regex(regex), replacement);
}