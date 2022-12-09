#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <filesystem>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>


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
        Directory *parent;
        int dir_size = 0;
        int dir_sum = 0;

        //Set inclusive limit for adding dir size to dir sum
        int size_limit = 100000;
        int deletion_limit = 0;
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

        Directory(std::string name, Directory *parent)
        {
            this->dir_size = 0;
            this->name = name;
            this->parent = parent;
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
                this->sub_directories.push_back(Directory(name, this));
        }

        void add_file(std::string file_size, std::string name)
        {
            files.push_back(Files(stoi(file_size), name));
        }

        void list_files()
        {
            for (auto itr = this->files.begin(); itr != this->files.end(); itr++)
            {
                std::cout << itr->name << std::endl;
            }
        }

        void list_subdirs()
        {
            for (auto itr = this->sub_directories.begin(); itr != this->sub_directories.end(); itr++)
            {
                std::cout << itr->name << std::endl;
            }
        }

        int get_dir_size()
        {
            this->dir_size = 0;
            for (auto itr = this->files.begin(); itr != this->files.end(); itr++)
            {
                this->dir_size += itr->file_size;
            }

            for (auto itr = this->sub_directories.begin(); itr != this->sub_directories.end(); itr++)
            {
                this->dir_size += itr->get_dir_size();
            }

            if (this->dir_size <= size_limit)
            {
                dir_sum = dir_size;
            }

            return this->dir_size;
        }

        //Call this method on root to recursively look at and sum the dir_sum of subdirectories
        int get_dir_sum_limited()
        {
            for (auto itr = this->sub_directories.begin(); itr != this->sub_directories.end(); itr++)
            {   
                this->dir_sum += itr->get_dir_sum_limited();
            }

            return this->dir_sum;
        }

        Directory* get_delete()
        {
            Directory *deletable = this;

            for (auto itr = this->sub_directories.begin(); itr != this->sub_directories.end(); itr++)
            {
                if (itr->dir_size >= this->deletion_limit)
                {
                    if (itr->dir_size < deletable->dir_size)
                    {
                        itr->deletion_limit = this->deletion_limit;
                        deletable = itr.base()->get_delete();
                    }
                }
            }

            return deletable;
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

//Reads off first cd for / to get and set root directory and current directory
Directory* get_root_dir()
{
    std::ifstream input;
    input.open("input.txt");

    std::filesystem::path root_name;
    std::vector<std::string> split_line;
    for(std::string line; getline(input, line);)
    {
        boost::split(split_line, line, boost::is_any_of(" "), boost::token_compress_on);
        root_name /= split_line[2];
        root_name = root_name.lexically_normal();
        return new Directory(root_name);
    }

    return new Directory("NULL");
}


int main()
{
    std::ifstream input_text;
    input_text.open("input.txt");
    
    std::vector<std::string> split_line;
    std::filesystem::path curr_directory_name;
    
    int dir_read_flag = 0;
    int root_exists = 0;

    Directory *curr_dir = get_root_dir();
    Directory *root_dir = curr_dir;

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

                //Back up to parent dir
                if (split_line[2] == "..")
                {
                    curr_dir = curr_dir->parent;
                }

                //Search subdirs for cd'd name
                else
                {
                    for (auto itr = curr_dir->sub_directories.begin(); itr != curr_dir->sub_directories.end(); itr++)
                    {
                        if (itr->name == curr_directory_name.string())
                        {
                            curr_dir = itr.base();
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

    //std::cout << "Curr_dir " << curr_dir->name << std::endl;

    } // For: Line in File END

    std::cout << "Curr dir name: " << curr_dir->name << std::endl;
    std::cout << "Root name: "<< root_dir->name << std::endl;
    root_dir->list_subdirs();
    root_dir->list_files();

    std::cout << "Total root size: " << root_dir->get_dir_size() << std::endl;
    std::cout << "Total dir size under 100000k: " << root_dir->get_dir_sum_limited() << std::endl;

    root_dir->deletion_limit = (root_dir->dir_size - (70000000 - 30000000));

    std::cout << "Deletable dir " << root_dir->get_delete()->name << std::endl;
    std::cout << "Deletable dir " << root_dir->get_delete()->dir_size << std::endl;


}