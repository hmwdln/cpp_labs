#include <iostream>
#include <cstdlib>
#include <termios.h>
#include <unistd.h>
#include <cstring>
#include <cstdio>
#include <regex>

using namespace std;

int mygetch()
{
    struct termios oldt, newt;
    int c;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    c = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return c;
}

class Subject
{
    public:
        char name[100];
        int grades[5];
        int grades_count = 0;

};

class student_data
{
    public:
        char* name;
        char* surname;
        char* full_group;
        int exams_marks[3];
        Subject subjects[3];
        int subjects_count = 0;

        int add_student(int data_size);
        void print_student_data();
        void all_student_data(student_data data[], int data_size);
        int remove_student(int data_size);
        void excellent_marks(student_data data[], int data_size);
        void change_student();
        static int load(student_data data[]);
        static void save(student_data data[], int size);
        void add_subject();
        void print_subjects();
        void add_grade(int subject_index);
        void add_grade_to_subject();
};

bool is_valid_group(const string &group)
{
    regex pattern("\\d{3}-\\d{2}");
    return regex_match(group, pattern);
}

int student_data::add_student(int data_size)
{   
    cin.ignore();
    char* temp_group = new char[100];
    char* temp_name = new char[100];
    char* temp_surname = new char[100];

    cout << "Фамилия: ";
    cin.getline(temp_surname, 100);
    cout << "Имя: ";
    cin.getline(temp_name, 100);

    bool valid_group = false;
    while (!valid_group) 
    {
        cout << "Группа (формат XXX-YY): ";
        cin.getline(temp_group, 100);
        if (is_valid_group(temp_group))
            valid_group = true;
        else 
            cout << "Неправильный формат группы. Введите снова.\n";
    }

    bool valid_marks = false;
    while (!valid_marks) 
    {
        cout << "Оценки по трём экзаменам (от 1 до 5, через пробел): ";
        cin >> exams_marks[0] >> exams_marks[1] >> exams_marks[2];
        if ((exams_marks[0] >= 1 && exams_marks[0] <= 5) &&
            (exams_marks[1] >= 1 && exams_marks[1] <= 5) &&
            (exams_marks[2] >= 1 && exams_marks[2] <= 5)) 
                valid_marks = true;
        else
            cout << "Оценки должны быть целыми числами от 1 до 5. Попробуйте снова.\n";
    }

    surname = temp_surname;
    name = temp_name;
    full_group = temp_group;
    data_size++;
    return data_size;
}

float average_value(int a[3])
{
    return (a[0] + a[1] + a[2]) / 3.0;
}

void student_data::print_student_data()
{
    printf(" | %-25s | %-17s | %-6s | %1d %1d %1d  | %1.2lf",
           surname, name,
           full_group, exams_marks[0],
           exams_marks[1], exams_marks[2],
           average_value(exams_marks));
}

void student_data::all_student_data(student_data data[], int data_size)
{
    cout << "ID | Фамилия\t\t| Имя\t\t| Группа | Оценки | Ср. балл\n";
    for (int i = 0; i < data_size; i++)
    {
        cout << i << " ";
        data[i].print_student_data();
        cout << "\n";
    }
}

int student_data::remove_student(int data_size)
{
    delete[] name;
    delete[] surname;
    delete[] full_group;
    return data_size - 1;
}

void student_data::excellent_marks(student_data data[], int data_size)
{
    cout << "ID | Фамилия\t\t| Имя\t\t| Группа | Оценки | Ср. балл\n";
    for (int i = 0; i < data_size; i++)
    {
        if (average_value(data[i].exams_marks) >= 4.50)
        {
            cout << i << " ";
            data[i].print_student_data();
            cout << "\n";
        }
    }
}

void student_data::change_student()
{
    remove_student(0);
    add_student(0);
}

int student_data::load(student_data data[])
{
    FILE* file = fopen("student_data.txt", "r");
    int size;
    fscanf(file, "%d\n", &size);

    for (int i = 0; i < size; i++)
    {
        char* group = new char[100];
        char* name = new char[100];
        char* surname = new char[100];
        
        int exams_marks[3];
        fgets(surname, 100, file);
        fgets(name, 100, file);
        fgets(group, 100, file);
        fscanf(file, "%d %d %d\n", &exams_marks[0], &exams_marks[1], &exams_marks[2]);

        surname[strlen(surname) - 1] = '\0';
        name[strlen(name) - 1] = '\0';
        group[strlen(group) - 1] = '\0';

        data[i].surname = surname;
        data[i].name = name;
        data[i].full_group = group;
        for (int j = 0; j < 3; j++)
            data[i].exams_marks[j] = exams_marks[j];

        fscanf(file, "%d\n", &data[i].subjects_count);
        for (int j = 0; j < data[i].subjects_count; j++)
        {
            fgets(data[i].subjects[j].name, 100, file);
            data[i].subjects[j].name[strlen(data[i].subjects[j].name) - 1] = '\0';
            fscanf(file, "%d\n", &data[i].subjects[j].grades_count);
            for (int k = 0; k < data[i].subjects[j].grades_count; k++)
            {
                fscanf(file, "%d ", &data[i].subjects[j].grades[k]);
            }
        }
    }

    fclose(file);
    return size;
}

void student_data::save(student_data data[], int size)
{
    FILE* file = fopen("student_data.txt", "w");
    fprintf(file, "%d\n", size);
    for (int i = 0; i < size; i++)
    {
        fprintf(file, "%s\n%s\n%s\n%d %d %d\n",
                data[i].surname, data[i].name, data[i].full_group,
                data[i].exams_marks[0], data[i].exams_marks[1], data[i].exams_marks[2]);

        fprintf(file, "%d\n", data[i].subjects_count);
        for (int j = 0; j < data[i].subjects_count; j++)
        {
            fprintf(file, "%s\n%d\n", data[i].subjects[j].name, data[i].subjects[j].grades_count);
            for (int k = 0; k < data[i].subjects[j].grades_count; k++)
            {
                fprintf(file, "%d ", data[i].subjects[j].grades[k]);
            }
            fprintf(file, "\n");
        }
    }
    fclose(file);
}

void student_data::add_subject()
{
    if (subjects_count >= 3) 
    {
        cout << "У студента уже 3 предмета, нельзя добавить больше.\n";
        return;
    }

    char new_subject[100];
    cout << "Введите название предмета: ";
    cin.ignore();
    cin.getline(new_subject, 100);
    
    for (int i = 0; i < subjects_count; i++) 
    {
        if (strcmp(subjects[i].name, new_subject) == 0) 
        {
            cout << "Предмет '" << new_subject << "' уже добавлен студенту.\n";
            return;
        }
    }

    strcpy(subjects[subjects_count].name, new_subject);
    subjects_count++;
}

// student_data* student
void student_data::print_subjects()
{
    if (subjects_count == 0) 
    {
        cout << "У студента нет добавленных предметов.\n";
        return;
    }

    cout << "Предметы студента:\n";
    for (int i = 0; i < subjects_count; i++) 
    {
        cout << subjects[i].name << " (Оценки: ";
        for (int j = 0; j < subjects[i].grades_count; j++) 
        {
            cout << subjects[i].grades[j];
            if (j < subjects[i].grades_count - 1)
                cout << ", ";
        }
        cout << ")\n";
    }
}

void student_data::add_grade(int subject_index)
{
    if (subjects[subject_index].grades_count >= 5) 
    {
        cout << "Нельзя добавить больше оценок для этого предмета.\n";
        return;
    }

    int grade;
    cout << "Введите оценку (от 1 до 5): ";
    cin >> grade;

    if (grade < 1 || grade > 5) 
    {
        cout << "Оценка должна быть от 1 до 5. Попробуйте снова.\n";
        return;
    }

    subjects[subject_index].grades[subjects[subject_index].grades_count] = grade;
    subjects[subject_index].grades_count++;
}

void student_data::add_grade_to_subject()
{
    if (subjects_count == 0) 
    {
        cout << "У студента нет добавленных предметов.\n";
        return;
    }

    cout << "Введите номер предмета, куда хотите добавить оценку:\n";
    for (int i = 0; i < subjects_count; i++) 
        cout << i << " - " << subjects[i].name << "\n";

    int subject_index;
    cin >> subject_index;

    if (subject_index >= 0 && subject_index < subjects_count) 
        add_grade(subject_index);
    else
        cout << "Неверный номер предмета.\n";
}

int main() 
{
    student_data data[100];
    int data_size = student_data::load(data);
    int removed, changed;
    char choice;

    do {
        cout << "[1] - добавить студента\n";
        cout << "[2] - вывести отличников\n";
        cout << "[3] - удалить студента\n";
        cout << "[4] - редактировать студента\n";
        cout << "[5] - вывести всех студентов\n";
        cout << "[6] - добавить предмет студенту\n";
        cout << "[7] - показать предметы студента\n";
        cout << "[8] - добавить оценку предмету студента\n";
        cout << "[q] - сохранить и выйти\n";
        cout << "Ваш выбор: ";
        cin >> choice;

        switch (choice) 
        {
            case '1':
                data_size = data[data_size].add_student(data_size);
                break;

            case '2':
                data[0].excellent_marks(data, data_size);
                break;

            case '3':
                data[0].all_student_data(data, data_size);
                cout << "Номер студента для удаления: ";
                cin >> removed;
                if (removed < data_size && removed >= 0) {
                    data_size = data[removed].remove_student(data_size);
                } else {
                    cout << "Некорректный номер студента!\n";
                }
                break;

            case '4':
                data[0].all_student_data(data, data_size);
                cout << "Номер студента для редактирования: ";
                cin >> changed;
                if (changed < data_size && changed >= 0) {
                    data[changed].change_student();
                } else {
                    cout << "Некорректный номер студента!\n";
                }
                break;

            case '5':
                data[0].all_student_data(data, data_size);
                break;

            case '6':
                data[0].all_student_data(data, data_size);
                cout << "Номер студента, которому добавляем предмет: ";
                cin >> changed;
                if (changed < data_size && changed >= 0) {
                    data[changed].add_subject();
                } else {
                    cout << "Некорректный номер студента!\n";
                }
                break;

            case '7':
                data[0].all_student_data(data, data_size);
                cout << "Номер студента: ";
                cin >> changed;
                if (changed < data_size && changed >= 0) {
                    data[changed].print_subjects();
                } else {
                    cout << "Некорректный номер студента!\n";
                }
                break;

            case '8':
                data[0].all_student_data(data, data_size);
                cout << "Номер студента: ";
                cin >> changed;
                if (changed < data_size && changed >= 0) {
                    data[changed].add_grade_to_subject();
                } else {
                    cout << "Некорректный номер студента!\n";
                }
                break;

            case 'q':
                data[0].save(data, data_size);
                return 0;

            default:
                cout << "Некорректный выбор, попробуйте снова.\n";
                break;
        }
    } while (choice != 'q');

    return 0;
}

