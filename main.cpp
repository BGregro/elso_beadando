#include "graphics.hpp"
#include <vector>
#include <iostream>

using namespace std;
using namespace genv;

const int screen_size = 600;
const int grid_number = 30, grid_size = screen_size/grid_number;
const color background_color(207,192,126), kigyo_color(24, 145, 21), black(0,0,0), white(255,255,255);

/* kigyo haladasi iranyahoz */
enum Irany {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

enum GameState {
    MAIN_MENU,
    RUNNING,
    GAME_OVER
};

struct Button {
    Button(string _label):
        x(0), y(0), hovered(false), label(_label)
    {
        bw = gout.twidth(_label);
        bh = gout.cascent() + gout.cdescent();
        centerX(); // ebben a programban az osszes gomb kozepre lesz igazitva az X tengelyen
    }

    void draw()
    {
        if (hovered)
            gout << move_to(x-5, y-bh) << color(black) << box(bw+10, bh+5)
                 << move_to(x, y) << color(white) << text(label);
        else
            gout << move_to(x-5, y-bh) << color(white) << box(bw+10, bh+5)
                 << move_to(x, y) << color(black) << text(label);
    }

    void hover(int mx, int my)
    {
        if (mx >= x && mx <= (x+bw+10) && my <= y+5 && my >= (y-bh))
            hovered = true;
        else
            hovered = false;
    }

    void centerX()
    {
        x = (screen_size - bw)/2;
    }

    void setX(int _x)
    {
        x = _x;
    }

    void setY(int _y)
    {
        y = _y;
    }

    int getX()
    {
        return x;
    }

    int getY()
    {
        return y;
    }

    int getHeight()
    {
        return bh;
    }

    int getWidth()
    {
        return bw;
    }

    bool isHovered()
    {
        return hovered;
    }

private:
    int x, y, bw, bh;
    bool hovered;
    string label;
};

struct Segment {
    Segment(int _x, int _y):
        x(_x), y(_y)
    {}

    void rajzol()
    {
        gout << kigyo_color << move_to(x*grid_size+1, y*grid_size+1) << box(grid_size-1, grid_size-1);
    }

    void torol_rajz()
    {
        gout << background_color << move_to(x*grid_size+1, y*grid_size+1) << box(grid_size-1, grid_size-1);
    }

    void setX(int _x)
    {
        x = _x;
    }

    void setY(int _y)
    {
        y = _y;
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

// TODO: collision check fallal
class Snake {
public:
    Snake()
    {
        int startX = grid_number/3 + rand()%(grid_number/3);
        int startY = grid_number/3 + rand()%(grid_number/3);

        kigyo.push_back(Segment(startX, startY));
        irany = UP;
        nextIrany = UP;
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
                nextIrany = uj;
        }
        else
            nextIrany = uj;
    }

    void mozog()
    {
        irany = nextIrany;
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

    bool checkCollision()
    {
        Segment head = kigyo[0];

        if (head.getX() < 0 || head.getY() < 0 ||
            head.getX() >= grid_number || head.getY() >= grid_number)
            return true;

        for (int i = 1; i < kigyo.size(); ++i)
        {
            if ((head.getX() == kigyo[i].getX() && head.getY() == kigyo[i].getY()))
                return true;
        }

        return false;
    }

    void respawn()
    {
        int startX = grid_number/3 + rand()%(grid_number/3);
        int startY = grid_number/3 + rand()%(grid_number/3);

        kigyo.clear();
        kigyo.push_back(Segment(startX, startY));
        irany = UP;
        nextIrany = UP;
    }

private:
    vector<Segment> kigyo;
    Irany irany, nextIrany;
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
            x = rand() % grid_number;
            y = rand() % grid_number;

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

/* ellenorzi, hogy a kigyo rajta van-e a food objektumon es ha igen, akkor noveli */
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

/* kigyo iranyvaltoztatasa gombnyomasra */
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

/* szoveg x tengelyen valo kozepre igazitasahoz */
int centerTextX(string text)
{
    return (screen_size - gout.twidth(text))/2;
}

/* main menu kirajzolasa */
void main_menu(Button &start, Button &exit)
{
    gout << move_to(0,0) << background_color << box(screen_size, screen_size);

    /* gombok elhelyezése és kirajzolása */
    exit.setY(screen_size/2);
    exit.draw();
    start.setY(exit.getY() - start.getHeight() - 10);
    start.draw();

    gout << black << move_to(centerTextX("Snake Game"), start.getY() - start.getHeight() - 10) << text("Snake Game");
}

void start_game(Snake &kigyo, Food &food)
{
    gout << move_to(0,0) << background_color << box(screen_size, screen_size);

    kigyo.rajzol();
    food.spawn(kigyo);
    food.rajzol();
}

void game_over(Button &retry, Button &exit)
{
    gout << move_to(0,0) << background_color << box(screen_size, screen_size);

    exit.draw(); // az exit button-t a program elinditasakor a main menu fuggveny kozepre helyezi, ezert itt ezt nem kell megtenni
    retry.setY(exit.getY() - retry.getHeight() - 10);
    retry.draw();

    gout << black << move_to(centerTextX("Game Over"), retry.getY() - retry.getHeight() - 10) << text("Game Over");
}

// TODO: belso falak letrehozasa -> kovetelmeny
    // fallal collision kulon legyen kezelve

int main()
{
    srand(time(0));
    gout.open(screen_size, screen_size);

    GameState game_state = MAIN_MENU;

    Snake kigyo;
    Food food;

    Button start_button("Start"), retry_button("Retry"), exit_button("Exit");

    main_menu(start_button, exit_button);

    gin.timer(100);

    event ev;
    while(gin >> ev)
    {
        /* menuk gombjainak kezelese es jatek allapotok beallitasa */
        if (ev.type == ev_mouse)
        {
            int mx = ev.pos_x, my = ev.pos_y;
            if (game_state == MAIN_MENU)
            {
                start_button.hover(mx, my);
                start_button.draw();
                exit_button.hover(mx, my);
                exit_button.draw();

                if (ev.button == btn_left)
                {
                    if (start_button.isHovered())
                    {
                        /* A jatek elinditasa */
                        game_state = RUNNING;
                        start_game(kigyo, food);
                    }
                    else if (exit_button.isHovered())
                        exit(0);
                }
            }
            else if (game_state == GAME_OVER)
            {
                retry_button.hover(mx, my);
                retry_button.draw();
                exit_button.hover(mx, my);
                exit_button.draw();

                if (ev.button == btn_left)
                {
                    if (retry_button.isHovered())
                    {
                        game_state = RUNNING;
                        kigyo.respawn();
                        start_game(kigyo, food);
                    }
                    else if (exit_button.isHovered())
                        exit(0);
                }
            }
        }

        /* jatek futtatasa */
        if (ev.type == ev_timer && game_state == RUNNING)
        {
            eves(kigyo, food);

            kigyo.torol_rajz();
            kigyo.mozog();
            kigyo.rajzol();

            if (kigyo.checkCollision())
            {
                game_state = GAME_OVER;
                game_over(retry_button, exit_button);
                continue;
            }
        }

        gout << refresh;

        if (ev.type == ev_key)
            changeIrany(kigyo, ev);

        if (ev.keycode == key_escape)
            exit(0);
    }

    return 0;
}

/* Otletek */
// game over kepernyo gombok: retry, main menu?, exit
// amikor kimegy a kigyo a falon kivulre vagy nekimegy a testenek, akkor maradjon kirajzolodva a vesztes allapot



