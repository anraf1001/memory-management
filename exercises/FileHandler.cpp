#include <cstdio>
#include <cstdlib>
#include <iostream>

class FileOpeningError : public std::runtime_error {
public:
    FileOpeningError(std::string path)
        : std::runtime_error("File opening failed: " + path) {}
};

class FileHandler {
    FILE* fp_;

public:
    FileHandler(const std::string& path)
        : fp_(std::fopen(path.c_str(), "r")) {
        if (!fp_) {
            throw FileOpeningError{path};
        }
    }
    ~FileHandler() {
        std::fclose(fp_);
    }

    friend std::ostream& operator<<(std::ostream& out, const FileHandler& fh);
};

std::ostream& operator<<(std::ostream& out, const FileHandler& fh) {
    char c;
    while ((c = std::fgetc(fh.fp_)) != EOF) {  // standard C I/O file reading loop
        out << c;
    }

    if (std::ferror(fh.fp_)) {
        throw std::runtime_error("I/O error when reading");
    } else if (std::feof(fh.fp_)) {
        out << "End of file reached successfully\n";
    }

    return out;
}

int main() {
    try {
        FileHandler fh("../ResourceD.cpp");
        std::cout << fh;
    } catch (FileOpeningError& foe) {
        std::cerr << foe.what() << '\n';
    } catch (std::runtime_error& re) {
        std::cerr << re.what() << '\n';
    }
    return 0;
}
