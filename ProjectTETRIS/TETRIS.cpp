#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <conio.h>

using namespace std;

vector<string> var_blocks{ "  %  % %%", "  %  %  %", "    %% %%", " %  %  %%", "   %%  %%", "    % %%%", "    %%%% " };//вектор с различными блоками
vector<vector<char>> Field(20, vector<char>(10)); //вектор векторов - игровое поле

class Block //класс блока
{
public:
	friend class GameField;//делаем класс дружественным для GameField,
	string cur;//строка, хранящая текущий блок
	int heg; int wid; int var;//характеристики блока
	Block()
	{
		var = rand() % 7; //выбираем блок случайным образом
		cur = var_blocks[var];

	}
	void turn(Block* bl, GameField& GF)//Функция поворота блока
	{
		if ((*bl).heg > 3 && (*bl).heg < 19)//поворот осуществляется только когда деталь находится в диапазоне 3<heg<19
		{
			bool fl = true;
			for (int i = 0; i < 9; i++)//проверка на присутствие знака % в месте поворота
			{
				if (Field[heg - i / 3][wid + i % 3] == '%')
					fl = false;
			}
			if (fl)//если поворот возможно осуществить
			{
				string s{ cur[6], cur[3],cur[0], cur[7] ,cur[4] ,cur[1] ,cur[8] ,cur[5] ,cur[2] };//повернутый блок

				if (s[6] == ' ' && s[7] == ' ' && s[8] == ' ')//если нижняя часть блока состоит полностью из пробелов то нам надо сместить тело детали вниз
				{
					string s1{ cur[8] ,cur[5] ,cur[2],cur[6], cur[3],cur[0], cur[7] ,cur[4] ,cur[1] };//аналогичная проверка
					if (s1[6] == ' ' && s1[7] == ' ' && s1[8] == ' ')
					{
						string s2{ cur[7] ,cur[4] ,cur[1],  cur[8] ,cur[5] ,cur[2],cur[6], cur[3],cur[0] };
						cur = s2;
					}
					else
					{
						cur = s1;
					}
				}
				else
				{
					cur = s;
				}

			}
		}
	}
};

class GameField //класс игрового поля
{
private:
	int Score = -200;
public:
	friend class Block;

	int nextstep(Block* bl)//функция для отображения следующего кадра
	{
		bool flag = true;
		if ((*bl).heg - 2 > 0 && (*bl).heg <= 19) //проверка на то, может ли деталька падать вниз
		{
			for (int i = 0; i < 9; i++)
			{
				if (!(Field[(*bl).heg - i / 3 - 1][(*bl).wid + i % 3] != '%' && (*bl).cur[i] == '%' || (*bl).cur[i] != '%'))
					flag = false;
			}
		}
		else
		{
			flag = false;
		}

		if (flag || (*bl).heg > 19)//если есть такая возможность то деталька двигается вниз (падает)
		{
			(*bl).heg--;
		}
		else //иначе она остается на поле - в поле записываются данные из этой детальки - отрисовываются знаки % в местах где остановилась деталь
		{
			if ((*bl).cur[0] == '%') Field[(*bl).heg][(*bl).wid] = (*bl).cur[0]; if ((*bl).cur[1] == '%') Field[(*bl).heg][(*bl).wid + 1] = (*bl).cur[1]; if ((*bl).cur[2] == '%') Field[(*bl).heg][(*bl).wid + 2] = (*bl).cur[2];
			if ((*bl).cur[3] == '%')Field[(*bl).heg - 1][(*bl).wid] = (*bl).cur[3]; if ((*bl).cur[4] == '%') Field[(*bl).heg - 1][(*bl).wid + 1] = (*bl).cur[4]; if ((*bl).cur[5] == '%') Field[(*bl).heg - 1][(*bl).wid + 2] = (*bl).cur[5];
			if ((*bl).cur[6] == '%')Field[(*bl).heg - 2][(*bl).wid] = (*bl).cur[6]; if ((*bl).cur[7] == '%') Field[(*bl).heg - 2][(*bl).wid + 1] = (*bl).cur[7]; if ((*bl).cur[8] == '%') Field[(*bl).heg - 2][(*bl).wid + 2] = (*bl).cur[8];
			if (bl->heg >= 19) //если последняя упавшая деталь упирается в конец игрового поля, то игра заканчивается
			{
				THEEND();
				return 0;
			}
			(*bl).heg = 21, (*bl).wid = rand() % 7;//задание новых параметров для детали - фактически абстрактное создание новой детали (генерация новой детали)
			int v = rand() % 7;
			(*bl).cur = var_blocks[v];
			return 1;
		}
	}

	void visGameField(Block* bl) //отрисовка игрового поля
	{
		for (int i = 19; i >= 0; i--)
		{
			cout << '|';
			if ((*bl).heg >= i && (*bl).heg - 2 <= i)
			{
				for (int j = 0; j < (*bl).wid; j++)//отрисовка части игрового поля
				{
					cout << Field[i][j];
				}
				for (int j = 0; j <= 2; j++) //отрисовка детали на игровом поле
				{
					cout << (*bl).cur[((*bl).heg - i) * 3 + j];
				}
				for (int j = (*bl).wid + 3; j <= 9; j++)//отрисовка части игрового поля
				{
					cout << Field[i][j];
				}
			}
			else VisRow(i);
			cout << '|';
			if (i == 10)
			{
				cout << "\tScore: " << Score;
			}
			cout << endl;
		}
		cout << " ----------";
	}
	void combustion()//функция для уничтожения полностью собранной линии
	{
		vector<int> lines; int num = 0;
		for (int i = 0; i < 20; i++)
		{
			bool fl = true;
			for (int j = 0; j < 10; j++)//проверка на то, что линия полностью собрана
			{
				if (Field[i][j] == ' ')
					fl = false;
			}
			if (fl)//если линия полностью собрана
			{
				Score += 10;//то меняется счет
				num++; lines.push_back(i);
				for (int j = 0; j < 10; j++)
				{
					Field[i][j] = ' ';//линия обнуляется
				}
			}
		}

		if (num > 0)
		{
			for (int i = 1; i < 20; i++)
			{
				for (int j = 0; j < 10; j++)
				{
					if (Field[i][j] == '%')//все блоки оказавшиеся над сгоревшей линией падают
					{
						int cnt = 0;
						for (int k = 0; k < lines.size(); k++)
						{
							if (i > lines[k])
							{
								cnt++;//подсчет количества линий на которые должны спуститься блоки над сгоревшими линиями
							}
						}
						if (cnt > 0)
						{
							Field[i][j] = ' ';
							Field[i - cnt][j] = '%';
						}
					}
				}
			}
		}
	}
	void VisRow(const int j)//функция для отображения строки игрового поля
	{
		for (int i = 0; i < 10; i++)
		{
			cout << Field[j][i];
		}
	}

	bool NormLeft(Block* bl)//функция для проверки возможности сдвига детали влево
	{
		if ((*bl).wid == 0)
		{
			if (bl->cur[0] == ' ' && bl->cur[3] == ' ' && bl->cur[6] == ' ')
			{
				string cur = bl->cur;
				string s{ cur[1], cur[2],cur[0], cur[4] ,cur[5] ,cur[3] ,cur[7] ,cur[8] ,cur[6] };
				bl->cur = s;
			}
		}

		bool fl = true;
		if ((*bl).wid > 0 && (*bl).heg <= 19)
		{
			for (int i = 0; i < 9; i++)//проверяется, есть ли слева от детали другие блоки
			{
				if (!(Field[(*bl).heg - i / 3][(*bl).wid + i % 3 - 1] != '%' && (*bl).cur[i] == '%' || (*bl).cur[i] != '%'))
					fl = false;
			}
			return fl;
		}
		else
			return false;
	}


	bool NormRight(Block* bl)//функция для проверки возможности сдвига детали вправо
	{
		if ((*bl).wid == 7)
		{
			if (bl->cur[2] == ' ' && bl->cur[5] == ' ' && bl->cur[8] == ' ')
			{
				string cur = bl->cur;
				string s{ cur[2], cur[0],cur[1], cur[5] ,cur[3] ,cur[4] ,cur[8] ,cur[6] ,cur[7] };
				bl->cur = s;
			}
		}

		bool fl = true;
		if ((*bl).wid < 7 && (*bl).heg <= 19)
		{
			for (int i = 0; i < 9; i++)//проверка на присутствие других блоков справа от детали
			{
				if (!(Field[(*bl).heg - i / 3][(*bl).wid + i % 3 + 1] != '%' && (*bl).cur[i] == '%' || (*bl).cur[i] != '%'))
					fl = false;
			}
			return fl;
		}
		else
			return false;
	}

	void THEEND()//функция для отображения конца игры
	{
		system("cls");
		ifstream fin("result.txt");
		string str; int res;
		fin >> str;
		fin.close();
		ofstream fout("result.txt"); //объект для записи в файл
		res = stoi(str);
		if (res < Score)
		{
			fout << Score;
		}
		else
		{
			fout << res;
		}


		cout << "\n\n\n\n\n\n\t\t\t\t\tYOUR SCORE: " << Score << "\n\n\n\n";
		cout << "\t\t\t\t\tMaximum Score: " << max(Score, res) << "\n\n\n\n";
		fout.close();
	}
};

int main()
{
	srand((unsigned)time(NULL));//для генерации случайных чисел

	GameField GF;//создание объекта игрового поля
	Block* bl = new Block;//создание объекта блока
	(*bl).heg = 21, (*bl).wid = rand() % 7; //задание начальных параметров для блока
	while (true)
	{
		system("cls");
		char input;

		if (_kbhit() && (*bl).heg < 19 && (*bl).heg >= 3)//обработка сигналов
		{
			input = _getch();
			if (input == 'a')
			{
				if (GF.NormLeft(bl)) //если возможно движения блока налево
					(*bl).wid--;//то блок сдвигается налево
			}
			if (input == 'd')
			{
				if (GF.NormRight(bl))//если возможно движения блока направо
					(*bl).wid++;//то блок сдвигается направо
			}
			if (input == 's')
			{
				if ((*bl).heg > 4)//если возможно ускорение блока вниз
					GF.nextstep(bl);//то блок сдвигается еще раз вниз
			}
			if (input == ' ')//если нажат пробел
			{
				(*bl).turn(bl, GF);//то поворачиваем блок
			}
		}

		if (!GF.nextstep(bl))
		{
			return 0;
		} //выходим из программы, если избашня из блоков "доросла до верха"
		GF.combustion();

		GF.visGameField(bl);//вызов метода для отображения поля

		Sleep(90);//задержка программы - умедление
	}

	return 0;
}
