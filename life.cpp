#include <iostream>
#include <cstdlib>
#include <unistd.h>

void initialize_glider(bool **field, int height, int width)
{
    // Clear the field
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            field[i][j] = 0;
        }
    }

    // Place the glider pattern at the top-left corner
    if (height > 2 && width > 2)
    {
        field[0][1] = 1;
        field[1][2] = 1;
        field[2][0] = 1;
        field[2][1] = 1;
        field[2][2] = 1;
    }
}

void update_game_field(bool **field, int height, int width)
{
    bool **next_field = new bool *[height];
    for (int i = 0; i < height; ++i)
    {
        next_field[i] = new bool[width]();
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int cells_alive = 0;

            for (int k = -1; k <= 1; k++)
            {
                for (int l = -1; l <= 1; l++)
                {
                    if (k == 0 && l == 0)
                        continue; // Skip the current cell

                    int cur_height = (i + k + height) % height;
                    int cur_width = (j + l + width) % width;

                    cells_alive += field[cur_height][cur_width];
                }
            }

            if (field[i][j]) // Cell is alive
            {
                next_field[i][j] = (cells_alive == 2 || cells_alive == 3);
            }
            else // Cell is dead
            {
                next_field[i][j] = (cells_alive == 3);
            }
        }
    }

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            field[i][j] = next_field[i][j];
        }
        delete[] next_field[i];
    }
    delete[] next_field;
}

void draw_game_field(bool **field, int height, int width)
{
    std::cout << std::endl;
    unsigned char cell = 254; // ■
    for (int i = 0; i < height; ++i)
    {
        std::cout << "|";
        for (int j = 0; j < width; ++j)
        {
            std::cout << (field[i][j] ? '#' : ' ');
        }
        std::cout << "|" << std::endl;
    }
}

int main(int argc, char **argv)
{
    int height = 10;
    int width = 10;
    if (argc == 3)
    {
        height = atoi(argv[1]);
        width = atoi(argv[2]);
    }
    bool **field = new bool *[height];
    for (int i = 0; i < height; ++i)
    {
        field[i] = new bool[width]();
    }
    initialize_glider(field, height, width);

    while (true)
    {
        draw_game_field(field, height, width);
        update_game_field(field, height, width);
        usleep(100000);
        printf("\033[2J");
        printf("\033[H");
    }
    return 0;
}