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

class Person 
{
    public:
        virtual void printInfo() = 0;
        virtual ~Person() {}
};

class Subject : public Person
{
    private:
        char name[100];
        int grades[6];
        int grades_count;
    public:
        void setName(char* newName) {
            strncpy(name, newName, sizeof(name) - 1);
            name[sizeof(name) - 1] = '\0';
        }

        void setGrade (int index, int grade) {
            if (index >= 0 && index <= 5) {
                grades[index] = grade;
                if (index >= grades_count) {
                    grades_count = index + 1;
                }
            }
        }

        void printInfo() override 
        {
            std::cout << "Subject: " << name << std::endl;
            std::cout << "Grades: ";
            for (int i = 0; i < grades_count; i++) {
                std::cout << grades[i] << " ";
            }
            std::cout << std::endl;
        }

        void setGradesCount(int count) {
            if (count >= 0 && count <= 5) {
                grades_count = count;
            }
        }

        const char* getName() 
        {
            return name;
        }

        int getGrade(int index) {
            if (index >= 0 && index <= grades_count) {
                return grades[index];
            }
            return -1;
        }

        int getGradesCount() {
            return grades_count;
        }

        // +
        Subject operator+(const Subject& other) {
            Subject result;
            int index = 0;

            for (int i = 0; i < grades_count; i++) {
                result.setGrade(index++, grades[i]);
            }

            for (int i = 0; i < other.grades_count; i++) {
                result.setGrade(index++, other.grades[i]);
            }

            result.setGradesCount(index);
            return result;
        }

        // -
        Subject operator-(const Subject& other) {
            Subject result;
            int index = 0;

            for (int i = 0; i < grades_count; i++) {
                bool isUnique = true;
                for (int j = 0; j < other.grades_count; j++) {
                    if (grades[i] == other.grades[j]) {
                        isUnique = false;
                        break;
                    }
                }
                if (isUnique) {
                    result.setGrade(index++, grades[i]);
                }
            }

            result.setGradesCount(index); 
            return result;
        }

        // префиксный
        Subject& operator++() {
            for (int i = 0; i < 6; i++) {
                grades[i]++;
            }
            return *this;
        }

        // постфиксный
        Subject operator++(int) {
            Subject temp = *this;
            ++(*this);
            return temp;
        }

        // ==
        bool operator==(const Subject& other) const {
            if (this->grades_count != other.grades_count) return false;
            for (int i = 0; i < this->grades_count; i++) {
                if (this->grades[i] != other.grades[i]) return false;
            }
            return true;
        }

        // =
        Subject& operator=(const Subject& other) {
            if (this != &other) {
                strncpy(this->name, other.name, sizeof(this->name) - 1);
                this->grades_count = other.grades_count;
                for (int i = 0; i < 5; i++) {
                    this->grades[i] = other.grades[i];
                }
            }
            return *this;
        }
};

class student_data
{
    private:
        char* name;
        char* surname;
        char* full_group;
        int exams_marks[3];
        Subject subjects[3];
        int grades_count;
    public:
        student_data()
        {
            name = new char[100];
            surname = new char[100];
            full_group = new char[100];
            grades_count = 0;
        };
        student_data(char *name_u, char *surname_u, char *full_group_u) : student_data()
        {
            // даем первоначальные значения
            strcpy(surname, surname_u);
            strcpy(name, name_u);
            strcpy(full_group, full_group_u);
        }
        student_data(char *name_u, char *surname_u, char *full_group_u, int *exams_marks_u) : student_data()
        {
            strcpy(surname, surname_u);
            strcpy(name, name_u);
            strcpy(full_group, full_group_u);

            for (int i = 0; i < 3; ++i) {
                exams_marks[i] = exams_marks_u[i];
            }
        }

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

        const char* getSurname() {
            return surname;
        }

        const char* getName() {
            return name;
        }

        const char* getFullGroup() {
            return full_group;
        }

        int getExamMark(int index) {
            if (index < 0 || index >= 3)
                return -1;
            return exams_marks[index];
        }
        
        int getSubjectsCount() {
            return grades_count;
        }

        const char* getSubjectName(int index) {
            if (index < 0 || index >= grades_count)
                return NULL;
            return subjects[index].getName();
        }

        int getSubjectGrade(int subjectIndex, int gradeIndex) {
            if (subjectIndex < 0 || subjectIndex >= grades_count)
                return -1;
            return subjects[subjectIndex].getGrade(gradeIndex);
        }

        void setSurname(char* newSurname) {
            surname = new char[strlen(newSurname) + 1];
            strcpy(surname, newSurname);
        }

        void setName(char* newName) {
            name = new char[strlen(newName) + 1];
            strcpy(name, newName);
        }

        void setFullGroup(char* newGroup) {
            full_group = new char[strlen(newGroup) + 1];
            strcpy(full_group, newGroup);
        }

        void setMark(int index, int mark) {
            if (index >= 0 && index <= 3) {
                exams_marks[index] = mark;
            }
        }

        void setSubjectName(int index, char* name) {
            subjects[index].setName(name);
        }

        void setSubjectGrade(int subjectIndex, int gradeIndex, int grade) {
            subjects[subjectIndex].setGrade(gradeIndex, grade);
        }

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
    while (!valid_group) {
        cout << "Группа (формат XXX-YY): ";
        cin.getline(temp_group, 100);
        if (is_valid_group(temp_group)) {
            valid_group = true;
        } else {
            cout << "Неправильный формат группы. Введите снова.\n";
        }
    }

    bool valid_marks = false;
    while (!valid_marks) {
        cout << "Оценки по трём экзаменам (от 1 до 5, через пробел): ";
        cin >> exams_marks[0] >> exams_marks[1] >> exams_marks[2];
        if ((exams_marks[0] >= 1 && exams_marks[0] <= 5) &&
            (exams_marks[1] >= 1 && exams_marks[1] <= 5) &&
            (exams_marks[2] >= 1 && exams_marks[2] <= 5)) {
            valid_marks = true;
        } else {
            cout << "Оценки должны быть целыми числами от 1 до 5. Попробуйте снова.\n";
        }
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

        data[i].setSurname(surname);
        data[i].setName(name);
        data[i].setFullGroup(group);

        for (int j = 0; j < 3; j++)
            data[i].exams_marks[j] = exams_marks[j];

        fscanf(file, "%d\n", &data[i].grades_count);
        for (int j = 0; j < data[i].grades_count; j++)
        {
            char subject_name[100];
            fgets(subject_name, 100, file);
            subject_name[strlen(subject_name) - 1] = '\0';
            data[i].subjects[j].setName(subject_name);

            int grades_count;
            fscanf(file, "%d\n", &grades_count);
            for (int k = 0; k < grades_count; k++)
            {
                int grade;
                fscanf(file, "%d ", &grade);
                data[i].subjects[j].setGrade(k, grade);
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
                data[i].getSurname(), data[i].getName(), data[i].getFullGroup(),
                data[i].getExamMark(0), data[i].getExamMark(1), data[i].getExamMark(2));

        fprintf(file, "%d\n", data[i].getSubjectsCount());
        for (int j = 0; j < data[i].getSubjectsCount(); j++)
        {
            fprintf(file, "%s\n%d\n", data[i].subjects[j].getName(), data[i].subjects[j].getGradesCount());
            for (int k = 0; k < data[i].subjects[j].getGradesCount(); k++)
            {
                fprintf(file, "%d ", data[i].subjects[j].getGrade(k));
            }
            fprintf(file, "\n");
        }
    }
    fclose(file);
}

void student_data::add_subject()
{
    if (grades_count >= 3) {
        cout << "У студента уже 3 предмета, нельзя добавить больше.\n";
        return;
    }

    char new_subject[100];
    cout << "Введите название предмета: ";
    cin.ignore();
    cin.getline(new_subject, 100);
    
    for (int i = 0; i < grades_count; i++) {
        if (strcmp(subjects[i].getName(), new_subject) == 0) {
            cout << "Предмет '" << new_subject << "' уже добавлен студенту.\n";
            return;
        }
    }

    subjects[grades_count].setName(new_subject);
    subjects[grades_count].setGradesCount(0);
    grades_count++;
    cout << subjects[grades_count - 1].getName() << endl;
}

// student_data* student
void student_data::print_subjects()
{
    if (grades_count == 0) {
        cout << "У студента нет добавленных предметов.\n";
        return;
    }

    cout << "Предметы студента:\n";
    for (int i = 0; i < grades_count; i++) {
        cout << subjects[i].getName() << " (Оценки: ";
        for (int j = 0; j < subjects[i].getGradesCount(); j++) {
            cout << subjects[i].getGrade(j);
            if (j < subjects[i].getGradesCount() - 1)
                cout << ", ";
        }
        cout << ")\n";
    }
}

void student_data::add_grade(int subject_index)
{
    if (subjects[subject_index].getGradesCount() >= 5) {
        cout << "Нельзя добавить больше оценок для этого предмета.\n";
        return;
    }

    int grade;
    cout << "Введите оценку (от 1 до 5): ";
    cin >> grade;

    if (grade < 1 || grade > 5) {
        cout << "Оценка должна быть от 1 до 5. Попробуйте снова.\n";
        return;
    }

    subjects[subject_index].setGrade(subjects[subject_index].getGradesCount(), grade);
    subjects[subject_index].setGradesCount(subjects[subject_index].getGradesCount() + 1);
}

void student_data::add_grade_to_subject()
{
    if (grades_count == 0) {
        cout << "У студента нет добавленных предметов.\n";
        return;
    }

    cout << "Введите номер предмета, куда хотите добавить оценку:\n";
    for (int i = 0; i < grades_count; i++) {
        cout << i << " - " << subjects[i].getName() << "\n";
    }

    int subject_index;
    cin >> subject_index;

    if (subject_index >= 0 && subject_index < grades_count) {
        add_grade(subject_index);
    } else {
        cout << "Неверный номер предмета.\n";
    }
}

int main()
{
    student_data data[100];
    int data_size = student_data::load(data);
    int changed, removed;
    while (true)
    {
        cout << "\n[1] - добавить студента\n"
                  "[2] - вывести отличников\n"
                  "[3] - удалить студента\n"
                  "[4] - редактировать студента\n"
                  "[5] - вывести всех студентов\n"
                  "[6] - добавить предмет студенту\n"
                  "[7] - показать предметы студента\n"
                  "[8] - добавить оценку предмету студента\n"
                  "[q] - сохранить и выйти\n";
        char choice = mygetch();
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
                if (removed < data_size && removed >= 0)
                    data_size = data[removed].remove_student(data_size);
                break;
            case '4':
                data[0].all_student_data(data, data_size);
                cout << "Номер студента для редактирования: ";
                cin >> changed;
                if (changed < data_size && changed >= 0)
                    data[changed].change_student();
                break;
            case '5':
                data[0].all_student_data(data, data_size);
                break;
            case '6':
                data[0].all_student_data(data, data_size);
                cout << "Номер студента, которому добавляем предмет: ";
                cin >> changed;
                if (changed < data_size && changed >= 0)
                    data[changed].add_subject();
                break;
            case '7':
                data[0].all_student_data(data, data_size);
                cout << "Номер студента: ";
                cin >> changed;
                if (changed < data_size && changed >= 0)
                    data[changed].print_subjects();
                break;
            case '8':
                data[0].all_student_data(data, data_size);
                cout << "Номер студента: ";
                cin >> changed;
                if (changed < data_size && changed >= 0)
                    data[changed].add_grade_to_subject();
                break;
            case 'q':
                student_data::save(data, data_size);
                return 0;
        }
    }
}