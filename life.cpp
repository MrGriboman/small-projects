#include <iostream>
#include <cstdlib>
#include <utility>
#include <vector>
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
    std::vector<std::pair<int, int>> dead_cels;
    std::vector<std::pair<int, int>> born_cels;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int cells_alive = 0;
            for (int k = i - 1; k <= i + 1; k++)
            {
                for (int l = j - 1; l <= j + 1; l++)
                {
                    int cur_height = (k >= 0 && k < height) ? k : ((k < 0) ? height - (-k % height) : 0 + (k % height));
                    int cur_width = (l >= 0 && l < width) ? l : ((l < 0) ? width - (-l % width) : 0 + (l % width));
                    cells_alive += field[cur_height][cur_width];
                }
            }
            cells_alive -= field[i][j];
            if (field[i][j] && (cells_alive < 2 || cells_alive > 3))
            {
                dead_cels.push_back(std::make_pair(i, j));
            }
            else if (!field[i][j] && cells_alive == 3)
            {
                born_cels.push_back(std::make_pair(i, j));
            }
        }
    }
    for (auto it = dead_cels.begin(); it != dead_cels.end(); ++it)
    {
        std::pair<int, int> indices = *it;
        field[indices.first][indices.second] = 0;
    }
    for (auto it = born_cels.begin(); it != born_cels.end(); ++it)
    {
        std::pair<int, int> indices = *it;
        field[indices.first][indices.second] = 1;
    }
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