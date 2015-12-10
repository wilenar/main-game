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
	int dir, playerScore; //������ ����� ���� ������
	String File;
	Image image;
	Texture texture;
	Sprite sprite;

	Player(String F, float X, float Y, float W, float H) { //����������� � �����������(�����������) ��� ������ Player. ��� �������� ������� ������ �� ����� �������� ��� �����, ���������� � � �, ������ � ������
		dir = 0; playerScore = 0;
		File = F;
		w = W; h = H;//������ � ������
		image.loadFromFile("images/" + File);//���������� � image ���� ����������� ������ File �� ��������� ��, ��� �������� ��� �������� �������.
		image.createMaskFromColor(Color(255, 255, 255));//������� �������� ����� ����.
		texture.loadFromImage(image);//���������� ���� ����������� � ��������
		sprite.setTexture(texture);//�������� ������ ���������
		x = X; y = Y;//���������� ��������� �������
		sprite.setTextureRect(IntRect(0, 0, w, h)); //������ ������� ���� ������������� ��� ������ ������ �������. IntRect - ���������� �����
	}


	void update(float time) //������� "���������" ������� ������. update - ����������. ��������� � ���� ����� SFML , ���������� ���� �������� ����������, ����� ��������� ��������.
	{
		switch (dir)//��������� ��������� � ����������� �� �����������. (������ ����� ������������� �����������)
		{
		case 0: dx = speed; dy = 0; break;//�� ���� ������ ������������� ��������, �� ������ ��������. ��������, ��� �������� ���� ������ ������
		case 1: dx = -speed; dy = 0; break;//�� ���� ������ ������������� ��������, �� ������ ��������. ����������, ��� �������� ���� ������ �����
		case 2: dx = 0; dy = speed; break;//�� ���� ������ ������� ��������, �� ������ �������������. ����������, ��� �������� ���� ������ ����
		case 3: dx = 0; dy = -speed; break;//�� ���� ������ ������� ��������, �� ������ �������������. ����������, ��� �������� ���� ������ �����
		
		}


		x += dx*time;//���� ��������� �� ����� �������� �������� ��������� � ��� ��������� ��������
		y += dy*time;//���������� �� ������

		speed = 0;//�������� ��������, ����� �������� �����������.
		sprite.setPosition(x, y); //������� ������ � ������� x y , ����������. ���������� ������� � ���� �������, ����� �� ��� ������ ����� �� �����.
		interactionWithMap();//�������� �������, ���������� �� �������������� � ������
	}

	float getplayercoordinateX() {
		return x;
	}
	float getplayercoordinateY() {
		return y;
	}



		void interactionWithMap()//�-��� �������������� � ������
		{

			for (int i = y / 32; i < (y + h) / 32; i++)//���������� �� ��������, �������������� � �������, �� ���� �� ���� ����������� ������� 32*32, ������� �� ���������� � 9 �����. ��� ������� ������� ����.
			for (int j = x / 32; j<(x + w) / 32; j++)//��� ����� �� 32, ��� ����� �������� ����� ���������, � ������� �������� �������������. (�� ���� ������ ������� 32*32, ������� ����� ������������ ������ �� ���������� ���������). � j<(x + w) / 32 - ������� ����������� ��������� �� ����. �� ���� ���������� ������ ������� ��������, ������� ������������� � ����������. ����� ������� ���� � ����� ����� ������� �� ����, ������� �� �� ������ �������� (���������������� � ������), �� ������� �������� (���������������� � ������)
				{
					if (TileMap[i][j] == '0')//���� ��� ��������� ������������� ������� 0 (�����), �� ��������� "����������� ��������" ���������:
					{
						if (dy>0)//���� �� ��� ����,
						{
							y = i * 32 - h;//�� �������� ���������� ����� ���������. ������� �������� ���������� ������ ���������� �� �����(�����) � ����� �������� �� ������ ������� ���������.
						}
						if (dy<0)
						{
							y = i * 32 + 32;//���������� � ������� �����. dy<0, ������ �� ���� ����� (���������� ���������� ������)
						}
						if (dx>0)
						{
							x = j * 32 - w;//���� ���� ������, �� ���������� � ����� ����� (������ 0) ����� ������ ���������
						}
						if (dx < 0)
						{
							x = j * 32 + 32;//���������� ���� �����
						}
					}

					if (TileMap[i][j] == 's') { //���� ������ ����� 's' (������)
						playerScore++;
						TileMap[i][j] = ' ';//������� ������, ���� ����� �����. ����� � �� �������, ������.
					}
				}
		}
	
};

int main()
{	
	RenderWindow window(sf::VideoMode(1024, 640), "Main Game");
	view.reset(sf::FloatRect(0, 0, 1024, 640));//������ "����" ������ ��� �������� ������� ���� ������. (����� ����� ������ ��� �����) ��� �� ���� �������������.

	Font font;
	font.loadFromFile("CyrilicOld.ttf");
	Text text("", font, 20);
	text.setColor(Color::Red);
	text.setStyle(Text::Bold);

	Image map_image;//������ ����������� ��� �����
	map_image.loadFromFile("images/map.png");//��������� ���� ��� �����
	Texture map;//�������� �����
	map.loadFromImage(map_image);//�������� �������� ���������
	Sprite s_map;//������ ������ ��� �����
	s_map.setTexture(map);//�������� �������� ��������

	Player p("ships.png", 239, 255, 80.0, 64.0);//������� ������ p ������ player,������ ships.png ��� ��� �����+����������, ����� ���������� �,�, ������, ������.

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
			p.dir = 1; p.speed = 0.1;//dir =1 - ����������� �����, speed =0.1 - �������� ��������.
			CurrentFrame += 0.005*time;
			if (CurrentFrame > 3) CurrentFrame -= 3;
			p.sprite.setTextureRect(IntRect(80 * int(CurrentFrame), 64, 80, 64)); //����� ������ p ������ player ������ ������, ����� �������� (��������� �������� �����)
			getplayercoordinateforview(p.getplayercoordinateX(), p.getplayercoordinateY());
		}

		if (Keyboard::isKeyPressed(Keyboard::Right)) {
			p.dir = 0; p.speed = 0.1;//����������� ������, �� ����
			CurrentFrame += 0.005*time;
			if (CurrentFrame > 3) CurrentFrame -= 3;
			p.sprite.setTextureRect(IntRect(80 * int(CurrentFrame), 128, 80, 64)); //����� ������ p ������ player ������ ������
			getplayercoordinateforview(p.getplayercoordinateX(), p.getplayercoordinateY());
		}

		if (Keyboard::isKeyPressed(Keyboard::Up)) {
			p.dir = 3; p.speed = 0.1;//����������� ����, �� ����
			CurrentFrame += 0.005*time;
			if (CurrentFrame > 3) CurrentFrame -= 3;
			p.sprite.setTextureRect(IntRect(80 * int(CurrentFrame), 192, 80, 64)); //����� ������ p ������ player ������ ������
			getplayercoordinateforview(p.getplayercoordinateX(), p.getplayercoordinateY());

		}

		if (Keyboard::isKeyPressed(Keyboard::Down)) { 
			p.dir = 2; p.speed = 0.1;//����������� �����, �� ����
			CurrentFrame += 0.005*time; //������ ��� ����������� �� "������". ���������� ������� �� ���� �������� ������������ ������� � ��������. ������� 0.005 ����� �������� �������� ��������
			if (CurrentFrame > 3) CurrentFrame -= 3; //���������� �� ������ � ������� �� ������ ������������. ���� ������ � �������� ����� - ������������ �����.
			p.sprite.setTextureRect(IntRect(80 * int(CurrentFrame), 0, 80, 64)); //���������� �� ����������� �.
			getplayercoordinateforview(p.getplayercoordinateX(), p.getplayercoordinateY());

		}
		getplayercoordinateforview(p.getplayercoordinateX(), p.getplayercoordinateY());
		p.update(time);

		window.setView(view);//"��������" ������ � ���� sfml
		window.clear();
		
		for (int i = 0; i < HEIGHT_MAP; i++)
		for (int j = 0; j < WIDTH_MAP; j++)
			{
				if (TileMap[i][j] == ' ')  s_map.setTextureRect(IntRect(0, 0, 32, 32)); //���� ��������� ������ ������, �� ������ 1� ���������
				if (TileMap[i][j] == 's')  s_map.setTextureRect(IntRect(32, 0, 32, 32));//���� ��������� ������ s, �� ������ 2� ���������
				if ((TileMap[i][j] == '0')) s_map.setTextureRect(IntRect(64, 0, 32, 32));//���� ��������� ������ 0, �� ������ 3� ���������


				s_map.setPosition(j * 32, i * 32);//�� ���� ����������� ����������, ��������� � �����. �� ���� ������ ������� �� ��� �������. ���� ������, �� ��� ����� ���������� � ����� �������� 32*32 � �� ������ ���� �������
	
				window.draw(s_map);//������ ���������� �� �����
			}
	std::ostringstream playerScoreString;
		playerScoreString << p.playerScore;
		text.setString("������� ����������:" + playerScoreString.str());
		text.setPosition(view.getCenter().x - 510,view.getCenter().y - 323);
		window.draw(text);
		window.draw(p.sprite);//������ ������ ������� p ������ player
		window.display();
	}

	return 0;
}