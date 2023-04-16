#include <iostream>
#include <string>
#include <cstdlib>
#include <filesystem>

using namespace std;

void createJpgFromPdf(const string& pdfPath) {
    string folderPath = pdfPath.substr(0, pdfPath.find_last_of('\\')) + "\\resultPdfToJpeg";
    filesystem::create_directory(folderPath);

    string command = "magick -density 300 -quality 100 \"" + pdfPath + "\" \"" + folderPath + "\\" + filesystem::path(pdfPath).stem().string() + ".jpg\"";
    system(command.c_str());

    cout << "JPG files created in folder: " << folderPath << endl;
}

void createPdfFromJpg(const string& jpgPath) {
    string folderPath = jpgPath.substr(0, jpgPath.find_last_of('\\')) + "\\resultJpegToPdf";
    filesystem::create_directory(folderPath);

    string command = "magick -quality 100 \"" + jpgPath + "\" \"" + folderPath + "\\" + filesystem::path(jpgPath).stem().string() + ".pdf\"";
    system(command.c_str());

    cout << "PDF file created in folder: " << folderPath << endl;
}

void convertJpgsToPdf(string folderPath) {
    // get the parent directory of the given folder path
    std::filesystem::path parentPath = std::filesystem::path(folderPath).parent_path();

    // create the new folder in the parent directory
    std::filesystem::path resultPath = parentPath / "resultJpegsToPdf";
    if (!std::filesystem::exists(resultPath)) {
        std::filesystem::create_directory(resultPath);
    }

    // create the output PDF file path
    std::filesystem::path outputPath = resultPath / "output.pdf";

    // create the command to convert the JPEG files to PDF
    string command = "magick *.jpg \"";
    command += outputPath.string();
    command += "\"";

    // change the working directory to the folder path
    std::filesystem::current_path(folderPath);

    // execute the command
    system(command.c_str());

    // change the working directory back to the parent directory
    std::filesystem::current_path(parentPath);
}


int main() {
    string path;
    cout << "Enter the full path of the file or folder: ";
    getline(cin, path);

    // Remove quotes from path if present
    if (path.front() == '"' && path.back() == '"') {
        path = path.substr(1, path.length() - 2);
    }

    // Determine whether path is a file or folder
    filesystem::path p(path);
    if (filesystem::is_directory(p)) {
        // Path is a folder
        convertJpgsToPdf(path);
    }
    else {
        // Path is a file
        string ext = p.extension().string();
        if (ext == ".pdf") {
            createJpgFromPdf(path);
        }
        else if (ext == ".jpg" || ext == ".jpeg") {
            createPdfFromJpg(path);
        }
        else {
            cout << "Invalid file extension. Must be .pdf, .jpg, or .jpeg" << endl;
        }
    }

    return 0;
}