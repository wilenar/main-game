#include <SFML/Graphics.hpp>
#include <iostream>
#include "map.h"
#include "view.h"
#include <sstream>

using namespace sf;

class Player {
private: float x, y = 0;
public:
	float w, h, dx, dy, speed = 0;
	int dir, playerScore; //храним здесь очки игрока
	String File;
	Image image;
	Texture texture;
	Sprite sprite;

	Player(String F, float X, float Y, float W, float H) { //Конструктор с параметрами(формальными) для класса Player. При создании объекта класса мы будем задавать имя файла, координату Х и У, ширину и высоту
		dir = 0; playerScore = 0;
		File = F;
		w = W; h = H;//высота и ширина
		image.loadFromFile("images/" + File);//запихиваем в image наше изображение вместо File мы передадим то, что пропишем при создании объекта.
		image.createMaskFromColor(Color(255, 255, 255));//убираем ненужный белый цвет.
		texture.loadFromImage(image);//закидываем наше изображение в текстуру
		sprite.setTexture(texture);//заливаем спрайт текстурой
		x = X; y = Y;//координата появления спрайта
		sprite.setTextureRect(IntRect(0, 0, w, h)); //Задаем спрайту один прямоугольник для вывода одного корабля. IntRect - приведение типов
	}


	void update(float time) //функция "оживления" объекта класса. update - обновление. принимает в себя время SFML , вследствие чего работает бесконечно, давая персонажу движение.
	{
		switch (dir)//реализуем поведение в зависимости от направления. (каждая цифра соответствует направлению)
		{
		case 0: dx = speed; dy = 0; break;//по иксу задаем положительную скорость, по игреку зануляем. получаем, что персонаж идет только вправо
		case 1: dx = -speed; dy = 0; break;//по иксу задаем отрицательную скорость, по игреку зануляем. получается, что персонаж идет только влево
		case 2: dx = 0; dy = speed; break;//по иксу задаем нулевое значение, по игреку положительное. получается, что персонаж идет только вниз
		case 3: dx = 0; dy = -speed; break;//по иксу задаем нулевое значение, по игреку отрицательное. получается, что персонаж идет только вверх
		
		}


		x += dx*time;//Наше ускорение на время получаем смещение координат и как следствие движение
		y += dy*time;//аналогично по игреку

		speed = 0;//зануляем скорость, чтобы персонаж остановился.
		sprite.setPosition(x, y); //выводим спрайт в позицию x y , посередине. бесконечно выводим в этой функции, иначе бы наш спрайт стоял на месте.
		interactionWithMap();//вызываем функцию, отвечающую за взаимодействие с картой
	}

	float getplayercoordinateX() {
		return x;
	}
	float getplayercoordinateY() {
		return y;
	}



		void interactionWithMap()//ф-ция взаимодействия с картой
		{

			for (int i = y / 32; i < (y + h) / 32; i++)//проходимся по тайликам, контактирующим с игроком, то есть по всем квадратикам размера 32*32, которые мы окрашивали в 9 уроке. про условия читайте ниже.
			for (int j = x / 32; j<(x + w) / 32; j++)//икс делим на 32, тем самым получаем левый квадратик, с которым персонаж соприкасается. (он ведь больше размера 32*32, поэтому может одновременно стоять на нескольких квадратах). А j<(x + w) / 32 - условие ограничения координат по иксу. то есть координата самого правого квадрата, который соприкасается с персонажем. таким образом идем в цикле слева направо по иксу, проходя по от левого квадрата (соприкасающегося с героем), до правого квадрата (соприкасающегося с героем)
				{
					if (TileMap[i][j] == '0')//если наш квадратик соответствует символу 0 (стена), то проверяем "направление скорости" персонажа:
					{
						if (dy>0)//если мы шли вниз,
						{
							y = i * 32 - h;//то стопорим координату игрек персонажа. сначала получаем координату нашего квадратика на карте(стены) и затем вычитаем из высоты спрайта персонажа.
						}
						if (dy<0)
						{
							y = i * 32 + 32;//аналогично с ходьбой вверх. dy<0, значит мы идем вверх (вспоминаем координаты паинта)
						}
						if (dx>0)
						{
							x = j * 32 - w;//если идем вправо, то координата Х равна стена (символ 0) минус ширина персонажа
						}
						if (dx < 0)
						{
							x = j * 32 + 32;//аналогично идем влево
						}
					}

					if (TileMap[i][j] == 's') { //если символ равен 's' (камень)
						playerScore++;
						TileMap[i][j] = ' ';//убираем камень, типа взяли бонус. можем и не убирать, кстати.
					}
				}
		}
	
};

int main()
{	
	RenderWindow window(sf::VideoMode(1024, 640), "Main Game");
	view.reset(sf::FloatRect(0, 0, 1024, 640));//размер "вида" камеры при создании объекта вида камеры. (потом можем менять как хотим) Что то типа инициализации.

	Font font;
	font.loadFromFile("CyrilicOld.ttf");
	Text text("", font, 20);
	text.setColor(Color::Red);
	text.setStyle(Text::Bold);

	Image map_image;//объект изображения для карты
	map_image.loadFromFile("images/map.png");//загружаем файл для карты
	Texture map;//текстура карты
	map.loadFromImage(map_image);//заряжаем текстуру картинкой
	Sprite s_map;//создаём спрайт для карты
	s_map.setTexture(map);//заливаем текстуру спрайтом

	Player p("ships.png", 239, 255, 80.0, 64.0);//создаем объект p класса player,задаем ships.png как имя файла+расширение, далее координата Х,У, ширина, высота.

	float CurrentFrame = 0;
	Clock clock;

	while (window.isOpen())
	{	
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 800;
		
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		if (Keyboard::isKeyPressed(Keyboard::Left)) {
			p.dir = 1; p.speed = 0.1;//dir =1 - направление вверх, speed =0.1 - скорость движения.
			CurrentFrame += 0.005*time;
			if (CurrentFrame > 3) CurrentFrame -= 3;
			p.sprite.setTextureRect(IntRect(80 * int(CurrentFrame), 64, 80, 64)); //через объект p класса player меняем спрайт, делая анимацию (используя оператор точку)
			getplayercoordinateforview(p.getplayercoordinateX(), p.getplayercoordinateY());
		}

		if (Keyboard::isKeyPressed(Keyboard::Right)) {
			p.dir = 0; p.speed = 0.1;//направление вправо, см выше
			CurrentFrame += 0.005*time;
			if (CurrentFrame > 3) CurrentFrame -= 3;
			p.sprite.setTextureRect(IntRect(80 * int(CurrentFrame), 128, 80, 64)); //через объект p класса player меняем спрайт
			getplayercoordinateforview(p.getplayercoordinateX(), p.getplayercoordinateY());
		}

		if (Keyboard::isKeyPressed(Keyboard::Up)) {
			p.dir = 3; p.speed = 0.1;//направление вниз, см выше
			CurrentFrame += 0.005*time;
			if (CurrentFrame > 3) CurrentFrame -= 3;
			p.sprite.setTextureRect(IntRect(80 * int(CurrentFrame), 192, 80, 64)); //через объект p класса player меняем спрайт
			getplayercoordinateforview(p.getplayercoordinateX(), p.getplayercoordinateY());

		}

		if (Keyboard::isKeyPressed(Keyboard::Down)) { 
			p.dir = 2; p.speed = 0.1;//направление вверх, см выше
			CurrentFrame += 0.005*time; //служит для прохождения по "кадрам". переменная доходит до трех суммируя произведение времени и скорости. изменив 0.005 можно изменить скорость анимации
			if (CurrentFrame > 3) CurrentFrame -= 3; //проходимся по кадрам с первого по третий включительно. если пришли к третьему кадру - откидываемся назад.
			p.sprite.setTextureRect(IntRect(80 * int(CurrentFrame), 0, 80, 64)); //проходимся по координатам Х.
			getplayercoordinateforview(p.getplayercoordinateX(), p.getplayercoordinateY());

		}
		getplayercoordinateforview(p.getplayercoordinateX(), p.getplayercoordinateY());
		p.update(time);

		window.setView(view);//"оживляем" камеру в окне sfml
		window.clear();
		
		for (int i = 0; i < HEIGHT_MAP; i++)
		for (int j = 0; j < WIDTH_MAP; j++)
			{
				if (TileMap[i][j] == ' ')  s_map.setTextureRect(IntRect(0, 0, 32, 32)); //если встретили символ пробел, то рисуем 1й квадратик
				if (TileMap[i][j] == 's')  s_map.setTextureRect(IntRect(32, 0, 32, 32));//если встретили символ s, то рисуем 2й квадратик
				if ((TileMap[i][j] == '0')) s_map.setTextureRect(IntRect(64, 0, 32, 32));//если встретили символ 0, то рисуем 3й квадратик


				s_map.setPosition(j * 32, i * 32);//по сути раскидывает квадратики, превращая в карту. то есть задает каждому из них позицию. если убрать, то вся карта нарисуется в одном квадрате 32*32 и мы увидим один квадрат
	
				window.draw(s_map);//рисуем квадратики на экран
			}
	std::ostringstream playerScoreString;
		playerScoreString << p.playerScore;
		text.setString("Собрано астероидов:" + playerScoreString.str());
		text.setPosition(view.getCenter().x - 510,view.getCenter().y - 323);
		window.draw(text);
		window.draw(p.sprite);//рисуем спрайт объекта p класса player
		window.display();
	}

	return 0;
}