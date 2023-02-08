#include <keyparser/terminal.hpp>
#include "paint.hpp"

using namespace KP;

int atoi(std::string str) {
	int minus = 0, num = 0;
	if (!str.empty()) {
		minus = str[0] == '-' ? 1 : 0;
		for (int i = minus; i < str.size(); i++) {
			if (str[i] < 48 || str[i] > 57) throw std::domain_error("str_num: Invalid number format!");
			num = num * 10 + (str[i] - 48);
		}
	}
	return minus ? -1 * num : num;
}

namespace Main { std::vector<Paint::Image> images(1); }

namespace Color {
	bool is_help = false;

	void onRoot(Args opts) {
		if (is_help) return;
		try {
			char index = opts[0][0], value = atoi(opts[1]);
			for (auto &i : Main::images) i = Paint::set_component(i, index, value);
		}
		catch (std::invalid_argument exp) {
			throw std::invalid_argument(std::string("Color->") + exp.what());
		}
		catch (std::domain_error exp) {
			throw std::invalid_argument(std::string("Color->") + exp.what());
		}
	}

	void onHelp(Args opts) {
		is_help = true;
		std::cout << "Справочная информация по -c / --color:\n";
		std::cout << "Обязательные параметры:\n";
		std::cout << "\t1 параметр - изменяемый компонент.\n";
		std::cout << "\t2 параметр - новое значение компонента (от 0 до 255).\n";
	}
}

namespace Circle {
	Paint::BGR fill_color, border_color = {0, 0, 0};
	int border = 0;
	bool is_help = false;

	void onRoot(Args opts) {
		if (is_help) return;
		try {
			int x = atoi(opts[0]), y = atoi(opts[1]), radius = atoi(opts[2]);
			for (auto &i : Main::images) i = Paint::put_circle(i, x, y, radius, fill_color, border, border_color);
		}
		catch (std::invalid_argument exp) {
			throw std::invalid_argument(std::string("Circle->") + exp.what());
		}
		catch (std::domain_error exp) {
			throw std::invalid_argument(std::string("Circle->") + exp.what());
		}
	}

	void onFill(Args opts) {
		try {
			fill_color.r = atoi(opts[0]);
			fill_color.g = atoi(opts[1]);
			fill_color.b = atoi(opts[2]);
		}
		catch (std::domain_error exp) {
			throw std::invalid_argument(std::string("Circle->") + exp.what());
		}
	}

	void onBorder(Args opts) {
		try {
			border = atoi(opts[0]);
			border_color.r = atoi(opts[1]);
			border_color.g = atoi(opts[2]);
			border_color.b = atoi(opts[3]);
		}
		catch (std::domain_error exp) {
			throw std::invalid_argument(std::string("Circle->") + exp.what());
		}
	}

	void onHelp(Args opts) {
		is_help = true;
		std::cout << "Справочная информация по -r (--circle):\n";
		std::cout << "Флаги:\n";
		std::cout << "\t-f (--fill):\t\n";
		std::cout << "\t-b (--border):\t\n";
		std::cout << "Обязательные параметры:\t\n";
		std::cout << "\tФлаг -f (--fill): параметры - RGB компонент: <r> <g> <b>.\n";
		std::cout << "\t1 параметр - положение центра по горизонтали.\n";
		std::cout << "\t2 параметр - положение центра по вертикали.\n";
		std::cout << "\t3 параметр - радиус окружности.\n";
		std::cout << "Необязательные параметры:\t\n";
		std::cout << "\tФлаг -b (--border): параметры - ширина края и RGB компонент: <border> <r> <g> <b>.\n";
	}
}

namespace Slice {
	bool is_help = false;

	void onRoot(Args opts) {
		if (is_help) return;
		try {
			int x_lines = atoi(opts[0]), y_lines = atoi(opts[1]);
			for (auto &i : Paint::slice_image(Main::images[0], x_lines, y_lines)) Main::images.push_back(i);
			Main::images.erase(Main::images.begin());
		}
		catch (std::invalid_argument exp) {
			throw std::invalid_argument(std::string("Slice->") + exp.what());
		}
		catch (std::domain_error exp) {
			throw std::invalid_argument(std::string("Slice->") + exp.what());
		}
	}

	void onHelp(Args opts) {
		is_help = true;
		std::cout << "Справочная информация по -x / --slice:\n";
		std::cout << "Обязательные параметры:\n";
		std::cout << "\t1 параметр - число вертикальных изображений.\n";
		std::cout << "\t2 параметр - число горизонтальных изображений.\n";
	}
}

namespace Square {
	bool is_help = false;

	void onRoot(Args opts) {
		if (is_help) return;
		try {
			std::vector<int> D;
			for (auto &i : opts) D.push_back(atoi(i));
			for (auto &i : Main::images) i = Paint::put_square(i, D[0], D[1], D[2], D[3], D[4], D[5]);
		}
		catch (std::invalid_argument exp) {
			throw std::invalid_argument(std::string("Square->") + exp.what());
		}
		catch (std::domain_error exp) {
			throw std::invalid_argument(std::string("Square->") + exp.what());
		}
	}

	void onHelp(Args opts) {
		is_help = true;
		std::cout << "Справочная информация по -s / --square:\n";
		std::cout << "Обязательные параметры:\n";
		std::cout << "\t1 параметр - положение нижнего левого угла по горизонтали.\n";
		std::cout << "\t2 параметр - положение нижнего левого угла по вертикали.\n";
		std::cout << "\t3 параметр - положение верхнего правого угла по горизонтали.\n";
		std::cout << "\t4 параметр - положение верхнего правого угла по вертикали.\n";
		std::cout << "\t5 параметр - место по горизонтали для перемещения нижнего левого угла.\n";
		std::cout << "\t6 параметр - место по вертиакли для перемещения нижнего левого угла.\n";
	}
}

namespace Main {
	std::string save_path;
	bool is_help = false;

	void set_load_path(Args opts) {
		if (opts.size() != 2) throw std::invalid_argument("Main: No working file specified!");
		try {
			images[0].load(opts[1]);
			save_path = opts[1];
			int pos = opts[1].find(".bmp");
			if (pos) save_path = opts[1].substr(0, pos) + "_0";
		}
		catch (std::domain_error exp) {
			throw std::invalid_argument(std::string("Main->") + exp.what());
		}
	}

	void set_save_path(Args opts) {
		if (opts.size() != 1) throw std::invalid_argument("Main: Only one arguments expected after \"o/out\" key!");
		try {
			save_path = opts[0];
			int pos = opts[0].find(".bmp");
			if (pos) save_path = opts[0].substr(0, pos);
		}
		catch (std::domain_error exp) {
			throw std::invalid_argument(std::string("Main->") + exp.what());
		}
	}

	void onColor(Args opts) {
		Terminal terminal(Terminal::RS_S);
		terminal.setRootRange(2, 2);
		terminal.setRoot(Color::onRoot);
		terminal.setKey(Key('h', "help",	0, 0),	Color::onHelp);
		terminal.execute(opts);
	}

	void onCircle(Args opts) {
		Terminal terminal(Terminal::RS_S);
		terminal.setRootRange(3, 3);
		terminal.setRoot(Circle::onRoot);
		terminal.setKey(Key('f', "fill",	3, 3),	Circle::onFill);
		terminal.setKey(Key('b', "border",	4, 4),	Circle::onBorder);
		terminal.setKey(Key('h', "help",	0, 0),	Circle::onHelp);
		terminal.execute(opts);
	}

	void onSlice(Args opts) {
		Terminal terminal(Terminal::RS_S);
		terminal.setRootRange(2, 2);
		terminal.setRoot(Slice::onRoot);
		terminal.setKey(Key('h', "help",	0, 0),	Slice::onHelp);
		terminal.execute(opts);
	}

	void onSquare(Args opts) {
		Terminal terminal(Terminal::RS_S);
		terminal.setRootRange(6, 6);
		terminal.setRoot(Square::onRoot);
		terminal.setKey(Key('h', "help",	0, 0),	Square::onHelp);
		terminal.execute(opts);
	}

	void onHelp(Args opts) {
		is_help = true;
		std::cout << "Справочная информация по программе:\n";
		std::cout << "Вызов:\t<программа> <файл> [-o (--out), -c (--color), -r (--circle), -x (--slice), -s (--square), -h (--help)]\n";
		std::cout << "Флаги:\n";
		std::cout << "\t-o (--out):\tПуть и имя файла сохранения.\n";
		std::cout << "\t-c (--color):\tИзменение значения RGB компонента.\n";
		std::cout << "\t-r (--circle):\tРисование круга или круга с отверстием.\n";
		std::cout << "\t-x (--slice):\tРазрезание исходного изображение на несколько.\n";
		std::cout << "\t-s (--square):\tКопирование и перемещения части изображения в другое место.\n";
		std::cout << "\t-h (--help):\tПолучение справочной информации.\n";
	}

	void onFinal() {
		if (is_help) return;
		try {
			if (images.size() == 1) images[0].save(save_path + ".bmp");
			else for (int i = 0; i < images.size(); i++) {
				std::string str;
				for (int j = i; j > 0; j /= 10) str = char(j % 10 + 48) + str;
				images[i].save(save_path + str + ".bmp");
			}
		}
		catch (std::invalid_argument exp) {
			throw std::invalid_argument(std::string("Main->") + exp.what());
		}
	}
}

int main(int argc, char *argv[]) {
	try {
		Terminal terminal;
		terminal.setRootRange(2, 2);
		terminal.setRoot(Main::set_load_path);
		terminal.setKey(Key('o', "output",	1, 1),	Main::set_save_path);
		terminal.setKey(Key('c', "color",	2),		Main::onColor);
		terminal.setKey(Key('r', "circle",	3),		Main::onCircle);
		terminal.setKey(Key('x', "slice",	2),		Main::onSlice);
		terminal.setKey(Key('s', "square",	6),		Main::onSquare);
		terminal.setKey(Key('h', "help",	0, 0),	Main::onHelp);
		terminal.setFinal(Main::onFinal);
		terminal.execute(argc, argv);
	}
	catch (std::invalid_argument e) {
		std::cout << e.what() << "\n";
	}

	return 0;
}