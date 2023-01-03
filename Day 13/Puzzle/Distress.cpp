#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <typeinfo>

#include <nlohmann/json.hpp>

std::vector<std::string> store_input(std::string file_select)
{
    std::ifstream input_file(file_select);
    std::vector<std::string> input;

    for (std::string line; std::getline(input_file, line);)
    {
        input.push_back(line);
    }

    return input;
}

std::vector<nlohmann::json> json_parsed(std::vector<std::string> input)
{
    //Converts the input into json, could just implement this into the store_input function
    //But I didn't want to change it right now
    std::vector<nlohmann::json> parsed_input;

    int i = 0;
    for (auto line : input)
    {
        if (!line.empty()) parsed_input.push_back(nlohmann::json::parse(line));
    }

    return parsed_input;
}

std::vector<std::pair<std::string, std::string>> pack_pairs(std::vector<std::string> input)
{
    //Returns pairs of packets for easier manipulation
    std::vector<std::pair<std::string, std::string>> parsed_input;
    std::pair<std::string, std::string> temp_pair;

    //The input is broken up into pairs of packets like:
    //Line 0: packet 1
    //Line 1: packet 2
    //Line 2: blank
    int i = 0;
    for (auto line : input)
    {
        switch (i)
        {
            case 0:
                temp_pair.first = line;
                i++;
                break;
            
            case 1:
                temp_pair.second = line;
                i++;
                break;

            case 2:

                parsed_input.push_back(temp_pair);
                i = 0;
                break;

            default:
                break;
        }
    }

    parsed_input.push_back(temp_pair);
    return parsed_input;
}

int iterate_packet(nlohmann::json packet1, nlohmann::json packet2)
{
    int largest_packet = packet1.size() > packet2.size() ? packet1.size() : packet2.size();

    for (int i = 0; i < largest_packet; i++)
    {
        std::cout << packet1[i] << " ||| " << packet2[i] << std::endl;

        if (packet1[i].is_null() && !packet2[i].is_null()) 
        {
            std::cout << "Packet 1 is null, packet 2 is not null" << std::endl; 
            return 1;
        }
        else if (!packet1[i].is_null() && packet2[i].is_null()) 
        {   
            std::cout << "Packet 1 is not null, packet 2 is null" << std::endl; 
            return 0;
        }
        else if (packet1[i].is_array() && packet2[i].is_array())
        {
            std::cout << "Both arrays" << std::endl;
            int parsed_array = iterate_packet(packet1[i], packet2[i]);
            if (parsed_array == 1) return 1;
            else if (parsed_array == 0) return 0;
        }
         else if (packet1[i].is_number() && packet2[i].is_array())
        {
            std::cout << "Packet 1 is number, packet 2 is array" << std::endl;
            nlohmann::json::initializer_list_t temp = {packet1[i]};
            int temp_result = iterate_packet(temp, packet2[i]);
            if (temp_result == 1) return 1;
            else if (temp_result == 0) return 0;

        }
        else if (packet1[i].is_array() && packet2[i].is_number())
        {
            std::cout << "Packet 1 is array, packet 2 is number" << std::endl;
            nlohmann::json::initializer_list_t temp = {packet2[i]};
            int temp_result = iterate_packet(packet1[i], temp);
            if (temp_result == 1) return 1;
            else if (temp_result == 0) return 0;
        }
        else if (packet1[i].is_number() && packet1[i].is_number())
        {
            std::cout << "Both numbers" << std::endl;
            if (packet1[i] < packet2[i]) return 1;
            else if (packet1[i] > packet2[i]) return 0;
        }
    }

    return 2;
}

int compare_single_packet_index(std::pair<std::string, std::string> packet)
{
    //Each packet is a json list.
    //This function compares the whole packet

    //Convert to json
    nlohmann::json packet1 = nlohmann::json::parse(packet.first);
    nlohmann::json packet2 = nlohmann::json::parse(packet.second);

    std::cout << "Packet 1: " << packet1 << std::endl;
    std::cout << "Packet 2: " << packet2 << std::endl;

    int largest_packet = packet1.size() > packet2.size() ? packet1.size() : packet2.size();

    return iterate_packet(packet1, packet2);
}

int main()
{
    std::vector<std::pair<std::string, std::string>> input = pack_pairs(store_input("input.txt"));
    std::vector<nlohmann::json> unsorted_packets = json_parsed(store_input("input.txt"));

    int packet_index = 1;
    int packet_index_sum = 0;
    for (auto packet : input)
    {
        std::cout << "\nPacket index: " << packet_index;
        int temp = compare_single_packet_index(packet);
        if (temp == 1) 
        {
            std::cout << " Packet is in correct order" << std::endl; 
            packet_index_sum += packet_index;
        }
        else if (temp == 0) std::cout << " Packet in incorrect order" << std::endl;

        //if (packet_index == 7) break;
        packet_index++;
    }


    //Insert markers
    unsorted_packets.push_back(nlohmann::json::parse("[[2]]"));
    unsorted_packets.push_back(nlohmann::json::parse("[[6]]"));


    //Sort unsorted packets
    std::sort(unsorted_packets.begin(), unsorted_packets.end(), [](nlohmann::json a, nlohmann::json b) { return iterate_packet(a, b) == 1 ? true : false; });

    //Print sorted packets
    // for (auto packet : unsorted_packets)
    // {
    //     std::cout << packet << std::endl;
    // }

    //Find the index of the markers
    int marker1_index = 0;
    int marker2_index = 0;

    for (int i = 0; i < unsorted_packets.size(); i++)
    {
        if (unsorted_packets[i] == nlohmann::json::parse("[[2]]")) marker1_index = i + 1;
        if (unsorted_packets[i] == nlohmann::json::parse("[[6]]")) marker2_index = i + 1;
    }

    std::cout << "Marker 1 index: " << marker1_index << std::endl;
    std::cout << "Marker 2 index: " << marker2_index << std::endl;

    std::cout << packet_index_sum << std::endl;
    std::cout << marker1_index * marker2_index << std::endl;

}