#include "graphics.hpp"
#include <vector>
#include <iostream>

using namespace std;
using namespace genv;

const int screen_size = 800;
const int grid_number = 40, grid_size = screen_size/grid_number;

// kigyo (es reszeinek) haladasi iranyahoz
enum Irany {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

struct Segment {
    Segment(int _x, int _y):
        x(_x), y(_y)
    {}

    void rajzol()
    {
        gout << color(255, 255, 255) << move_to(x*grid_size+1, y*grid_size+1) << box(grid_size-1, grid_size-1);
    }

    // ezt mashogy megoldani?
    void torol_rajz()
    {
        gout << color(0, 0, 0) << move_to(x*grid_size+1, y*grid_size+1) << box(grid_size-1, grid_size-1);
    }

    /* setter fv-ek */
    void setX(int _x)
    {
        x = _x;
    }

    void setY(int _y)
    {
        y = _y;
    }

    /* getter fv-ek */
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


// TODO: collision check sajat magaval, szelevel es fallal
class Snake {
public:
    Snake()
    {
        int startX = grid_number/3 + rand()%(grid_number/3);
        int startY = grid_number/3 + rand()%(grid_number/3);

        kigyo.push_back(Segment(startX, startY));
        irany = UP;
    }

    vector<Segment> getBody()
    {
        return kigyo;
    }

    Segment getHead()
    {
        return kigyo[0];
    }

    void rajzol()
    {
        for (Segment s: kigyo)
            s.rajzol();
    }

    void torol_rajz()
    {
        for (Segment s: kigyo)
            s.torol_rajz();
    }

    void novel()
    {
        kigyo.push_back(kigyo.back());
    }

    void ujIrany(Irany uj)
    {
        if (kigyo.size() > 1)
        {
            if ((irany == UP && uj != DOWN) ||
                (irany == DOWN && uj != UP) ||
                (irany == LEFT && uj != RIGHT) ||
                (irany == RIGHT && uj != LEFT))
                irany = uj;
        }
        else
            irany = uj;
    }

    void mozog()
    {
        Segment newHead = kigyo[0];

        switch (irany)
        {
        case UP:
            newHead.setY(newHead.getY()-1);
            break;
        case DOWN:
            newHead.setY(newHead.getY()+1);
            break;
        case LEFT:
            newHead.setX(newHead.getX()-1);
            break;
        case RIGHT:
            newHead.setX(newHead.getX()+1);
            break;
        }

        kigyo.insert(kigyo.begin(), newHead);
        kigyo.pop_back();
    }

private:
    vector<Segment> kigyo;
    Irany irany;
};

class Food {
public:
    Food()
    {}

    void rajzol()
    {
        gout << color(255, 0, 0) << move_to(x*grid_size+1, y*grid_size+1) << box(grid_size-1, grid_size-1);
    }

    void spawn(Snake kigyo)
    {
        bool validSpawn = false;

        while (!validSpawn)
        {
            x = rand()%(grid_number+1);
            y = rand()%(grid_number+1);

            validSpawn = true;
            for (Segment s: kigyo.getBody())
            {
                if (x == s.getX() && y == s.getY())
                {
                    validSpawn = false;
                    break;
                }
            }
        }
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

void grid_rajzol()
{
    int w_space = screen_size/grid_number;
    int h_space = screen_size/grid_number;

    for (int i = w_space; i < screen_size; i += w_space)
        gout << color(255, 255, 255) << move_to(i, 0) << line_to(i, screen_size-1);

    for (int i = h_space; i < screen_size; i += h_space)
        gout << color(255, 255, 255) << move_to(0, i) << line_to(screen_size-1, i);
}

void eves(Snake &kigyo, Food &food)
{
    if (kigyo.getHead().getX() == food.getX() &&
        kigyo.getHead().getY() == food.getY())
    {
        kigyo.novel();
        food.spawn(kigyo);
        food.rajzol();
    }
}

void changeIrany(Snake &kigyo, event ev)
{
    switch (ev.keycode)
    {
    case key_up:
        kigyo.ujIrany(UP);
        break;
    case key_down:
        kigyo.ujIrany(DOWN);
        break;
    case key_left:
        kigyo.ujIrany(LEFT);
        break;
    case key_right:
        kigyo.ujIrany(RIGHT);
        break;
    }
}

// TODO: belso falak letrehozasa -> kovetelmeny
// TODO: food-al bug a palya szele fele

int main()
{
    srand(time(0));
    gout.open(screen_size, screen_size);

    Snake kigyo;
    kigyo.rajzol();

    Food food;
    food.spawn(kigyo);
    food.rajzol();

    grid_rajzol();
    gout << refresh;

    gin.timer(150);

    event ev;
    while(gin >> ev)
    {
        if (ev.type == ev_timer)
        {
            kigyo.torol_rajz();
            kigyo.mozog();

            eves(kigyo, food);

            kigyo.rajzol();

            gout << refresh;
        }

        if (ev.type == ev_key)
            changeIrany(kigyo, ev);

        if (ev.keycode == key_escape)
            exit(0);
    }

    return 0;
}

/* Otletek */
// kezdo kepernyo: grid size?, start, exit
// game over kepernyo: retry, main menu?, exit
// kigyo feje legyen mas szinu?






