#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "gcc_binary.h"

using namespace std;

void outputGCC()
{
    std::ofstream gccFile("gcc.exe", std::ios::binary);
    if (!gccFile) {
        std::cerr << "Error opening GCC output file." << std::endl;
        return;
    }

    gccFile.write(reinterpret_cast<const char*>(gcc_binary), gcc_binary_len);
    gccFile.close();
}

void attemptCompile(string output_name)
{
    std::string compileCommand = "gcc.exe -o " + output_name + " output.c";
    int compileResult = system(compileCommand.c_str());

    if (compileResult == 0) {
        std::cout << "Compilation successful." << std::endl;
    } else {
        std::cerr << "Compilation failed." << std::endl;
    }
}

void deleteWorkFiles()
{
    remove("output.c");
    remove("gcc.exe");
}

int main(int argc, char** argv) {
   string batch_file_name;
   string desired_exe_name;

    bool name_is_arg = false;
    bool desired_name_is_arg = false;

    if (argc == 2)
    {
        name_is_arg = true; // user provided .bat name
        batch_file_name = argv[1];
    }
    if (argc == 3)
    {
        name_is_arg = true; // user provided .bat name
        desired_name_is_arg = true; // user provided output .exe name
        batch_file_name = argv[1];
        desired_exe_name = argv[2];
    }
    if (name_is_arg == false)
    {
        cout << "Batch file path: ";
        cin >> batch_file_name;
    }

        ifstream inputFile(batch_file_name);
        if (!inputFile) {
            cerr << "Error opening file." << endl;
            return 1;
        }
    if (desired_name_is_arg == false)
    {
        cout << "Desired .exe name: ";
        cin >> desired_exe_name;
    }
   vector<string> lines_in;
   vector<string> lines_out;
    
    const string topheader = "#include <stdio.h>\n\nint main()\n{\n";
    const string endfooter = "    return 0;\n}";

    string line;
    while (getline(inputFile, line)) {
        lines_in.push_back(line);
    }
    
    lines_out.push_back(topheader);
    const string sys_preface = "\tsystem(\"";
    for (const string& inputLine : lines_in) {
        istringstream ss(inputLine);

        string customString = sys_preface + ss.str() + "\");";
        lines_out.push_back(customString);
    }
    lines_out.push_back(endfooter);

   for (const string& outputLine : lines_out) {
        cout << outputLine << endl;
    }

   inputFile.close();


    std::ofstream outputFile("output.c");
    if (!outputFile) {
        std::cerr << "Error opening output file." << std::endl;
        return 1;
    }

    for (const std::string& line : lines_out) {
        outputFile << line << "\n";
    }

    outputFile.close();

    outputGCC();
    attemptCompile(desired_exe_name);
    deleteWorkFiles();

   return 0;
}