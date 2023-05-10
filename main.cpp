#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include "map.h"
#include <cmath>
#include <list>


using namespace sf;
int NPCDIAL=0;
class Entity {
public:
    enum { left, right, up, down, stay, upleft, upright, downleft, downright} state;// тип перечисления - состояние объекта
    float dx, dy, x, y, speed, moveTimer;//добавили переменную таймер для будущих целей
    int w, h, health; //переменная “health”, хранящая жизни игрока
    bool life; //переменная “life” жизнь, логическая
    Texture texture;//сфмл текстура
    Sprite sprite;//сфмл спрайт
    float CurrentFrame;//хранит текущий кадр
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

        if ((Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::W))&&(Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A))) {
            state = downleft;
            speed = 0.1;
        }
        if ((Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W))&&(Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::A))) {
            state = upright;
            speed = 0.1;
        }
        if ((Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::W))&&(Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::A))) {
            state = downright;
            speed = 0.1;
        }

    }
    //Метод проверки столкновений с элементами карты
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
                if (TileMap[i][j] == 's') {
                    NPCDIAL=1;
                }
                else NPCDIAL=0;
                if (TileMap[i][j] == 'f') {

                    TileMap[i][j] = 'h';//убрали цветок
                }
                if (TileMap[i][j] == 'h') {

                }
            }
    }
    void update(float time) //метод "оживления/обновления" объекта класса.
    {
        if (life) {//проверяем, жив ли герой
            control();//функция управления персонажем
            switch (state)//делаются различные действия в зависимости от состояния
            {
            dx = 0;
            dy = 0;
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
            checkCollisionWithMap(dx, 0);//обрабатываем столкновение по Х
            y += dy*time; //движение по “Y”
            checkCollisionWithMap(0, dy);//обрабатываем столкновение по Y
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
                checkCollisionWithMap(dx, 0);//обрабатываем столкновение по Хy += dy*time; //движение по “Y”
                checkCollisionWithMap(0, dy);//обрабатываем столкновение по Y
                sprite.setPosition(x, y); //спрайт в позиции (x, y).
                if (health <= 0){ life = false; }//если жизней меньше 0, либо равно 0, то умираем
            }
        }
    }
};//класс Enemy закрыт
////////////////////////////КЛАСС ПУЛИ////////////////////////
//            class Bullet :public Entity{//класс пули
//            public:
//            int direction;//направление пули
//            //всё так же, только взяли в конце состояние игрока (int dir)
//            //для задания направления полёта пули
//            Bullet(Image &image, float X, float Y, int W, int H, std::string Name, int dir)
//            :Entity(image, X, Y, W, H, Name){
//            x = X;
//            y = Y;
//            direction = dir;
//            speed = 0.8;
//            w = h = 16;
//            life = true;
//            //выше инициализация в конструкторе
//            }
//            void update(float time)
//            {
//            switch (direction)
//            {
//            case 0: dx = -speed; dy = 0; break;// state = left
//            case 1: dx = speed; dy = 0; break;// state = right
//            case 2: dx = 0; dy = -speed; break;// state = up
//            case 3: dx = 0; dy = speed; break;// state = down
//            }
//            if (life){
//            x += dx*time;//само движение пули по х
//            y += dy*time;//по у
//            if (x <= 0) x = 20;// задержка пули в левой стене, чтобы при проседании кадров она случайно не вылетела за предел карты и не было ошибки (сервер может тормозить!)
//            if (y <= 0) y = 20;
//            if (x >= 800) x = 780;// задержка пули в правой стене, чтобы при проседании
//           // кадров она случайно не вылетела за предел карты и не было ошибки (сервер может тормозить!)
//            if (y >= 640) y = 620;
//            for (int i = y / 32; i < (y + h) / 32; i++)//проходимся по элементам карты
//            for (int j = x / 32; j < (x + w) / 32; j++)
//            {
//            if (TileMap[i][j] == '0')//если элемент наш тайлик земли, то
//            life = false;// то пуля умирает
//            }
//            sprite.setPosition(x + w / 2, y + h / 2);//задается позицию пули
//            }
//            }
//            };

int main()
{
    float dialnum=0;
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(sf::VideoMode(800, 800, desktop.bitsPerPixel), "The Witcher 4");

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
    //Image BulletImage;//изображение для пули
    //BulletImage.loadFromFile("images/bullet.png");//загрузили картинку в объект изображения
    Player p(heroImage, 100, 100, 96, 96, "Player1");//объект класса игрока
    std::list<Entity*> enemies; //список врагов
    //std::list<Entity*> Bullets; //список пуль
    std::list<Entity*>::iterator it; //итератор чтобы проходить по элементам списка
    const int ENEMY_COUNT = 0; //максимальное количество врагов в игре
    int enemiesCount = 0; //текущее количество врагов в игре
    //Заполняем список объектами врагами
    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        float xr = 150 + rand() % 500; // случайная координата врага на поле игры по оси “x”
        float yr = 150 + rand() % 350; // случайная координата врага на поле игры по оси “y”
        //создаем врагов и помещаем в список
        enemies.push_back(new Enemy(easyEnemyImage, xr, yr, 96, 96, "EasyEnemy"));
        enemiesCount += 1; //увеличили счётчик врагов
    }
    int createObjectForMapTimer = 0;//Переменная под время для генерирования камней
    while (window.isOpen())
    {
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
//            //стреляем по нажатию клавиши "P"
//            if (event.type == sf::Event::KeyPressed)
//            {
//                if (event.key.code == sf::Keyboard::P)
//                {
//                    Bullets.push_back(new Bullet(BulletImage, p.x, p.y, 16, 16, "Bullet", p.state));
//                }
//            }
        }
        p.update(time); //оживляем объект “p” класса “Player”
        //оживляем врагов
        for (it = enemies.begin(); it != enemies.end(); it++)
        {
            (*it)->update(time); //запускаем метод update()
        }
        //оживляем пули
//        for (it = Bullets.begin(); it != Bullets.end(); it++)
//        {
//            (*it)->update(time); //запускаем метод update()
//        }
        //Проверяем список на наличие "мертвых" пуль и удаляем их
//        for (it = Bullets.begin(); it != Bullets.end(); )//говорим что проходимся от начала до конца
//        {// если этот объект мертв, то удаляем его
//            if ((*it)-> life == false) { it = Bullets.erase(it); }
//            else it++;//и идем курсором (итератором) к след объекту.
//        }
        //Проверка пересечения игрока с врагами
        //Если пересечение произошло, то "health = 0", игрок обездвижевается и
        //выводится сообщение "you are lose"
        if (p.life == true){//если игрок жив
            for (it = enemies.begin(); it != enemies.end(); it++){//бежим по списку врагов
                if ((p.getRect().intersects((*it)->getRect())) && ((*it)->name == "EasyEnemy"))
                {
                    p.health = 0;
                    std::cout << "you are lose";
                }
            }
        }
        window.clear();
        /////////////////////////////Рисуем карту/////////////////////
        for (int i = 0; i < HEIGHT_MAP; i++)
            for (int j = 0; j < WIDTH_MAP; j++)
            {
                if (TileMap[i][j] == ' ') s_map.setTextureRect(IntRect(0, 0, 32, 32));
                if (TileMap[i][j] == 's') s_map.setTextureRect(IntRect(32, 0, 32, 32));
                if ((TileMap[i][j] == '0')) s_map.setTextureRect(IntRect(64, 0, 32, 32));
                if ((TileMap[i][j] == 'f')) s_map.setTextureRect(IntRect(96, 0, 32, 32));//сундук
                if ((TileMap[i][j] == 'h')) s_map.setTextureRect(IntRect(128, 0, 32, 32));//открытый сундук
                s_map.setPosition(j * 32, i * 32);
                window.draw(s_map);
            }
        //объявили переменную здоровья и времени
        std::ostringstream playerHealthString, gameTimeString;
        playerHealthString << p.health; gameTimeString << gameTime;//формируем строку

        window.draw(p.sprite);//рисуем спрайт объекта “p” класса “Player”
        //рисуем врагов
        for (it = enemies.begin(); it != enemies.end(); it++)
        {
            if ((*it)->life) //если враги живы
                window.draw((*it)->sprite); //рисуем
        }
        //рисуем пули
//        for (it = Bullets.begin(); it != Bullets.end(); it++)
//        {
//            if ((*it)->life) //если пули живы
//                window.draw((*it)->sprite); //рисуем объекты
//        }
if (NPCDIAL==1)
{
if (dialnum==0)
{
        text.setString("Greetings,do you remember ho did you get here?");//задаем строку тексту
        text.setPosition(50, 50);//задаем позицию текста, отступая от центра камеры
        window.draw(text);//рисую этот текст
}
if ((dialnum>0)&&(dialnum<2))
{
    text.setString("Nevermind, at the end of this dungeon, all the answers await");//задаем строку тексту
    text.setPosition(50, 50);//задаем позицию текста, отступая от центра камеры
    window.draw(text);//рисую этот текст
}
if (dialnum>2)
{
    text.setString("Take my belongings from that chest, it will help you in tour journey");//задаем строку тексту
    text.setPosition(50, 50);//задаем позицию текста, отступая от центра камеры
    window.draw(text);//рисую этот текст
}
if (Keyboard::isKeyPressed(Keyboard::Space))
    dialnum=dialnum+0.01;
}
        window.display();
    }
    return 0;
}
