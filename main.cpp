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
            gout << color(0, 255, 0) << move_to(x*grid_size+1, y*grid_size+1) << box(grid_size-1, grid_size-1);
        else
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

    void setIrany(Irany ujIrany)
    {
        irany = ujIrany;
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

    Irany getIrany()
    {
        return irany;
    }

private:
    int x, y;
    bool head; // ezt csak arra hasznalom, hogy a fej mas szinu legyen
    Irany irany;
};

class Snake {
public:
    Snake()
    {
        int startX = rand()%(grid_number/2) + rand()%(grid_number/2);
        int startY = rand()%(grid_number/2) + rand()%(grid_number/2);

        kigyo.push_back(new Segment(startX, startY, true, UP));
    }

    vector<Segment*> getBody()
    {
        return kigyo;
    }

    Segment* getHead()
    {
        return kigyo[0];
    }

    void rajzol()
    {
        for (Segment *s: kigyo)
            s->rajzol();
    }

    void torol_rajz()
    {
        for (Segment *s: kigyo)
            s->torol_rajz();
    }

    void novel()
    {
        kigyo.push_back(new Segment(kigyo.back()->getX(), kigyo.back()->getY(), false, kigyo.back()->getIrany()));
    }

    void ujIrany(Irany uj)
    {
        Irany fejIrany = kigyo[0]->getIrany();

        if (kigyo.size() > 1)
        {
            if ((fejIrany == UP && uj != DOWN) ||
                (fejIrany == DOWN && uj != UP) ||
                (fejIrany == LEFT && uj != RIGHT) ||
                (fejIrany == RIGHT && uj != LEFT))
                kigyo[0]->setIrany(uj);
        }
        else
            kigyo[0]->setIrany(uj);
    }

    void mozog()
    {
        for (int i = 0; i < kigyo.size(); ++i)
        {
            /* segment-ek mozgatasa a megfelelo iranyba */
            if (kigyo[i]->getIrany() == UP)
                kigyo[i]->setY(kigyo[i]->getY()-1);

            else if (kigyo[i]->getIrany() == DOWN)
                kigyo[i]->setY(kigyo[i]->getY()+1);

            else if (kigyo[i]->getIrany() == LEFT)
                kigyo[i]->setX(kigyo[i]->getX()-1);

            else if (kigyo[i]->getIrany() == RIGHT)
                kigyo[i]->setX(kigyo[i]->getX()+1);

            /* minden segment atveszi az elotte levonek az iranyat */
            if (i > 0)
                kigyo[i]->setIrany(kigyo[i-1]->getIrany());
        }
    }

private:
    vector<Segment*> kigyo;
    //Segment* head; // ez segítene atlathatobba tenni a programot (kajalas ellenorzeseben stb)
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
            x = rand()%grid_number;
            y = rand()%grid_number;

            validSpawn = true;
            for (Segment *s: kigyo.getBody())
            {
                if (x == s->getX() && y == s->getY())
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

void eves(Snake &kigyo, Food &food)
{
    if (kigyo.getHead()->getX() == food.getX() &&
        kigyo.getHead()->getY() == food.getY())
    {
        kigyo.novel();
        food.spawn(kigyo);
        food.rajzol();
    }
}

int main()
{
    srand(time(0));
    gout.open(screen_size, screen_size);

    Snake kigyo;
    kigyo.rajzol();

    Food food;
    food.spawn(kigyo);
    food.rajzol();

    // TODO: kezelni, hogy a kaja és a kigyo mas helyre spawn-oljon
    // TODO: jatek kozben a food ne spawnoljon a kigyoba

    grid_rajzol();
    gout << refresh;

    gin.timer(200);

    event ev;
    while(gin >> ev)
    {
        if (ev.type == ev_timer)
        {
            kigyo.torol_rajz();
            kigyo.mozog();

            eves(kigyo, food);

            cout << kigyo.getBody().size();

            kigyo.rajzol();

            gout << refresh;
        }

        if (ev.type == ev_key)
        {
            // ezt kulon fv-be? + switch-case?
            if (ev.keycode == key_up)
                kigyo.ujIrany(UP);
            else if (ev.keycode == key_down)
                kigyo.ujIrany(DOWN);
            else if (ev.keycode == key_left)
                kigyo.ujIrany(LEFT);
            else if (ev.keycode == key_right)
                kigyo.ujIrany(RIGHT);
        }

        if (ev.keycode == key_escape)
            exit(0);
    }

    return 0;
}

// Ötlet -> getHead fv kigyohoz (collision check + kaja check) -> csináljam meg a head-et külön
