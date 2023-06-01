#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <conio.h>

using namespace std;

vector<string> var_blocks{ "  %  % %%", "  %  %  %", "    %% %%", " %  %  %%", "   %%  %%", "    % %%%", "    %%%% " };//������ � ���������� �������
vector<vector<char>> Field(20, vector<char>(10)); //������ �������� - ������� ����

class Block //����� �����
{
public:
	friend class GameField;//������ ����� ������������� ��� GameField,
	string cur;//������, �������� ������� ����
	int heg; int wid; int var;//�������������� �����
	Block()
	{
		var = rand() % 7; //�������� ���� ��������� �������
		cur = var_blocks[var];

	}
	void turn(Block* bl, GameField& GF)//������� �������� �����
	{
		if ((*bl).heg > 3 && (*bl).heg < 19)//������� �������������� ������ ����� ������ ��������� � ��������� 3<heg<19
		{
			bool fl = true;
			for (int i = 0; i < 9; i++)//�������� �� ����������� ����� % � ����� ��������
			{
				if (Field[heg - i / 3][wid + i % 3] == '%')
					fl = false;
			}
			if (fl)//���� ������� �������� �����������
			{
				string s{ cur[6], cur[3],cur[0], cur[7] ,cur[4] ,cur[1] ,cur[8] ,cur[5] ,cur[2] };//���������� ����

				if (s[6] == ' ' && s[7] == ' ' && s[8] == ' ')//���� ������ ����� ����� ������� ��������� �� �������� �� ��� ���� �������� ���� ������ ����
				{
					string s1{ cur[8] ,cur[5] ,cur[2],cur[6], cur[3],cur[0], cur[7] ,cur[4] ,cur[1] };//����������� ��������
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

class GameField //����� �������� ����
{
private:
	int Score = -200;
public:
	friend class Block;

	int nextstep(Block* bl)//������� ��� ����������� ���������� �����
	{
		bool flag = true;
		if ((*bl).heg - 2 > 0 && (*bl).heg <= 19) //�������� �� ��, ����� �� �������� ������ ����
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

		if (flag || (*bl).heg > 19)//���� ���� ����� ����������� �� �������� ��������� ���� (������)
		{
			(*bl).heg--;
		}
		else //����� ��� �������� �� ���� - � ���� ������������ ������ �� ���� �������� - �������������� ����� % � ������ ��� ������������ ������
		{
			if ((*bl).cur[0] == '%') Field[(*bl).heg][(*bl).wid] = (*bl).cur[0]; if ((*bl).cur[1] == '%') Field[(*bl).heg][(*bl).wid + 1] = (*bl).cur[1]; if ((*bl).cur[2] == '%') Field[(*bl).heg][(*bl).wid + 2] = (*bl).cur[2];
			if ((*bl).cur[3] == '%')Field[(*bl).heg - 1][(*bl).wid] = (*bl).cur[3]; if ((*bl).cur[4] == '%') Field[(*bl).heg - 1][(*bl).wid + 1] = (*bl).cur[4]; if ((*bl).cur[5] == '%') Field[(*bl).heg - 1][(*bl).wid + 2] = (*bl).cur[5];
			if ((*bl).cur[6] == '%')Field[(*bl).heg - 2][(*bl).wid] = (*bl).cur[6]; if ((*bl).cur[7] == '%') Field[(*bl).heg - 2][(*bl).wid + 1] = (*bl).cur[7]; if ((*bl).cur[8] == '%') Field[(*bl).heg - 2][(*bl).wid + 2] = (*bl).cur[8];
			if (bl->heg >= 19) //���� ��������� ������� ������ ��������� � ����� �������� ����, �� ���� �������������
			{
				THEEND();
				return 0;
			}
			(*bl).heg = 21, (*bl).wid = rand() % 7;//������� ����� ���������� ��� ������ - ���������� ����������� �������� ����� ������ (��������� ����� ������)
			int v = rand() % 7;
			(*bl).cur = var_blocks[v];
			return 1;
		}
	}

	void visGameField(Block* bl) //��������� �������� ����
	{
		for (int i = 19; i >= 0; i--)
		{
			cout << '|';
			if ((*bl).heg >= i && (*bl).heg - 2 <= i)
			{
				for (int j = 0; j < (*bl).wid; j++)//��������� ����� �������� ����
				{
					cout << Field[i][j];
				}
				for (int j = 0; j <= 2; j++) //��������� ������ �� ������� ����
				{
					cout << (*bl).cur[((*bl).heg - i) * 3 + j];
				}
				for (int j = (*bl).wid + 3; j <= 9; j++)//��������� ����� �������� ����
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
	void combustion()//������� ��� ����������� ��������� ��������� �����
	{
		vector<int> lines; int num = 0;
		for (int i = 0; i < 20; i++)
		{
			bool fl = true;
			for (int j = 0; j < 10; j++)//�������� �� ��, ��� ����� ��������� �������
			{
				if (Field[i][j] == ' ')
					fl = false;
			}
			if (fl)//���� ����� ��������� �������
			{
				Score += 10;//�� �������� ����
				num++; lines.push_back(i);
				for (int j = 0; j < 10; j++)
				{
					Field[i][j] = ' ';//����� ����������
				}
			}
		}

		if (num > 0)
		{
			for (int i = 1; i < 20; i++)
			{
				for (int j = 0; j < 10; j++)
				{
					if (Field[i][j] == '%')//��� ����� ����������� ��� ��������� ������ ������
					{
						int cnt = 0;
						for (int k = 0; k < lines.size(); k++)
						{
							if (i > lines[k])
							{
								cnt++;//������� ���������� ����� �� ������� ������ ���������� ����� ��� ���������� �������
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
	void VisRow(const int j)//������� ��� ����������� ������ �������� ����
	{
		for (int i = 0; i < 10; i++)
		{
			cout << Field[j][i];
		}
	}

	bool NormLeft(Block* bl)//������� ��� �������� ����������� ������ ������ �����
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
			for (int i = 0; i < 9; i++)//�����������, ���� �� ����� �� ������ ������ �����
			{
				if (!(Field[(*bl).heg - i / 3][(*bl).wid + i % 3 - 1] != '%' && (*bl).cur[i] == '%' || (*bl).cur[i] != '%'))
					fl = false;
			}
			return fl;
		}
		else
			return false;
	}


	bool NormRight(Block* bl)//������� ��� �������� ����������� ������ ������ ������
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
			for (int i = 0; i < 9; i++)//�������� �� ����������� ������ ������ ������ �� ������
			{
				if (!(Field[(*bl).heg - i / 3][(*bl).wid + i % 3 + 1] != '%' && (*bl).cur[i] == '%' || (*bl).cur[i] != '%'))
					fl = false;
			}
			return fl;
		}
		else
			return false;
	}

	void THEEND()//������� ��� ����������� ����� ����
	{
		system("cls");
		ifstream fin("result.txt");
		string str; int res;
		fin >> str;
		fin.close();
		ofstream fout("result.txt"); //������ ��� ������ � ����
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
	srand((unsigned)time(NULL));//��� ��������� ��������� �����

	GameField GF;//�������� ������� �������� ����
	Block* bl = new Block;//�������� ������� �����
	(*bl).heg = 21, (*bl).wid = rand() % 7; //������� ��������� ���������� ��� �����
	while (true)
	{
		system("cls");
		char input;

		if (_kbhit() && (*bl).heg < 19 && (*bl).heg >= 3)//��������� ��������
		{
			input = _getch();
			if (input == 'a')
			{
				if (GF.NormLeft(bl)) //���� �������� �������� ����� ������
					(*bl).wid--;//�� ���� ���������� ������
			}
			if (input == 'd')
			{
				if (GF.NormRight(bl))//���� �������� �������� ����� �������
					(*bl).wid++;//�� ���� ���������� �������
			}
			if (input == 's')
			{
				if ((*bl).heg > 4)//���� �������� ��������� ����� ����
					GF.nextstep(bl);//�� ���� ���������� ��� ��� ����
			}
			if (input == ' ')//���� ����� ������
			{
				(*bl).turn(bl, GF);//�� ������������ ����
			}
		}

		if (!GF.nextstep(bl))
		{
			return 0;
		} //������� �� ���������, ���� ������� �� ������ "������� �� �����"
		GF.combustion();

		GF.visGameField(bl);//����� ������ ��� ����������� ����

		Sleep(90);//�������� ��������� - ���������
	}

	return 0;
}
