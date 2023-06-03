#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "map.h"
#include "map1.h"
#include <cmath>
#include <list>
#include <windows.h>

using namespace sf;
int NPCDIAL=0,quest1=1,quest2=0,Lvl = 1,track=0,track1=0;
class Entity {
public:
    enum { left, right, up, down, stay, upleft, upright, downleft, downright} state;
    float dx, dy, x, y, x1, y1, speed, moveTimer,Mana, StoneKD;
    int w, h, health;
    bool life;
    Texture texture;
    Sprite sprite;
    float CurrentFrame;
    std::string name;//враги могут быть разные, врагов можно различать по именам
    //каждому можно дать свое действие в update() в зависимости от имени

    Entity(Image &image, float X, float Y, int W, int H, std::string Name){
        x = X; y = Y; //координата появления спрайта
        w = W; h = H;
        name = Name;
        moveTimer = 0;
        dx = 0; dy = 0;
        speed = 0;
        CurrentFrame = 0;
        health = 100;
        Mana = 10;//задаем значение манки
        StoneKD = 1;
        life = true; //инициализировали логическую переменную жизни, герой жив
        texture.loadFromImage(image); //заносим наше изображение в текстуру
        sprite.setTexture(texture); //заливаем спрайт текстурой
    }
    FloatRect getRect(){//метод получения прямоугольника. его коорд, размеры (шир,высот).
        FloatRect FR(x, y, w, h); // переменная FR типа FloatRect
        return FR;
        //Тип данных (класс) "sf::FloatRect" позволяет хранить четыре координаты прямоугольника
        //в нашей игре это координаты текущего расположения тайла на карте
        //далее это позволит спросить, есть ли ещё какой-либо тайл на этом месте
        //эта ф-ция нужна для проверки пересечений
    }
    virtual void update(float time) = 0;
};
////////////////////////////КЛАСС ИГРОКА////////////////////////
class Player :public Entity {
public:
    int playerScore;//эта переменная может быть только у игрока
    Player(Image &image, float X, float Y, int W, int H, std::string Name) :Entity(image, X,
                                                                                   Y, W, H, Name){
        playerScore = 0;
        state = stay;
        if (name == "Player1"){
            //Задаем спрайту один прямоугольник для
            //вывода одного игрока. IntRect – для приведения типов
            sprite.setTextureRect(IntRect(0, 0, w, h));
        }
    }

    void control(){
        if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A)) {
            state = left;
            speed = 0.1;
        }
        if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D)) {
            state = right;
            speed = 0.1;
        }
        if (Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W) ) {
            state = up;
            speed = 0.1;
        }
        if (Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S)) {
            state = down;
            speed = 0.1;
        }
        if ((Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W))&&(Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A))) {
            state = upleft;
            speed = 0.1;
        }

        if ((Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S))&&(Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A))) {
            state = downleft;
            speed = 0.1;
        }
        if ((Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W))&&(Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D))) {
            state = upright;
            speed = 0.1;
        }
        if ((Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S))&&(Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D))) {
            state = downright;
            speed = 0.1;
        }
    }
    //Метод проверки столкновений с элементами карты
    void checkCollisionWithMap2(float Dx, float Dy) {
        for (int i = y / 32; i < (y + h) / 32; i++)//проходимся по элементам карты
            for (int j = x / 32; j<(x + w) / 32; j++)
            {
                if (Map[i][j] == '0')//если элемент тайлик земли
                {
                    if (Dy > 0) { y = i * 32 - h; dy = 0; }//по Y
                    if (Dy < 0) { y = i * 32 + 32; dy = 0; }//столкновение с верхними краями
                    if (Dx > 0) { x = j * 32 - w; dx = 0; }//с правым краем карты
                    if (Dx < 0) { x = j * 32 + 32; dx = 0; }// с левым краем карты
                }
                if (Map[i][j] == 'h') {
                    Map[i][j] = ' ';
                    health += 25;


                }
            }
    }

    void checkCollisionWithMap(float Dx, float Dy) {
        for (int i = y / 32; i < (y + h) / 32; i++)//проходимся по элементам карты
            for (int j = x / 32; j<(x + w) / 32; j++)
            {
                if (TileMap[i][j] == '0')//если элемент тайлик земли
                {
                    if (Dy > 0) { y = i * 32 - h; dy = 0; }//по Y
                    if (Dy < 0) { y = i * 32 + 32; dy = 0; }//столкновение с верхними краями
                    if (Dx > 0) { x = j * 32 - w; dx = 0; }//с правым краем карты
                    if (Dx < 0) { x = j * 32 + 32; dx = 0; }// с левым краем карты
                }
                if (TileMap[i][j] == 'C' ) { //закрытый сундук

                    TileMap[i][j] = 'N';//открытый сундук
                    TileMap[i][j+1] = 'n';//открытый сундук
                    TileMap[i+1][j] = 'U';//открытый сундук
                    TileMap[i+1][j+1] = 'u';//открытый сундук
                    TileMap[i][j-2] = 'h'; // Хилка
                }
                if (TileMap[i][j] == 'h') {
                    TileMap[i][j] = ' ';
                    health += 25;
                    quest1=3;

                }
                if(TileMap[i][j] == 'l' && quest1 >= 3){
                    TileMap[i][j-1] = 'V';
                    TileMap[i][j] = 'v';
                    Lvl++;
                }
            }
    }
    void update(float time) //метод "оживления/обновления" объекта класса.
    {
        if (life) {//проверяем, жив ли герой
            control();//функция управления персонажем
            switch (state)//делаются различные действия в зависимости от состояния
            {
            case right:{//состояние идти вправо
                dx = speed;
                CurrentFrame += 0.005*time;
                if (CurrentFrame > 3) CurrentFrame -= 3;
                sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 192, 96, 96));
                break;
            }
            case left:{//состояние идти влево
                dx = -speed;
                CurrentFrame += 0.005*time;
                if (CurrentFrame > 3) CurrentFrame -= 3;
                sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 96, 96, 96));
                break;
            }
            case up:{//идти вверх
                dy = -speed;
                CurrentFrame += 0.005*time;
                if (CurrentFrame > 3) CurrentFrame -= 3;
                sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 288, 96, 96));
                break;
            }
            case down:{//идти вниз
                dy = speed;
                CurrentFrame += 0.005*time;
                if (CurrentFrame > 3) CurrentFrame -= 3;
                sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 0, 96, 96));
                break;
            }
            case upright:{//состояние идти вправо
                dx = speed;
                dy = -speed;
                CurrentFrame += 0.005*time;
                if (CurrentFrame > 3) CurrentFrame -= 3;
                sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 288, 96, 96));
                break;
            }
            case upleft:{//состояние идти влево
                dx = -speed;
                dy = -speed;
                CurrentFrame += 0.005*time;
                if (CurrentFrame > 3) CurrentFrame -= 3;
                sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 288, 96, 96));
                break;
            }
            case downright:{//идти вверх
                dy = speed;
                dx = speed;
                CurrentFrame += 0.005*time;
                if (CurrentFrame > 3) CurrentFrame -= 3;
                sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 0, 96, 96));
                break;
            }
            case downleft:{//идти вниз
                dy = speed;
                dx = -speed;
                CurrentFrame += 0.005*time;
                if (CurrentFrame > 3) CurrentFrame -= 3;
                sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 0, 96, 96));
                break;
            }
            case stay:{//стоим
                dy = speed;
                dx = speed;
                break;
            }
            }
            x += dx*time; //движение по “X”
            if (Lvl == 1)
            checkCollisionWithMap(dx, 0);
            if (Lvl == 2)
            checkCollisionWithMap2(dx, 0);//обрабатываем столкновение по Х
            y += dy*time; //движение по “Y”
            if (Lvl == 1)
            checkCollisionWithMap(0, dy);
            if (Lvl == 2)
            checkCollisionWithMap2(0, dy);//обрабатываем столкновение по Y
            speed = 0; //обнуляем скорость, чтобы персонаж остановился.
            //state = stay;
            dx = 0;
            dy = 0;
            sprite.setPosition(x, y); //спрайт в позиции (x, y).
            if (health <= 0){ life = false; }//если жизней меньше 0, либо равно 0, то умираем
        }
    }
};
////////////////////////////КЛАСС ВРАГА////////////////////////
class Enemy :public Entity{
public:
    int direction;//направление движения врага
    Enemy(Image &image, float X, float Y, int W, int H, std::string Name) :Entity(image, X,
                                                                                  Y, W, H, Name){
        if (name == "EasyEnemy"){
            //Задаем спрайту один прямоугольник для
            //вывода одного игрока. IntRect – для приведения типов
            sprite.setTextureRect(IntRect(0, 0, w, h));
            direction = rand() % (3); //Направление движения врага задаём случайным образом
            //через генератор случайных чисел
            speed = 0.1;//даем скорость.этот объект всегда двигается
            dx = speed;
        }
    }
    void checkCollisionWithMap(float Dx, float Dy)//ф-ция проверки столкновений с картой
    {
        for (int i = y / 32; i < (y + h) / 32; i++)//проходимся по элементам карты
            for (int j = x / 32; j<(x + w) / 32; j++)
            {
                if (TileMap[i][j] == '0')//если элемент - тайлик земли
                {
                    if (Dy > 0) {
                        y = i * 32 - h; dy = -0.1;
                        direction = rand() % (3); //Направление движения врага
                    }//по Y
                    if (Dy < 0) {
                        y = i * 32 + 32; dy = 0.1;
                        direction = rand() % (3);//Направление движения врага
                    }//столкновение с верхними краями
                    if (Dx > 0) {
                        x = j * 32 - w; dx = -0.1;
                        direction = rand() % (3);//Направление движения врага
                    }//с правым краем карты
                    if (Dx < 0) {
                        x = j * 32 + 32; dx = 0.1;
                        direction = rand() % (3); //Направление движения врага
                    }// с левым краем карты
                }
            }
    }
    void update(float time)
    {
        if (name == "EasyEnemy"){//для персонажа с таким именем логика будет такой
            if (life) {//проверяем, жив ли герой
                switch (direction)//делаются различные действия в зависимости от состояния
                {
                case 0:{//состояние идти вправо
                    dx = speed;
                    CurrentFrame += 0.005*time;
                    if (CurrentFrame > 3) CurrentFrame -= 3;
                    sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 192, 96, 96));
                    break;
                }
                case 1:{//состояние идти влево
                    dx = -speed;
                    CurrentFrame += 0.005*time;
                    if (CurrentFrame > 3) CurrentFrame -= 3;
                    sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 96, 96, 96));
                    break;
                }
                case 2:{//идти вверх
                    dy = -speed;
                    CurrentFrame += 0.005*time;
                    if (CurrentFrame > 3) CurrentFrame -= 3;
                    sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 288, 96, 96));
                    break;
                }
                case 3:{//идти вниз
                    dy = speed;
                    CurrentFrame += 0.005*time;
                    if (CurrentFrame > 3) CurrentFrame -= 3;
                    sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 0, 96, 96));
                    break;
                }
                }
                x += dx*time; //движение по “X”

                checkCollisionWithMap(dx, 0);//обрабатываем столкновение по Хy += dy*time;
                y += dy*time;//движение по “Y”
                checkCollisionWithMap(0, dy);//обрабатываем столкновение по Y
                sprite.setPosition(x, y); //спрайт в позиции (x, y).
                if (health <= 0){ life = false; }//если жизней меньше 0, либо равно 0, то умираем
            }
        }
    }
};//класс Enemy закрыт
////////////////////////////КЛАСС ПУЛИ////////////////////////

class Boss :public Entity{
public:
    int direction;//направление движения врага
    Boss(Image &image, float X, float Y, int W, int H, std::string Name) :Entity(image, X,
                                                                                 Y, W, H, Name){
        if (name == "hardBoss"){
            //Задаем спрайту один прямоугольник для
            //вывода одного игрока. IntRect – для приведения типов
            sprite.setTextureRect(IntRect(0, 0, w, h));
            direction = rand() % (3); //Направление движения врага задаём случайным образом
            //через генератор случайных чисел
            speed = 0.15;//даем скорость.этот объект всегда двигается
            dx = speed;
        }
    }
    void checkCollisionWithMap(float Dx, float Dy)//ф-ция проверки столкновений с картой
    {
        for (int i = y / 32; i < (y + h) / 32; i++)//проходимся по элементам карты
            for (int j = x / 32; j<(x + w) / 32; j++)
            {
                if (Map[i][j] == '0')//если элемент - тайлик земли
                {
                    if (Dy > 0) {
                        y = i * 32 - h; dy = -0.1;
                        direction = rand() % (3); //Направление движения врага
                    }//по Y
                    if (Dy < 0) {
                        y = i * 32 + 32; dy = 0.1;
                        direction = rand() % (3);//Направление движения врага
                    }//столкновение с верхними краями
                    if (Dx > 0) {
                        x = j * 32 - w; dx = -0.1;
                        direction = rand() % (3);//Направление движения врага
                    }//с правым краем карты
                    if (Dx < 0) {
                        x = j * 32 + 32; dx = 0.1;
                        direction = rand() % (3); //Направление движения врага
                    }// с левым краем карты
                }
            }
    }
    void update(float time)
    {
        if (name == "hardBoss"){//для персонажа с таким именем логика будет такой
            if (life) {//проверяем, жив ли герой
                switch (direction)//делаются различные действия в зависимости от состояния
                {
                case 0:{//состояние идти вправо
                    dx = speed;
                    CurrentFrame += 0.005*time;
                    if (CurrentFrame > 3) CurrentFrame -= 3;
                    sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 192, 96, 96));
                    break;
                }
                case 1:{//состояние идти влево
                    dx = -speed;
                    CurrentFrame += 0.005*time;
                    if (CurrentFrame > 3) CurrentFrame -= 3;
                    sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 96, 96, 96));
                    break;
                }
                case 2:{//идти вверх
                    dy = -speed;
                    CurrentFrame += 0.005*time;
                    if (CurrentFrame > 3) CurrentFrame -= 3;
                    sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 288, 96, 96));
                    break;
                }
                case 3:{//идти вниз
                    dy = speed;
                    CurrentFrame += 0.005*time;
                    if (CurrentFrame > 3) CurrentFrame -= 3;
                    sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 0, 96, 96));
                    break;
                }
                }
                x += dx*time; //движение по “X”

                checkCollisionWithMap(dx, 0);//обрабатываем столкновение по Хy += dy*time;
                y += dy*time;//движение по “Y”
                checkCollisionWithMap(0, dy);//обрабатываем столкновение по Y
                sprite.setPosition(x, y); //спрайт в позиции (x, y).
                if (health <= 0){ life = false; }//если жизней меньше 0, либо равно 0, то умираем
            }
        }
    }
};//класс Boss закрыт

class Bullet :public Entity{//класс пули
public:
    int direction;//направление пули
    //всё так же, только взяли в конце состояние игрока (int dir)
    //для задания направления полёта пули
    Bullet(Image &image, float X, float Y, int W, int H, std::string Name, int dir)
        :Entity(image, X, Y, W, H, Name){
        x = X;
        y = Y;
        direction = dir;
        speed = 0.8;
        w = h = 16;
        life = true;
        //выше инициализация в конструкторе
    }
    void update(float time)
    {
        switch (direction)
        {
        case 0: dx = -speed; dy = 0; break;// state = left
        case 1: dx = speed; dy = 0; break;// state = right
        case 2: dx = 0; dy = -speed; break;// state = up
        case 3: dx = 0; dy = speed; break;// state = down
        case 4: dx = speed; dy = -speed; break;// state = left
        case 5: dx = -speed/2; dy = -speed/2; break;// state = upright
        case 6: dx = speed/2; dy = -speed/2; break;// state = upleft
        case 7: dx = -speed/2; dy = speed/2; break;// state = downleft
        case 8: dx = speed/2; dy = speed/2; break;// state = downright
        }
        if (life){
            x += dx*time;//само движение пули по х
            y += dy*time;//по у
            if (x <= 0) x = 20;// задержка пули в левой стене, чтобы при проседании кадров она случайно не вылетела за предел карты и не было ошибки (сервер может тормозить!)
            if (y <= 0) y = 20;
            if (x >= 800) x = 780;// задержка пули в правой стене, чтобы при проседании
            // кадров она случайно не вылетела за предел карты и не было ошибки (сервер может тормозить!)
            if (y >= 800) y = 780;
            for (int i = y / 32; i < (y + h) / 32; i++)//проходимся по элементам карты
                for (int j = x / 32; j < (x + w) / 32; j++)
                {
                    if (TileMap[i][j] == '0' || TileMap[i][j] == 'L' || TileMap[i][j] == 'l')//если элемент наш тайлик земли, то
                        life = false;// то пуля умирает
                    if (Map[i][j] == '0' || Map[i][j] == 'L' || Map[i][j] == 'l')//если элемент наш тайлик двери, то
                        life = false;// то пуля умирает
                }
            sprite.setPosition(x + w / 2, y + h / 2);//задается позицию пули
        }
    }
};


class Stone :public Entity{//класс камня
public:
    int direction;
    Stone(Image &image, float X, float Y, int W, int H, std::string Name, int dir)
        :Entity(image, X, Y, W, H, Name){
        x = X;
        y = Y;
        x1 = X;
        y1 = Y;
        direction = dir;
        speed = 0.8;
        w = h = 16;
        life = true;
    }
    void update(float time)
    {

        switch (direction)
        {
        case 0: dx = -speed; dy = 0; break;// state = left
        case 1: dx = speed; dy = 0; break;// state = right
        case 2: dx = 0; dy = -speed; break;// state = up
        case 3: dx = 0; dy = speed; break;// state = down
        case 4: dx = speed; dy = -speed; break;// state = left
        case 5: dx = -speed/2; dy = -speed/2; break;// state = upright
        case 6: dx = speed/2; dy = -speed/2; break;// state = upleft
        case 7: dx = -speed/2; dy = speed/2; break;// state = downleft
        case 8: dx = speed/2; dy = speed/2; break;// state = downright
        }

        if (life){

            x += dx*1.1;
            y += dy*1.1;
            if (x <= 0) x = 20;
            if (y <= 0) y = 20;
            if (x >= 800) x = 780;
            if (y >= 800) y = 780;
            for (int i = y / 32; i < (y + h) / 32; i++)
                for (int j = x / 32; j < (x + w) / 32; j++)
                {
                    if ((TileMap[i][j] == '0'))
                        life = false;
                    if ((Map[i][j] == '0'))
                        life = false;
                    if ((abs(x-x1)>100)||(abs(y-y1)>100))
                        life = false;
                }
            sprite.setPosition(x + w / 2, y + h / 2);
        }
    }
};

class Effect :public Entity{
public:
    int lifetime;
    Effect(Image &image, float X, float Y, int W, int H, std::string Name, int lftime)
        :Entity(image, X, Y, W, H, Name){
        x = X;
        y = Y;
        w = h = 64;
        life = true;
        lifetime = lftime;
        sprite.setScale(sf::Vector2f(2,2));
    }
    void update(float time)
    {
        sprite.setPosition(x + w / 2, y + h / 2);
        if (lifetime != 0)
            lifetime -= 0.1;
        else
            life = false;
    }
};

void menu(sf::RenderWindow &window) {
    Texture menuTexture1, menuBackground;
    menuTexture1.loadFromFile("images/111.png");
    menuBackground.loadFromFile("images/witcher.png");
    Sprite menu1(menuTexture1), menuBg(menuBackground);
    menuBg.setScale(sf::Vector2f(2,2));
    bool isMenu = 1;
    int menuNum = 0;
    menu1.setPosition(320, 430);
    menuBg.setPosition(0, 0);

    //////////////////////////////МЕНЮ///////////////////
    while (isMenu)
    {
        menu1.setColor(Color::White);
        menuNum = 0;
        window.clear(Color(129, 181, 221));

        if (IntRect(320, 430, 300, 50).contains(Mouse::getPosition(window))) { menu1.setColor(Color::Blue); menuNum = 1; }

        if (Mouse::isButtonPressed(Mouse::Left))
        {
            if (menuNum == 1) {isMenu = false; isMenu = false; window.close();}

        }

        window.draw(menuBg);
        window.draw(menu1);

        window.display();

    }
}


int main()
{
    float invinctime = 0;//объявляем переменную времени неуязвмости, можно объявить её в Entity, напомните ёё перенести
    float dialnum=0; // номер диалога
    float dialtime=0; //время для учета кулдауна на прочтение диалога
    bool isdial = 0;
    bool timebool = 1; //переменная, чтобы dialtimeest отнимался по 1 а не по 100
    int dialtimeest = 5; //время для вывода на чтение диалога
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::RenderWindow windowm(sf::VideoMode(800, 800), "Main Menu");
    menu(windowm);
    sf::SoundBuffer buffer;
    buffer.loadFromFile("fireball.ogg");
    sf::Sound sound;
    sound.setBuffer(buffer);
    sf::SoundBuffer buffer1;
    buffer1.loadFromFile("rock.ogg");
    sf::Sound sound1;
    sound1.setBuffer(buffer1);
    sf::Music ost1;
    ost1.openFromFile("music1.ogg");
    sf::Music ost;
    ost.openFromFile("music.ogg");
    sf::RenderWindow window(sf::VideoMode(800, 800, desktop.bitsPerPixel), "The Witcher 4");
    window.setKeyRepeatEnabled(false);
    Font font;//шрифт
    font.loadFromFile("CyrilicOld.ttf");//передаем нашему шрифту файл шрифта
    Text text("", font, 20);//создаем объект текст
    text.setStyle(Text::Bold);//жирный текст.

    Image map_image;//объект изображения для карты
    map_image.loadFromFile("images/map_new.png");//загружаем файл для карты
    Texture map;//текстура карты
    map.loadFromImage(map_image);//заряжаем текстуру картинкой
    Sprite s_map;//создаём спрайт для карты
    s_map.setTexture(map);//заливаем текстуру спрайтом

    Clock clock;
    Clock gameTimeClock;//переменная игрового времени, будем здесь хранить время игры
    int gameTime = 0;//объявили игровое время, инициализировали.

    Image heroImage;
    heroImage.loadFromFile("images/hero.png"); // загружаем изображение игрока
    Image easyEnemyImage;
    easyEnemyImage.loadFromFile("images/enemy.png"); // загружаем изображение врага
    Image hardBossImage;
    hardBossImage.loadFromFile("images/boss.png"); // загружаем изображение врага
    Image hit;
    hit.loadFromFile("images/hit.png");
    Image BulletImage;//изображение для пули
    BulletImage.loadFromFile("images/bullet.png");//загрузили картинку в объект изображения

    Image StoneImage;//изображение для пули
    StoneImage.loadFromFile("images/stone.png");//загрузили картинку в объект изображения
    Player p(heroImage, 100, 100, 96, 96, "Player1");//объект класса игрока
    std::list<Entity*> enemies; //список врагов
    std::list<Entity*> Bullets; //список пуль
    std::list<Entity*> Effects;
    std::list<Entity*> Stones;
    std::list<Entity*> bosses;
    std::list<int> Inventory = {0,1,2};
    std::list<Entity*>::iterator it;
    std::list<Entity*>::iterator it1;
    std::list<Entity*>::iterator it2;
    std::list<int>::iterator nextit;
    const int ENEMY_COUNT = 2; //максимальное количество врагов в игре
    const int BOSS_COUNT = 1;
    int enemiesCount = 2; //текущее количество врагов в игре
    int bossesCount = 1;
    float bossHP = 100;
    nextit = Inventory.begin();
    //Заполняем список объектами врагами

    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        float xr = 150 + rand() % 500; // случайная координата врага на поле игры по оси “x”
        float yr = 150 + rand() % 350; // случайная координата врага на поле игры по оси “y”
        //создаем врагов и помещаем в список
        enemies.push_back(new Enemy(easyEnemyImage, xr, yr, 96, 96, "EasyEnemy"));
        enemiesCount += 1; //увеличили счётчик врагов
    }
    for (int i = 0; i < BOSS_COUNT; i++)
    {
        float xr = 300 + rand() % 500; // случайная координата врага на поле игры по оси “x”
        float yr = 300 + rand() % 350; // случайная координата врага на поле игры по оси “y”
        //создаем врагов и помещаем в список
        bosses.push_back(new Boss(hardBossImage, xr, yr, 96, 96, "hardBoss"));
        bossesCount += 1; //увеличили счётчик врагов
    }
    int createObjectForMapTimer = 0;//Переменная под время для генерирования камней
    while (window.isOpen())
    {
        if ((Lvl==1)&&(track1<50))
           {
            ost1.play();
            track1++;
            }
        if ((Lvl==2)&&(track<50))
        {
        ost1.stop();
        ost.play();
        track++;
        }
        float time = clock.getElapsedTime().asMicroseconds();
        if (p.life) gameTime = gameTimeClock.getElapsedTime().asSeconds();//игровое время в
        //секундах идёт вперед, пока жив игрок. Перезагружать как time его не надо.
        //оно не обновляет логику игры
        clock.restart();
        time = time / 800;
        createObjectForMapTimer += time;//наращиваем таймер
        if (createObjectForMapTimer>3000){

            createObjectForMapTimer = 0;//обнуляем таймер
        }
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                if (Keyboard::isKeyPressed(Keyboard::I))
                    ++nextit;
            }
        }
        if(nextit != Inventory.begin() && nextit != Inventory.end()){
            if ((p.Mana-9)>0)//проверка хватит ли маны на выстрел
                if (Keyboard::isKeyPressed(Keyboard::P))
                {
                    sound.play();
                    Bullets.push_back(new Bullet(BulletImage, p.x, p.y, 16, 16, "Bullet", p.state));
                    p.Mana-=10;//тратим ману
                }
        }
        if(nextit == Inventory.end()){
            if (Keyboard::isKeyPressed(Keyboard::P))
            {
                if (p.StoneKD>0.999){
                    sound1.play();
                    Stones.push_back(new Stone(StoneImage, p.x, p.y, 16, 16, "Stone", p.state));
                    p.StoneKD-=1;
                }
            }
        }

        p.update(time); //оживляем объект “p” класса “Player”
        //оживляем врагов
        if (Lvl == 1){
            for (it = enemies.begin(); it != enemies.end(); it++)
            {
                (*it)->update(time); //запускаем метод update()
            }
        }
        //оживляем босс
        if (Lvl == 2){
            for (it = bosses.begin(); it != bosses.end(); it++)
            {
                (*it)->update(time); //запускаем метод update()
            }
        }

        //оживляем пули
        for (it = Bullets.begin(); it != Bullets.end(); it++)
        {
            (*it)->update(time); //запускаем метод update()
        }

        for (it = Effects.begin(); it != Effects.end(); it++)
        {
            (*it)->update(time); //запускаем метод update()
        }

        for (it = Stones.begin(); it != Stones.end(); it++)
        {
            (*it)->update(time);
        }
        //Проверяем список на наличие "мертвых" пуль и удаляем их
        for (it = Bullets.begin(); it != Bullets.end(); )//говорим что проходимся от начала до конца
        {// если этот объект мертв, то удаляем его
            if ((*it)-> life == false) { it = Bullets.erase(it); }
            else it++;//и идем курсором (итератором) к след объекту.
        }
        for (it = Stones.begin(); it != Stones.end(); )
        {
            if ((*it)-> life == false) { it = Stones.erase(it); }
            else it++;//и идем курсором (итератором) к след объекту.
        }
        //Проверка пересечения игрока с врагами
        //Если пересечение произошло, то "health = 75", игрок обездвижевается и

        if ((invinctime==0)||(invinctime<0))
            if (p.life == true){//если игрок жив
                for (it = enemies.begin(); it != enemies.end(); it++){//бежим по списку врагов
                    if ((p.getRect().intersects((*it)->getRect())) && ((*it)->name == "EasyEnemy"))
                    {
                        p.health -=25;//вычитаем 25 хп при уроне
                        invinctime=2.5;//даем неуязвимость на короткий промежуток времени
                        Effects.push_back(new Effect(hit, p.x, p.y, 16, 16, "hit", 500));
                    }
                }
            }

        //проверка пересечения с боссом
        if ((invinctime==0)||(invinctime<0))
            if ((p.life == true)&&(Lvl==2)){//если игрок жив
                for (it2 = bosses.begin(); it2 != bosses.end(); it2++){//бежим по списку врагов
                    if ((p.getRect().intersects((*it2)->getRect())) && ((*it2)->name == "hardBoss"))
                    {
                        p.health -=50;
                        invinctime=2.5;
                        Effects.push_back(new Effect(hit, p.x, p.y, 16, 16, "hit", 500));
                    }
                }
            }
        invinctime-=0.001;//время неуязвимости постоянно убывает
        if ((p.life == true)&&(Lvl==1)){ //если игрок жив
            //бежим по списку пуль
            for (it = Bullets.begin(); it != Bullets.end(); it++){
                for (it1 = enemies.begin(); it1 != enemies.end(); it1++){
                    if ((*it)->getRect().intersects((*it1)->getRect()))
                    {
                        it1 = enemies.erase(it1);
                        it = enemies.erase(it);
                    }
                }
            }


        }



        if (p.life == true){

            for (it = Stones.begin(); it != Stones.end(); it++){
                for (it1 = enemies.begin(); it1 != enemies.end(); it1++){
                    if ((*it)->getRect().intersects((*it1)->getRect()))
                    {
                        it1 = enemies.erase(it1);
                        it = enemies.erase(it);
                    }
                }
            }
        }

        if ((p.life == true)&&(Lvl==2)){ //если игрок жив
            //бежим по списку пуль
            for (it = Bullets.begin(); it != Bullets.end(); it++){
                for (it1 = bosses.begin(); it1 != bosses.end(); it1++){
                    if ((*it)->getRect().intersects((*it1)->getRect()))
                    {
                        bossHP -=0.1;
                        if (bossHP <=0){
                          it1 = bosses.erase(it1);
                          bossHP = 0;
                        }
                    }
                }
            }


        }
        if ((p.life == true)&&(Lvl==2)){ //если игрок жив
            //бежим по списку пуль
            for (it = Stones.begin(); it != Stones.end(); it++){
                for (it1 = bosses.begin(); it1 != bosses.end(); it1++){
                    if ((*it)->getRect().intersects((*it1)->getRect()))
                    {
                        bossHP -=0.5;
                        if (bossHP <=0){
                          it1 = bosses.erase(it1);
                          bossHP = 0;
                        }
                    }
                }
            }


        }

        window.clear();
        /////////////////////////////Рисуем карту/////////////////////
        if(Lvl == 1){
            for (int i = 0; i < HEIGHT_MAP; i++)
                for (int j = 0; j < WIDTH_MAP; j++)
                {
                    if (TileMap[i][j] == ' ') s_map.setTextureRect(IntRect(0, 0, 32, 32));
                    if (TileMap[i][j] == 'S') s_map.setTextureRect(IntRect(32, 0, 32, 32));
                    if (TileMap[i][j] == 's') s_map.setTextureRect(IntRect(64, 0, 32, 32));
                    if (TileMap[i][j] == 'G') s_map.setTextureRect(IntRect(96, 0, 32, 32));
                    if (TileMap[i][j] == 'g') s_map.setTextureRect(IntRect(128, 0, 32, 32));
                    if ((TileMap[i][j] == '0')) s_map.setTextureRect(IntRect(160, 0, 32, 32));
                    if ((TileMap[i][j] == 'C')) s_map.setTextureRect(IntRect(192, 0, 32, 32));
                    if ((TileMap[i][j] == 'c')) s_map.setTextureRect(IntRect(224, 0, 32, 32));
                    if ((TileMap[i][j] == 'M')) s_map.setTextureRect(IntRect(256, 0, 32, 32));
                    if ((TileMap[i][j] == 'm')) s_map.setTextureRect(IntRect(288, 0, 32, 32));
                    if ((TileMap[i][j] == 'N')) s_map.setTextureRect(IntRect(320, 0, 32, 32));
                    if ((TileMap[i][j] == 'n')) s_map.setTextureRect(IntRect(352, 0, 32, 32));
                    if ((TileMap[i][j] == 'U')) s_map.setTextureRect(IntRect(384, 0, 32, 32));
                    if ((TileMap[i][j] == 'u')) s_map.setTextureRect(IntRect(416, 0, 32, 32));
                    if ((TileMap[i][j] == 'h')) s_map.setTextureRect(IntRect(448, 0, 32, 32));
                    if ((TileMap[i][j] == 'i')) s_map.setTextureRect(IntRect(608, 0, 32, 32));
                    if ((TileMap[i][j] == 'd')) s_map.setTextureRect(IntRect(640, 0, 32, 32));
                    if ((TileMap[i][j] == 'p')) s_map.setTextureRect(IntRect(544, 0, 32, 32));
                    if ((TileMap[i][j] == 'e')) s_map.setTextureRect(IntRect(576, 0, 32, 32));
                    if ((TileMap[i][j] == 'b')) s_map.setTextureRect(IntRect(480, 0, 32, 32));
                    if ((TileMap[i][j] == 'j')) s_map.setTextureRect(IntRect(512, 0, 32, 32));
                    if ((TileMap[i][j] == 'L')) s_map.setTextureRect(IntRect(672, 0, 32, 32));
                    if ((TileMap[i][j] == 'l')) s_map.setTextureRect(IntRect(704, 0, 32, 32));
                    if ((TileMap[i][j] == 'V')) s_map.setTextureRect(IntRect(736, 0, 32, 32));
                    if ((TileMap[i][j] == 'v')) s_map.setTextureRect(IntRect(768, 0, 32, 32));

                    s_map.setPosition(j * 32, i * 32);
                    window.draw(s_map);
                }
        }
        if (Lvl == 2){
            for (int i = 0; i < HeightMap; i++)
                for (int j = 0; j < WidthwMap; j++)
                {

                    if (Map[i][j] == ' ') s_map.setTextureRect(IntRect(0, 0, 32, 32));
                    if (Map[i][j] == 'S') s_map.setTextureRect(IntRect(32, 0, 32, 32));
                    if (Map[i][j] == 's') s_map.setTextureRect(IntRect(64, 0, 32, 32));
                    if (Map[i][j] == 'G') s_map.setTextureRect(IntRect(96, 0, 32, 32));
                    if (Map[i][j] == 'g') s_map.setTextureRect(IntRect(128, 0, 32, 32));
                    if ((Map[i][j] == '0')) s_map.setTextureRect(IntRect(160, 0, 32, 32));
                    if ((Map[i][j] == 'C')) s_map.setTextureRect(IntRect(192, 0, 32, 32));
                    if ((Map[i][j] == 'c')) s_map.setTextureRect(IntRect(224, 0, 32, 32));
                    if ((Map[i][j] == 'M')) s_map.setTextureRect(IntRect(256, 0, 32, 32));
                    if ((Map[i][j] == 'm')) s_map.setTextureRect(IntRect(288, 0, 32, 32));
                    if ((Map[i][j] == 'N')) s_map.setTextureRect(IntRect(320, 0, 32, 32));
                    if ((Map[i][j] == 'n')) s_map.setTextureRect(IntRect(352, 0, 32, 32));
                    if ((Map[i][j] == 'U')) s_map.setTextureRect(IntRect(384, 0, 32, 32));
                    if ((Map[i][j] == 'u')) s_map.setTextureRect(IntRect(416, 0, 32, 32));
                    if ((Map[i][j] == 'h')) s_map.setTextureRect(IntRect(448, 0, 32, 32));
                    if ((Map[i][j] == 'L')) s_map.setTextureRect(IntRect(672, 0, 32, 32));
                    if ((Map[i][j] == 'l')) s_map.setTextureRect(IntRect(704, 0, 32, 32));
                    if ((Map[i][j] == 'V')) s_map.setTextureRect(IntRect(736, 0, 32, 32));
                    if ((Map[i][j] == 'v')) s_map.setTextureRect(IntRect(768, 0, 32, 32));

                    s_map.setPosition(j * 32, i * 32);
                    window.draw(s_map);
                }
        }

        if(nextit != Inventory.begin() && nextit != Inventory.end()){
            text.setString("Weapon: FireBall");
            text.setPosition(5, 770);//задаем позицию текста, отступая от центра камеры
            window.draw(text);//рисую этот текст
        }
        if(nextit == Inventory.begin()){

            text.setString("No Weapon,Press key I for change weapon");//задаем строку тексту
            text.setPosition(5, 770);//задаем позицию текста, отступая от центра камеры
            window.draw(text);//рисую этот текст

        }
        if(nextit == Inventory.end()){

            text.setString("Weapon: Stone");//задаем строку тексту
            text.setPosition(5, 770);//задаем позицию текста, отступая от центра камеры
            window.draw(text);//рисую этот текст

        }
        //объявили переменную здоровья и времени
        std::ostringstream playerManaString;
        playerManaString << (int)p.Mana; //формируем строку с целочисленной маной
        text.setString("Mana: " + playerManaString.str());//задаем строку тексту
        text.setPosition(170, 5);       //задаем позицию текста
        window.draw(text);              //рисуем этот текст

        std::ostringstream playerStoneString;
        playerStoneString << (int)p.StoneKD; //формируем строку с целочисленной маной
        text.setString("Stone: " + playerStoneString.str());//задаем строку тексту
        text.setPosition(275, 5);       //задаем позицию текста
        window.draw(text);              //рисуем этот текст

        std::ostringstream playerHealthString;
        playerHealthString << p.health; //формируем строку
        text.setString("Health: " + playerHealthString.str());//задаем строку тексту
        text.setPosition(50, 5);//задаем позицию текста
        window.draw(text);//рисуем этот текст

        if (Lvl == 2){
        std::ostringstream bossHealthString;
        bossHealthString << bossHP; //формируем строку
        text.setString("Health BOSS: " + bossHealthString.str());//задаем строку тексту
        text.setPosition(50, 30);//задаем позицию текста
        window.draw(text);//рисуем этот текст
        }

        window.draw(p.sprite);//рисуем спрайт объекта “p” класса “Player”
        if(gameTime < 5){
            text.setString("What am I doing here, I need to go up to this old man to ask");//задаем строку тексту
            text.setPosition(70, 70);//задаем позицию текста, отступая от центра камеры
            window.draw(text);//рисую этот текст
        }
        //рисуем врагов
        if (Lvl == 1){
            for (it = enemies.begin(); it != enemies.end(); it++)
            {
                if ((*it)->life) //если враги живы
                    window.draw((*it)->sprite); //рисуем
            }
        }

        //рисуем Босса
        if (Lvl == 2){

            for (it1 = enemies.begin(); it1 != enemies.end(); it1++){
                {
                    it1 = enemies.erase(it1);
                }
            }
            for (it = bosses.begin(); it != bosses.end(); it++)
            {
                if ((*it)->life) //если босс жив
                    window.draw((*it)->sprite);
            }
        }

        //        рисуем пули
        for (it = Bullets.begin(); it != Bullets.end(); it++)
        {
            if ((*it)->life) //если пули живы
                window.draw((*it)->sprite); //рисуем объекты
        }

        for (it = Effects.begin(); it != Effects.end(); it++)
        {
            if ((*it)->life) //если пули живы
                window.draw((*it)->sprite); //рисуем объекты
        }
        for (it = Stones.begin(); it != Stones.end(); it++)
        {
            if ((*it)->life) //если пули живы
                window.draw((*it)->sprite); //рисуем объекты
        }
        if ((p.Mana<10))//восстановление маны
            p.Mana+=0.004;

        if ((p.StoneKD<1))//поиск камня
            p.StoneKD+=0.001;

        if ((abs(p.x - 600) < 80) && (abs(p.y - 100) < 80)){
            if (isdial){
            NPCDIAL=1;
            if(dialtime < 55) //чтобы dialtime не прибавлялся бесконечно + isdial проверяет начат ли диалог
            dialtime +=0.01;  //работаем с DialTime
            if (dialtime > 9 && dialtime < 11 && timebool) //так много if потому что dialtime - float и делает за одно число много действий
            {
                    dialtimeest -=1;
                    timebool=0;
            }
            if (dialtime > 11 && dialtime < 13)
            {
                    timebool=1;
            }
            if (dialtime > 19 && dialtime < 21 && timebool)
            {
                    dialtimeest -=1;
                    timebool=0;
            }
            if (dialtime > 21 && dialtime < 23)
            {
                    timebool=1;
            }
            if (dialtime > 29 && dialtime < 31 && timebool)
            {
                    dialtimeest -=1;
                    timebool=0;
            }
            if (dialtime > 31 && dialtime < 33)
            {
                    timebool=1;
            }
            if (dialtime > 39 && dialtime < 41 && timebool)
            {
                    dialtimeest -=1;
                    timebool=0;
            }
            if (dialtime > 41 && dialtime < 43)
            {
                    timebool=1;
            }
            if (dialtime > 49 && dialtime < 51 && timebool)
            {
                    dialtimeest -=1;
                    timebool=0;
            }
            if (dialtime > 51 && dialtime < 53)
            {
                    timebool=1;
            }
            Event nextdial;
            while (window.pollEvent(nextdial))
            {
                if (nextdial.type == Event::KeyReleased && dialtime>50){
                    if (nextdial.key.code == Keyboard::Space){
                        dialnum+=1;
                        dialtime = 0;
                        dialtimeest = 5;
                    }
                }
            }
        }
            else //event для старта диалога
            {
                Event startdial;
                while (window.pollEvent(startdial))
                {
                    if (startdial.type == Event::KeyReleased){
                        if (startdial.key.code == Keyboard::Space){
                            isdial = 1;
                        }
                    }
                }
                text.setString("Press Space to talk");//задаем строку тексту
                text.setPosition(70, 650);//задаем позицию текста, отступая от центра камеры
                window.draw(text);//рисую этот текст
            }
        }
        else
            NPCDIAL=0;
        if (NPCDIAL==1)
        {
            std::ostringstream dialtimeestStr;
            dialtimeestStr << dialtimeest; //формируем строку


            if (dialnum==0)
            {
                text.setString("Greetings,do you remember ho did you get here? "+ dialtimeestStr.str());//задаем строку тексту
                text.setPosition(70, 650);//задаем позицию текста, отступая от центра камеры
                window.draw(text);//рисую этот текст
            }
            if (dialnum==1)
            {
                text.setString("Nevermind, at the end of this dungeon, all the answers await "+ dialtimeestStr.str());//задаем строку тексту
                text.setPosition(70, 650);//задаем позицию текста, отступая от центра камеры
                window.draw(text);//рисую этот текст
            }
            if (dialnum==2)
            {
                text.setString("But right now, could you take the heart stone from that chest of mine? ");
                text.setPosition(70, 650);//задаем позицию текста, отступая от центра камеры
                window.draw(text);//рисую этот текст
                text.setString("This creature is blocking a way out, you can take everything else"+ dialtimeestStr.str());
                text.setPosition(70, 700);//задаем позицию текста, отступая от центра камеры
                window.draw(text);//рисую этот текст
                quest1=2;
            }
            if (dialnum==3)
            {
                text.setString("Don't forget that you have an ability, use it by pressing the P button "+ dialtimeestStr.str());//задаем строку тексту
                text.setPosition(70, 650);//задаем позицию текста, отступая от центра камеры
                window.draw(text);//рисую этот текст

            }
            if ((dialnum==4)&&(quest1==3))
            {

                text.setString("Thank you, I think u will need it more that i will, good luck "+ dialtimeestStr.str());
                text.setPosition(70, 650);//задаем позицию текста, отступая от центра камеры
                window.draw(text);//рисую этот текст

            }
            if (dialnum==5){
                quest1=4;
                quest2=1;}
            if (Keyboard::isKeyPressed(Keyboard::J)) dialnum = 0;

        }
        if ((quest1==1)&&(gameTime > 5))
        {
            text.setString("A New Begining");//задаем строку тексту
            text.setPosition(70,40);//задаем позицию текста, отступая от центра камеры
            window.draw(text);//рисую этот текст
            text.setString("Go Talk with a strange old man");//задаем строку тексту
            text.setPosition(70,70);//задаем позицию текста, отступая от центра камеры
            window.draw(text);//рисую этот текст
        }
        if (quest1==2)
        {
            text.setString("A New Begining");//задаем строку тексту
            text.setPosition(70,40);//задаем позицию текста, отступая от центра камеры
            window.draw(text);//рисую этот текст
            text.setString("Take old mans heart stone from the chest");//задаем строку тексту
            text.setPosition(70,70);//задаем позицию текста, отступая от центра камеры
            window.draw(text);//рисую этот текст
        }
        if ((quest1==3)&&(quest1!=4))
        {
            text.setString("A New Begining");//задаем строку тексту
            text.setPosition(70,40);//задаем позицию текста, отступая от центра камеры
            window.draw(text);//рисую этот текст
            text.setString("Give the heart stone back");//задаем строку тексту
            text.setPosition(70,70);//задаем позицию текста, отступая от центра камеры
            window.draw(text);//рисую этот текст
        }
        if (quest2==1){
            text.setString("Big trouble in a small dungeon");//задаем строку тексту
            text.setPosition(70,60);//задаем позицию текста, отступая от центра камеры
            window.draw(text);//рисую этот текст
            text.setString("Defeat the monster");//задаем строку тексту
            text.setPosition(70,80);//задаем позицию текста, отступая от центра камеры
            window.draw(text);//рисую этот текст
        }
        if (bossHP<=0){
            quest2=2;
            text.setString("Big trouble in a small dungeon");//задаем строку тексту
            text.setPosition(250,300);//задаем позицию текста, отступая от центра камеры
            window.draw(text);//рисую этот текст
            text.setString("Quest complete! Thanks 4 playing!");//задаем строку тексту
            text.setPosition(250,350);//задаем позицию текста, отступая от центра камеры
            window.draw(text);//рисую этот текст
        }
        window.display();

    }
    return 0;
}
