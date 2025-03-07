#include "graphics.hpp"
#include <vector>
#include <iostream>

using namespace std;
using namespace genv;

const int screen_size = 800;
const int grid_number = 50, grid_size = screen_size/grid_number;

struct Segment {
    Segment(int _x, int _y, bool _head):
        x(_x), y(_y), head(_head)
    {}

    void rajzol()
    {
        if (head)
            gout << color(0, 255, 0) << move_to(x*grid_size, y*grid_size) << box(grid_size+1, grid_size+1);
        else
            gout << color(255, 255, 255) << move_to(x*grid_size, y*grid_size) << box(grid_size, grid_size);
    }

    void mozog()
    {

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
};

struct Food {
    Food(int _x, int _y):
        x(_x), y(_y)
    {}

private:
    int x, y;
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

    //vector<Segment*> kigyo;

    // első szegmens random elhelyezése
    // TODO: a közepe fele helyezze el (kell random?)
    Segment kigyo(rand()%grid_number, rand()%grid_number, false);
    kigyo.rajzol();

    grid_rajzol();
    gout << refresh;

    gin.timer(500);

    event ev;
    while(gin >> ev)
    {
        if (ev.type == ev_timer)
        {
            kigyo.setX(rand()%grid_number);
            kigyo.setY(rand()%grid_number);
            kigyo.rajzol();
            gout << refresh;
        }

        if (ev.keycode == key_escape)
            exit(0);
    }

    return 0;
}
