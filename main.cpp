#include "graphics.hpp"
#include <vector>
#include <iostream>

using namespace std;
using namespace genv;

const int screen_size = 800;
const int grid_number = 50, grid_size = screen_size/grid_number;

// kigyo (es reszeinek) haladasi iranyahoz
enum Irany {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

// ÖTLET: kis nyilacskat rajzolni a segment haladasi iranyaba?
struct Segment {
    Segment(int _x, int _y, bool _head, Irany _irany):
        x(_x), y(_y), head(_head), irany(_irany)
    {}

    void rajzol()
    {
        if (head)
            gout << color(0, 255, 0) << move_to(x*grid_size, y*grid_size) << box(grid_size+1, grid_size+1);
        else
            gout << color(255, 255, 255) << move_to(x*grid_size, y*grid_size) << box(grid_size, grid_size);
    }

    void setX(int _x)
    {
        x = _x;
    }

    void setY(int _y)
    {
        y = _y;
    }

private:
    int x, y;
    bool head;
    Irany irany;
};

class Food {
public:
    Food(int _x, int _y):
        x(_x), y(_y)
    {}

    void rajzol()
    {
        gout << color(255, 0, 0) << move_to(x*grid_size, y*grid_size) << box(grid_size, grid_size);
    }

    void place_random()
    {
        x = rand()%grid_number;
        y = rand()%grid_number;
    }

    int getX()
    {
        return x;
    }

    int getY()
    {
        return y;
    }

private:
    int x, y;
};


class Snake {
public:
    Snake()
    {
        int startX = rand()%(grid_number/2) + rand()%(grid_number/2);
        int startY = rand()%(grid_number/2) + rand()%(grid_number/2);
        Segment head(startX, startY, true, UP);
        kigyo.push_back(head);
    }

    void rajzol()
    {
        for (Segment s: kigyo)
            s.rajzol();
    }

    void novel()
    {

    }

    void mozog(Irany ujIrany)
    {

    }

private:
    vector<Segment> kigyo;
};

// kezdo kepernyo: grid size?, start, exit
// game over kepernyo: retry, main menu?, exit

void grid_rajzol()
{
    int w_space = screen_size/grid_number;
    int h_space = screen_size/grid_number;

    for (int i = w_space; i < screen_size; i += w_space)
    {
        gout << color(255, 255, 255) << move_to(i, 0) << line_to(i, screen_size-1);
    }

    for (int i = h_space; i < screen_size; i += h_space)
    {
        gout << color(255, 255, 255) << move_to(0, i) << line_to(screen_size-1, i);
    }
}

int main()
{
    srand(time(0));
    gout.open(screen_size, screen_size);

    Snake kigyo;
    kigyo.rajzol();

    Food food(rand()%grid_number, rand()%grid_number);
    food.rajzol();

    grid_rajzol();
    gout << refresh;

    gin.timer(500);

    event ev;
    while(gin >> ev)
    {
        if (ev.type == ev_timer)
        {
            gout << refresh;
        }

        if (ev.keycode == key_escape)
            exit(0);
    }

    return 0;
}
