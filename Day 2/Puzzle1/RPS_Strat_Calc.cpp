#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

using namespace std;

enum Play
{
    Rock,
    Paper,
    Scissors,
};

int main()
{
    std::ifstream input_file;
    input_file.open("input.txt", std::ios::in);

    if (!input_file)
    {
        std::cout << "File could not be opened!" << std::endl;
        return 1;
    }

    int total_score = 0;
    for (std::string line; std::getline(input_file, line);)
    {
        std::vector<std::string> letters;
        std::vector<Play> tokens;

        if (!line.empty())
        {
            boost::split(letters, line, boost::is_any_of(" "), boost::token_compress_on);

            if (letters[0] == "A") tokens.push_back(Rock);
            if (letters[0] == "B") tokens.push_back(Paper);
            if (letters[0] == "C") tokens.push_back(Scissors);

            if (letters[1] == "X") tokens.push_back(Rock);
            if (letters[1] == "Y") tokens.push_back(Paper);
            if (letters[1] == "Z") tokens.push_back(Scissors);

            switch (tokens[0])
            {
                case Rock:
                    switch (tokens[1])
                    {
                        case Rock:        
                            total_score += (1 + 3);
                            break;

                        case Paper:
                            total_score += (2 + 6);
                            break;

                        case Scissors:
                            total_score += (3 + 0);
                            break;

                        default:
                            break;
                    }
                    break;

                case Paper:
                    switch (tokens[1])
                    {
                        case Rock:         
                            total_score += (1 + 0);
                            break;

                        case Paper:
                            total_score += (2 + 3);
                            break;

                        case Scissors:
                            total_score += (3 + 6);
                            break;

                        default:
                            break;
                    }
                    break;

                case Scissors:
                    switch (tokens[1])
                    {
                        case Rock:         
                            total_score += (1 + 6);
                            break;

                        case Paper:
                            total_score += (2 + 0);
                            break;

                        case Scissors:
                            total_score += (3 + 3);
                            break;

                        default:
                            break;
                    }
                    break;
                
                default:
                    break;
            }
        }
    }

    std::cout << "The Total Score is: " << total_score << std::endl;
    return 0;
}