

#include <iostream>
#include <conio.h>
#include <windows.h>
#include <string>
#include <stdexcept>
#include <fstream>
#include "Settings.h"
struct CORDINATE
{
    int x = -1;
    int y = -1;
    bool Its_You = false;
    bool Its_You_Head = false;
    bool Its_Eat = false;
};
struct Shake
{
    int x;
    int y;
    Shake* Next;
};
struct Record
{
    int points;
    std::string name;
};
bool win = false;
class ZMEIKA
{
    CORDINATE matrix_map[75][15];
    Shake* head = nullptr;
    int ALL_CODINATE = 0; //длина змейки
    int NAPRAVLENIE_X = 0; //1 вправо  -1 влево //0 ничего
    int NAPRAVLENIE_Y = 1; //1 вниз -1 вверх //0 ничего
    std::string name_player = "";
    public:
        void SetName(std::string s)
        {
            name_player = s;
        }
        std::string GetName()
        {
            return name_player;
        }
        void SetPointsNull()
        {
            ALL_CODINATE = 0;
        }
        void Save()
        {
            int count = 0;
            Shake* h = head;
            while (h != nullptr)
            {
                count++;
                h = h->Next;
            }
            h = head;
            CORDINATE* GG = new CORDINATE[count];
            count = 0;
            while (h != nullptr)
            {
                CORDINATE list = {h->x,h->y};
                GG[count] = list;
                count++;
                h = h->Next;
            }
            std::ofstream file(SAVE_FILE_NAME, std::ios::binary);
            file.write(reinterpret_cast<const char*>(&matrix_map), sizeof(matrix_map));
            file.write(reinterpret_cast<const char*>(&NAPRAVLENIE_X), sizeof(NAPRAVLENIE_X));
            file.write(reinterpret_cast<const char*>(&NAPRAVLENIE_Y), sizeof(NAPRAVLENIE_Y));
            file.write(reinterpret_cast<const char*>(&name_player), sizeof(name_player));
            file.write(reinterpret_cast<const char*>(GG),sizeof(CORDINATE) * count);
            file.close();
            GG = nullptr;
            h = nullptr;
            delete h;
            delete[] GG;


        }
        void Download()
        {
            std::ifstream file(SAVE_FILE_NAME, std::ios::binary);
            file.read(reinterpret_cast<char*>(&matrix_map),sizeof(matrix_map));
            file.read(reinterpret_cast<char*>(&NAPRAVLENIE_X), sizeof(NAPRAVLENIE_X));
            file.read(reinterpret_cast<char*>(&NAPRAVLENIE_Y), sizeof(NAPRAVLENIE_Y));
            file.read(reinterpret_cast<char*>(&name_player), sizeof(name_player));
            int bud = file.tellg();
            file.seekg(0,std::ios::end);
            int bud1 = file.tellg();
            file.seekg(bud,std::ios::beg);
            int size = (bud1 - bud) / sizeof(CORDINATE);
            CORDINATE* GG = new CORDINATE[size];
            file.read(reinterpret_cast<char*>(GG), size * sizeof(CORDINATE));
            file.close();
            ALL_CODINATE = size - 1;
            int count_obj = 1;
            Shake* h = new Shake{GG[0].x,GG[0].y};
            
            while (count_obj != size)
            {
                Shake* hh = new Shake{ GG[count_obj].x,GG[count_obj].y };
                h->Next = hh;
                if (count_obj == 1)
                {
                    head = h;
                }
                h = hh;
                count_obj++;
                hh = nullptr;
                delete hh;
            }
            h = nullptr;
            delete h;
            GG = nullptr;
            delete[] GG;

        }
        void MATRIX_FULL_NEW_GAME()
        {
            ALL_CODINATE = 0;
            NAPRAVLENIE_X = 0;
            NAPRAVLENIE_Y = 1;
            for (int y = 0; y < 15; y++)
            {
                for (int x = 0; x < 75; x++)
                {
                    if (x != 35 && y != 0)
                    {
                        matrix_map[x][y] = CORDINATE{ x,y };
                    }
                    else if (x == 35 && y == 0)
                    {
                        matrix_map[x][y] = CORDINATE{ 35,0,true,true,false };
                        head = new Shake{ 35,0 };
                    }
                }
            }
        }
        void DRAW_MAP()
        {
            for (int y = 0; y < 15; y++)
            {
                for (int x = 0; x < 75; x++)
                {
                    if (!matrix_map[x][y].Its_You && !matrix_map[x][y].Its_Eat)
                    {
                        std::cout << "_";
                    }
                    else if(matrix_map[x][y].Its_You_Head)
                    {
                        std::cout << "%";
                    }
                    else if(matrix_map[x][y].Its_You)
                    {
                        std::cout << "#";
                    }
                    else if (matrix_map[x][y].Its_Eat)
                    {
                        std::cout << "*";
                    }
                    
                }
                std::cout << "\n";
            }
        }
        void Spawn_Eat(int count)
        {
            srand(time(NULL));
            for (int i = 0; i < count; i++)
            {
                int x = rand() % 75;
                int y = rand() % 15;
                if (!matrix_map[x][y].Its_You && !matrix_map[x][y].Its_You_Head && !matrix_map[x][y].Its_Eat)
                {
                    matrix_map[x][y].Its_Eat = true;
                }
                else
                {
                    i--;
                }
            }
        }
        void New_Naprablenie(int s)//75 = влево  72 = вверх 77 = вправо 80 вниз
        {

            if (s == 224)
                return;
            if (s == 75)
            {
                NAPRAVLENIE_X = -1;
                NAPRAVLENIE_Y = 0;
            }
            else if (s == 72)
            {
                NAPRAVLENIE_X = 0;
                NAPRAVLENIE_Y = -1;
            }
            else if (s == 77)
            {
                NAPRAVLENIE_X = 1;
                NAPRAVLENIE_Y = 0;
            }
            else if (s == 80)
            {
                NAPRAVLENIE_X = 0;
                NAPRAVLENIE_Y = 1;
            }
            else if (s == 9)
            {
                std::cout << "Пауза\n1. Нажмите Tab чтобы сохранить прогресс и выйти\n2. Нажмите любую клавишу кроме Tab чтобы продожить игру\n";
                int s = _getch();
                if (s == 9)
                {
                    Save();
                    win = true;
                }

                
            }

        }
        int GetPoint()
        {
            return ALL_CODINATE;
        }
        void PrintRecordList()
        {
            std::ifstream files(RECORDS_FILE_NAME, std::ios::binary);            
            files.seekg(0, std::ios::end);
            int size = files.tellg();
            int count_records = size / sizeof(Record);
            files.seekg(0,std::ios::beg);
            Record* record = new Record[count_records];
            files.read(reinterpret_cast<char*>(record), size);
            files.close();
            for (int k = 0; k < count_records;k++)
            {
                Record last_record = record[k];
                float min_points = last_record.points;
                int index = k;
                for (int l = 0;l < count_records;l++)
                {
                    if (min_points < record[l].points)
                    {
                        record[index] = record[l];
                        record[l] = last_record;
                        index = l;
                    }
                }
            }

            for (int i = 0; i < count_records;i++)
            {
                std::cout << "\n" << i+1 << ". name: " << record[i].name << " point: " << record[i].points;
            }
            record = nullptr;
            delete[] record;
        }
        void AddRecord(Record record)
        {
            std::ofstream file(RECORDS_FILE_NAME, std::ios::app);
            file.write(reinterpret_cast<const char*>(&record), sizeof(Record));
            file.close();
        }
        bool GoZmeika()
        {
            int _x = head->x + NAPRAVLENIE_X;
            int _y = head->y + NAPRAVLENIE_Y;
            if ((_x > 74 || _y > 14 || _x < 0 || _y < 0))
            {
                Shake* s = head;
                while (s != nullptr)
                {
                    Shake* s2 = s->Next;
                    delete s;
                    s = s2;
                }
                s = nullptr;
                delete s;
                return false;
            }
            if (matrix_map[_x][_y].Its_Eat)
            {
                matrix_map[_x][_y].Its_Eat = false;
                matrix_map[_x][_y].Its_You = true;
                matrix_map[_x][_y].Its_You_Head = true;
                Shake* last_head = head;
                head = new Shake{ _x ,_y,last_head };  
                
                ALL_CODINATE++;
                if (ALL_CODINATE != 1224)
                    Spawn_Eat(1);
                last_head = nullptr;
                delete last_head;
                return true;
            }  
            else if (matrix_map[_x][_y].Its_You)
            {
                
                Shake* s = head;
                while (s != nullptr)
                {
                    Shake* s2 = s->Next;
                    delete s;
                    s = s2;
                }
                s = nullptr;
                delete s;
                return false;
            }
            else
            {
                Shake* h = head;
                float last_x = -1;
                float last_y = -1;
                while (h != nullptr)
                {
                    if (h == head)
                    {
                        last_x = h->x;
                        last_y = h->y;
                        matrix_map[h->x][h->y].Its_You = false;
                        matrix_map[h->x][h->y].Its_You_Head = false;

                        h->x = h->x + NAPRAVLENIE_X;
                        h->y = h->y + NAPRAVLENIE_Y;
                        matrix_map[h->x][h->y].Its_You = true;
                        matrix_map[h->x][h->y].Its_You_Head = true;
                        
                        h = h->Next;
                        
                    }
                    else
                    {
                        float g = h->x;
                        float g2 = h->y;
                        matrix_map[h->x][h->y].Its_You = false;
                        matrix_map[h->x][h->y].Its_You_Head = false;
                        h->x = last_x;
                        h->y = last_y;
                        last_x = g;
                        last_y = g2;
                        matrix_map[h->x][h->y].Its_You = true;
                        matrix_map[h->x][h->y].Its_You_Head = false;

                        h = h->Next;
                        
                    }
                }
                h = nullptr;
                delete h;
                return true;
            }
        }

};

int main()
{
    std::setlocale(LC_ALL,"Russian");
    ZMEIKA* Game = new ZMEIKA();
    while (true)
    {
        win = false;
        Game->SetPointsNull();
        system("cls");
        std::cout << "1. Новая игра\n2. Загрузить игру\n3. Таблица рекордов\n4. Выход\nВведите номер выполнения: ";
        int INT = 0;
        std::cin >> INT;
        if (INT < 1 || INT > 4)
        {
            continue;
        }
        if (INT == 1)
        {
            std::string name_player = "";
            std::cout << "Введите ник пользователя: ";
            std::cin.ignore();
            std::getline(std::cin, name_player);
            if (name_player.length() == 0)
            {
                std::cout << "\nНик не должен быть пустым\n";
                Sleep(2000);
                continue;
            }
            Game->SetName(name_player);
            Game->MATRIX_FULL_NEW_GAME();
            Game->Spawn_Eat(10);
            while (!win)
            {
                if (Game->GetPoint() == 1124)
                {
                    system("cls");
                    win = true;
                    std::cout << "You Win!\nНик: " << Game->GetName() << " Очки: " << Game->GetPoint();
                    if (_getch())
                        continue;
                    break;
                }
                if (_kbhit())
                    Game->New_Naprablenie(_getch());
                system("cls");
                if (_kbhit())
                    Game->New_Naprablenie(_getch());
                Game->DRAW_MAP();
                if (_kbhit())
                    Game->New_Naprablenie(_getch());
                bool status = Game->GoZmeika();
                if (_kbhit())
                    Game->New_Naprablenie(_getch());
                if (!status)
                    break;              
                if (_kbhit())
                    Game->New_Naprablenie(_getch());
                std::cout << "\nОчки: " << Game->GetPoint();
                Sleep(100);
                if (_kbhit())
                    Game->New_Naprablenie(_getch());
            }
            if (!win)
            {
                system("cls");
                std::cout << "You Lose:(\nНик: " << Game->GetName() << " Очки: " << Game->GetPoint();

                Record r = { Game->GetPoint(),Game->GetName() };
                Game->AddRecord(r);
                if (_getch())
                    continue;
            }
        }
        else if (INT == 2)
        {
            Game->Download();
            while (!win)
            {
                if (Game->GetPoint() == 1124)
                {
                    system("cls");
                    win = true;
                    std::cout << "You Win!\nНик: " << Game->GetName() << " Очки: " << Game->GetPoint();
                    if (_getch())
                        continue;
                    break;
                }
                if (_kbhit())
                    Game->New_Naprablenie(_getch());
                system("cls");
                if (_kbhit())
                    Game->New_Naprablenie(_getch());
                Game->DRAW_MAP();
                if (_kbhit())
                    Game->New_Naprablenie(_getch());
                bool status = Game->GoZmeika();
                if (_kbhit())
                    Game->New_Naprablenie(_getch());
                if (!status)
                    break;
                if (_kbhit())
                    Game->New_Naprablenie(_getch());
                std::cout << "\nОчки: " << Game->GetPoint();
                Sleep(100);
                if (_kbhit())
                    Game->New_Naprablenie(_getch());
            }
            if (!win)
            {
                system("cls");
                std::cout << "You Lose:(\nНик: " << Game->GetName() << " Очки: " << Game->GetPoint();

                Record r = { Game->GetPoint(),Game->GetName() };
                Game->AddRecord(r);
                if (_getch())
                    continue;
            }
        }
        else if (INT == 3)
        {          
            Game->PrintRecordList();
            if (_getch())
                continue;
        }
        else if (INT == 4)
        {
            delete Game;
            return 0;
        }

    }
}

