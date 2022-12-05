def parse_file():
    with open("Puzzle1/input.txt") as file:
        
        elf_sum = 0
        total_calories = []
        for line in file:
            if not line.isspace():
                elf_sum += int(line)
            else:
                total_calories.append(elf_sum)
                elf_sum = 0
        
        total_calories.append(elf_sum)
                
        total_calories.sort(reverse=True)
        return total_calories[0] + total_calories[1] + total_calories[2]
                  
print(parse_file())