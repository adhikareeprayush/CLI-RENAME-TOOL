# CLI Renaming Toolkit

A powerful command-line interface (CLI) toolkit for batch renaming files and folders, written in C++.

## Features

- Rename single files or folders
- Batch rename multiple items based on patterns
- Add prefixes or suffixes to filenames
- Replace text in filenames
- Use regular expressions for advanced renaming
- Dry-run mode to preview changes
- Verbose output option for detailed information

## Building the Project

This project uses CMake. To build:

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

## Usage

```bash
./rename-tool [options] <command> [arguments]
```

Options:
- `--dry-run`: Preview changes without applying them
- `--verbose`: Print detailed information
- `--quiet`: Suppress all output except errors
- `--files`: Include only files in renaming (default: folders only)
- `--all`: Include both files and folders in renaming

Commands:
- `single <old_name> <new_name>`
- `prefix <directory> <pattern> <prefix>`
- `suffix <directory> <pattern> <suffix>`
- `replace <directory> <pattern> <old_text> <new_text>`
- `regex <directory> <pattern> <regex> <replacement>`
- `batch <directory> <prefix> [start_number]`

## Key Functions

### RenameTool::renameSingle

This function renames a single file or folder. It includes safety checks to prevent renaming the executable itself.

```cpp
void RenameTool::renameSingle(const std::string& oldName, const std::string& newName, const Options& options)
```

### RenameTool::renameAll

Renames all files/folders in a directory based on a provided renaming function.

```cpp
void RenameTool::renameAll(const std::string& directory,
                           const std::function<std::string(const std::string&, int)>& renameFunc,
                           const Options& options)
```

### RenameTool::renameMultiple

Renames multiple files/folders in a directory based on a regex pattern.

```cpp
void RenameTool::renameMultiple(const std::string& directory, const std::string& pattern,
                                const std::function<std::string(const std::string&)>& renameFunc,
                                const Options& options)
```

### Helper Functions

- `addPrefix`: Adds a prefix to a filename.
- `addSuffix`: Adds a suffix to a filename while preserving the extension.
- `replaceText`: Replaces all occurrences of a substring in a filename.
- `regexReplace`: Uses regex to replace parts of a filename.

## How It Works

1. The main program parses command-line arguments to determine the operation and options.
2. Based on the command, it calls the appropriate RenameTool function.
3. The renaming functions use C++17's filesystem library for file operations.
4. Error handling is implemented using try-catch blocks for filesystem operations.
5. The toolkit provides flexibility through the Options struct, allowing for dry runs, verbose output, and more.

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## License

This project is licensed under the [MIT License](LICENSE) - see the LICENSE file for details.
