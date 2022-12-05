#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

using namespace std;

enum OpponentMove
{
    Rock,
    Paper,
    Scissors,
};

enum Play
{
    Win, //Z
    Lose, //X
    Draw, //Y
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
        OpponentMove opponent;
        Play playermove;

        if (!line.empty())
        {
            boost::split(letters, line, boost::is_any_of(" "), boost::token_compress_on);

            if (letters[0] == "A") opponent = Rock;
            if (letters[0] == "B") opponent = Paper;
            if (letters[0] == "C") opponent = Scissors;

            if (letters[1] == "X") playermove = Lose;
            if (letters[1] == "Y") playermove = Draw;
            if (letters[1] == "Z") playermove = Win;

            switch (opponent)
            {
                case Rock:
                    switch (playermove)
                    {
                        case Win:        //Paper
                            total_score += (2 + 6);
                            break;

                        case Draw:      //Rock
                            total_score += (1 + 3);
                            break;

                        case Lose:      //Scissors
                            total_score += (3 + 0);
                            break;

                        default:
                            break;
                    }
                    break;

                case Paper:
                    switch (playermove)
                    {
                        case Win:         //Scissors
                            total_score += (3 + 6);
                            break;

                        case Draw:      //Paper
                            total_score += (2 + 3);
                            break;

                        case Lose:      //Rock
                            total_score += (1 + 0);
                            break;

                        default:
                            break;
                    }
                    break;

                case Scissors:
                    switch (playermove)
                    {
                        case Win:         //Rock
                            total_score += (1 + 6);
                            break;

                        case Draw:      //Scissors
                            total_score += (3 + 3);
                            break;

                        case Lose:      //Paper
                            total_score += (2 + 0);
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