class Tile
{
    public:
        Tile(int x, int y, char type)
        {
            this->x = x;
            this->y = y;
            this->type = type;
        }

        int x;
        int y;
        char type;


    void set_rock()
    {
        this->type = '#';
    }

    void set_sand()
    {
        this->type = 'O';
    }
};