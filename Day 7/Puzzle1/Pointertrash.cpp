#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <filesystem>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

using namespace std;

class Files
{
    public:
        std::string name;
        int file_size;

    Files(int file_size, std::string name)
    {
        this->file_size = file_size;
        this->name = name;
    }
};

class Directory
{
    public:
        std::vector<Directory> parent;
        int dir_size;
        std::string name;
        std::vector<Directory> sub_directories;
        std::vector<Files> files;

        Directory()
        {
            this->dir_size = 0;
            this->name = "";
        }

        Directory(std::string name)
        {
            this->dir_size = 0;
            this->name = name;
        }

        Directory(std::string name, Directory parent)
        {
            this->dir_size = 0;
            this->name = name;
            this->parent.push_back(parent);
        }

        bool subdirectory_exists(Directory* dir)
        {
            for (auto itr = this->sub_directories.begin(); itr != this->sub_directories.end(); itr++)
            {
                if (dir->name == itr->name)
                {
                    return true;
                }
            }
            return false;
        }

        bool file_exists(Files* file)
        {
            for (auto itr = this->sub_directories.begin(); itr != this->sub_directories.end(); itr++)
            {
                if (file->name == itr->name)
                {
                    return true;
                }
            }
            return false;
        }

        void append_dir(std::string name)
        {
            Directory *temp = new Directory(name, *this);
            if (!subdirectory_exists(temp))
            {
                this->sub_directories.push_back(*temp);
            }
    
        }

        void add_file(std::string file_size, std::string name)
        {
            files.push_back(Files(stoi(file_size), name));
        }

        void list_files()
        {
            for (auto itr = this->files.begin(); itr != this->files.end(); itr++)
            {
                cout << itr->name << std::endl;
            }
        }

        void list_subdirs()
        {
            for (auto itr = this->sub_directories.begin(); itr != this->sub_directories.end(); itr++)
            {
                cout << itr->name << std::endl;
            }
        }

        int get_dir_size()
        {
            cout << this->name << std::endl;
            this->dir_size = 0;
            for (auto itr = this->files.begin(); itr != this->files.end(); itr++)
            {
                this->dir_size += itr->file_size;
            }

            for (auto itr = this->sub_directories.begin(); itr != this->sub_directories.end(); itr++)
            {
                this->dir_size += itr->get_dir_size();
            }



            if (this->dir_size < 100000)
                cout << "QUALOODES: " << this->dir_size << std::endl;
            return this->dir_size;
        }
};

bool is_command(std::string token)
{
    if (token == "$")
        return true;
    return false;
}

bool dir_flag_enabled(int flag)
{
    if (flag == 1)
        return true;
    return false;
}


int main()
{
    ifstream input_text;
    input_text.open("Segf.txt");
    
    std::vector<Directory> root;
    std::vector<std::string> split_line;
    std::filesystem::path curr_directory_name;
    
    Directory *curr_dir;
    
    int dir_read_flag = 0;
    int root_exists = 0;
    for(std::string line; getline(input_text, line);)
    {
        boost::split(split_line, line, boost::is_any_of(" "), boost::token_compress_on);

        if (is_command(split_line[0]))
        {
            //Command is CD
            dir_read_flag = 0;
            if (split_line[1] == "cd")
            {
                //Set dir name
                curr_directory_name /= split_line[2];
                curr_directory_name = curr_directory_name.lexically_normal();

                //Initial root setup
                if (root_exists == 0)
                {
                    curr_dir = new Directory(curr_directory_name.string());
                    root.push_back(*curr_dir);
                    root_exists = 1;
                }

                //Back up to parent dir
                else if (split_line[2] == "..")
                {
                    if (curr_dir->parent.size() != 0)
                        *curr_dir = *curr_dir->parent.begin();
                }

                //Search subdirs for cd'd name
                else
                {
                    for (auto itr = curr_dir->sub_directories.begin(); itr != curr_dir->sub_directories.end(); itr++)
                    {
                        if (itr->name == curr_directory_name.string())
                        {
                            *curr_dir = *itr;
                            break;
                        }
                    }

                }
            }

            //Command is LS
            else if (split_line[1] == "ls")
            {
                dir_read_flag = 1;
            }
        }

        //Parsing subdirs and files
        else if (dir_flag_enabled(dir_read_flag))
        {
            std::filesystem::path temp_path = curr_directory_name;

            //Dir name add
            if (split_line[0] == "dir")
            {
                temp_path /= split_line[1]; 
                curr_dir->append_dir(temp_path);
                for(auto itr = curr_dir->sub_directories.begin(); itr != curr_dir->sub_directories.end(); itr++)
                {

                }
            }

            //File name add
            else if (std::all_of(split_line[0].begin(), split_line[0].end(), ::isdigit))
            {
                curr_dir->add_file(split_line[0], split_line[1]);
            }
        }

    } // For: Line in File END

    
    root.begin()->list_subdirs();
}