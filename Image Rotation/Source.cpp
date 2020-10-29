#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
using namespace std;
#define PI 3.14159

// Цифры от 1 до 4 переключают функции манипуляций с изображением
// Вращение Хрен пойми как 1
void Rotate1(float angle, sf::Vertex** v, const sf::Vector2u& img_size, const sf::Vector2f& origin) 
{
	const double a = angle / 180. * PI;
	for (int i = 0; i < img_size.x; i++) {
		for (int j = 0; j < img_size.y; j++) {
			v[i][j].position.x = (origin.x + (i - origin.x) * cos(a) - (i - origin.x) * sin(a));
			v[i][j].position.y = (origin.y + (j - origin.y) * cos(a) + (j - origin.y) * sin(a));
		}
	}
}
// Вращение хрен пойми как 2
void Rotate2(float angle, sf::Vertex** v, const sf::Vector2u& img_size, const sf::Vector2f& origin)
{
	const double a = angle / 180. * PI;
	for (int i = 0; i < img_size.x; i++) {
		for (int j = 0; j < img_size.y; j++) {
			v[i][j].position.x = (origin.x + (i - origin.x) * cos(a) - (j - origin.x) * sin(a));
			v[i][j].position.y = (origin.y + (i - origin.y) * cos(a) + (j - origin.y) * sin(a));
		}
	}
}
// Вращение вокруг нулевой координаты (левый верхний угол)
void Rotate3(float angle, sf::Vertex** v, const sf::Vector2u& img_size, const sf::Vector2f& origin)
{
	const sf::Vector2u half = sf::Vector2u(img_size.x / 2, img_size.y / 2);
	const double a = angle / 180. * PI;
	const double b = atan2(half.y, half.x);
	const double r = sqrt(pow(half.x, 2) + pow(half.y, 2));
	for (int i = 0; i < img_size.x; i++) {
		for (int j = 0; j < img_size.y; j++) {
			double x = half.x + i * cos(a) - j * sin(a) + r * cos(b);
			double y = half.y + i * sin(a) + j * cos(a) + r * sin(b);
			v[i][j].position.x = x;
			v[i][j].position.y = y;
		}
	}
}
// Вращение вокруг центра изображения
void Rotate4(float angle, sf::Vertex** v, const sf::Vector2u& img_size, const sf::Vector2f& origin) {
	const double a = angle / 180. * PI;
	sf::Vector2f center = sf::Vector2f(img_size.x / 2, img_size.y / 2);
	for (int j = 0; j < img_size.y; j++) {
		for (int i = 0; i < img_size.x; i++) {
			v[i][j].position.x = origin.x - ((i - center.x) * cos(a) - (j - center.y) * sin(a));
			v[i][j].position.y = origin.y + ((j - center.y) * cos(a) + (i - center.x) * sin(a));
		}
	}
}

// Указатель на выполняемую функцию при переключении от 1 до 4
void Rotate(void* f())
{
	f();
}

int main() 
{
	// Разрешение экрана
	int scr_W = sf::VideoMode::getDesktopMode().width;
	int scr_H = sf::VideoMode::getDesktopMode().height;
	// Загружаем изображение
	sf::Image img;
	img.loadFromFile("1.png");

	// Создаём матрицу из пикселей попутно заполняя их цветом изображения
	sf::Vertex** v = new sf::Vertex *[img.getSize().x];
	for (int i = 0; i < img.getSize().x; i++)
		v[i] = new sf::Vertex[img.getSize().y];
	for (int i = 0; i < img.getSize().x; i++) {
		for (int j = 0; j < img.getSize().y; j++) {
			v[i][j] = sf::Vertex(sf::Vector2f((img.getSize().x / 2) + i, (img.getSize().y / 2) + j), img.getPixel(i,j));
			
		}
	}
	// Создаём окно для отрисовки
	sf::RenderWindow wnd;
	wnd.create(sf::VideoMode(scr_W, scr_H), "Image Ratation Func_1-4: Left - Right.", sf::Style::Close);
	sf::Event event;
	int angle_c = 0;
	sf::View cam = wnd.getDefaultView();
	cam.zoom(1.5);
	wnd.setView(cam);

	void (* f)(float angle, sf::Vertex** v, const sf::Vector2u& img_size, const sf::Vector2f& origin);
	f = Rotate1;

	while (wnd.isOpen()) {
		while (wnd.pollEvent(event)) {
			if (event.type == sf::Event::KeyPressed) {
				// Обработка переключения функций вращения 1-4
				if (event.key.code == sf::Keyboard::Num1) { wnd.setTitle("Image Ratation Func_1"); f = Rotate1; }
				if (event.key.code == sf::Keyboard::Num2) { wnd.setTitle("Image Ratation Func_2"); f = Rotate2; }
				if (event.key.code == sf::Keyboard::Num3) { wnd.setTitle("Image Ratation Func_3"); f = Rotate3; }
				if (event.key.code == sf::Keyboard::Num4) { wnd.setTitle("Image Ratation Func_4"); f = Rotate4; }
				// Закрытие окна на клавишу ESC
				if (event.key.code == sf::Keyboard::Escape) wnd.close();
				// Меняем угол поворота на клавиши лево-право+
				if (event.key.code == sf::Keyboard::Left) angle_c -= 1;
				if (event.key.code == sf::Keyboard::Right) angle_c += 1;
				// Вызываем функцию вращение
				f(angle_c, v, img.getSize(), wnd.getDefaultView().getCenter());
			}
		}

		wnd.clear();
		// Рисуем нашу матрицу
		for (int i = 0; i < img.getSize().x; i++) {
			for (int j = 0; j < img.getSize().y; j++) {
				wnd.draw(&v[i][j], 1, sf::Points);
			}
		}
		wnd.display();
	}
	return 0;
}