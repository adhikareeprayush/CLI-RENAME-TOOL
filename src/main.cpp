#include "rename_tool.h"
#include <iostream>
#include <string>
#include <functional>

void printUsage() {
    std::cout << "Usage:" << std::endl;
    std::cout << "  rename-tool [options] <command> [arguments]" << std::endl;
    std::cout << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  --dry-run    Preview changes without applying them" << std::endl;
    std::cout << "  --verbose    Print detailed information" << std::endl;
    std::cout << "  --quiet      Suppress all output except errors" << std::endl;
    std::cout << "  --files      Include files in renaming (default: folders only)" << std::endl;
    std::cout << "  --all        Include both files and folders in renaming" << std::endl;
    std::cout << std::endl;
    std::cout << "Commands:" << std::endl;
    std::cout << "  single <old_name> <new_name>" << std::endl;
    std::cout << "  prefix <directory> <pattern> <prefix>" << std::endl;
    std::cout << "  suffix <directory> <pattern> <suffix>" << std::endl;
    std::cout << "  replace <directory> <pattern> <old_text> <new_text>" << std::endl;
    std::cout << "  regex <directory> <pattern> <regex> <replacement>" << std::endl;
}

int main(int argc, char* argv[]) {
    RenameTool::Options options;
    std::vector<std::string> args;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--dry-run") options.dryRun = true;
        else if (arg == "--verbose") options.verbose = true;
        else if (arg == "--quiet") options.quiet = true;
        else if (arg == "--files") {
            options.includeFiles = true;
            options.includeFolders = false;
        }
        else if (arg == "--all") {
            options.includeFiles = true;
            options.includeFolders = true;
        }
        else args.push_back(arg);
    }

    if (args.empty()) {
        printUsage();
        return 1;
    }

    std::string command = args[0];

    if (command == "single" && args.size() == 3) {
        RenameTool::renameSingle(args[1], args[2], options);
    } else if (command == "prefix" && args.size() == 4) {
        RenameTool::renameMultiple(args[1], args[2],
            [&](const std::string& name) { return RenameTool::addPrefix(name, args[3]); }, options);
    } else if (command == "suffix" && args.size() == 4) {
        RenameTool::renameMultiple(args[1], args[2],
            [&](const std::string& name) { return RenameTool::addSuffix(name, args[3]); }, options);
    } else if (command == "replace" && args.size() == 5) {
        RenameTool::renameMultiple(args[1], args[2],
            [&](const std::string& name) { return RenameTool::replaceText(name, args[3], args[4]); }, options);
    } else if (command == "regex" && args.size() == 5) {
        RenameTool::renameMultiple(args[1], args[2],
            [&](const std::string& name) { return RenameTool::regexReplace(name, args[3], args[4]); }, options);
    } else {
        printUsage();
        return 1;
    }

    return 0;
}