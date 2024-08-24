#include "rename_tool.h"
#include <filesystem>
#include <iostream>
#include <regex>

namespace fs = std::filesystem;

std::string RenameTool::executableName;

void RenameTool::renameSingle(const std::string& oldName, const std::string& newName, const Options& options) {
    try {
        fs::path oldPath(oldName);
        fs::path newPath(newName);

        // Check if we're trying to rename the executable
        if (oldPath.filename() == executableName) {
            std::cerr << "Warning: Attempted to rename the executable. Skipping." << std::endl;
            return;
        }

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

void RenameTool::renameAll(const std::string& directory,
                           const std::function<std::string(const std::string&, int)>& renameFunc,
                           const Options& options) {
    try {
        int count = 0;
        for (const auto& entry : fs::directory_iterator(directory)) {
            if ((options.includeFiles && fs::is_regular_file(entry)) ||
                (options.includeFolders && fs::is_directory(entry))) {
                std::string oldName = entry.path().filename().string();

                static std::string executableName;


                // Skip renaming the executable
                if (oldName == executableName) {
                    if (options.verbose) {
                        std::cout << "Skipping executable: " << oldName << std::endl;
                    }
                    continue;
                }

                std::string newName = renameFunc(oldName, count);

                if (oldName != newName) {
                    if (options.verbose) {
                        std::cout << "Renaming: " << oldName << " to " << newName << std::endl;
                    }
                    renameSingle(entry.path().string(),
                                 (entry.path().parent_path() / newName).string(),
                                 options);
                    count++;
                }
            }
        }

        if (!options.quiet) {
            std::cout << "Renamed " << count << " items." << std::endl;
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
        std::cout << "Searching in directory: " << directory << std::endl;
        std::cout << "Using pattern: " << pattern << std::endl;

        for (const auto& entry : fs::directory_iterator(directory)) {
            std::string oldName = entry.path().filename().string();
            std::cout << "Checking: " << oldName << std::endl;

            if ((options.includeFiles && fs::is_regular_file(entry)) ||
                (options.includeFolders && fs::is_directory(entry))) {

                if (std::regex_match(oldName, patternRegex)) {
                    std::string newName = renameFunc(oldName);
                    std::cout << "Match found. Old name: " << oldName << ", New name: " << newName << std::endl;

                    if (oldName != newName ) {
                        renameSingle(entry.path().string(),
                                     (entry.path().parent_path() / newName).string(),
                                     options);
                    } else {
                        std::cout << "No change in name, skipping." << std::endl;
                    }
                } else {
                    std::cout << "No match for: " << oldName << std::endl;
                }
                } else {
                    std::cout << "Skipping (not a " << (options.includeFiles ? "file" : "folder") << "): " << oldName << std::endl;
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