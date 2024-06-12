#include <stdio.h>  // Подключение библиотеки для ввода/вывода
#include <stdlib.h> // Подключение библиотеки для работы с памятью и другими системными функциями
#include <string.h> // Подключение библиотеки для работы со строками
#include <sys/stat.h> // Подключение библиотеки для работы с файловой системой
#include <stddef.h> // Подключение библиотеки для определения типов и макросов
#include "huffman.h" // Подключение заголовочного файла с определениями для алгоритма Хаффмана

// Функция проверки расширения файла
int checkExtension(const char *filename, const char *extension) {
    const char *dot = strrchr(filename, '.'); // Находим последнюю точку в имени файла
    return dot && !strcmp(dot, extension); // Возвращаем true, если расширение совпадает
}

// Функция проверки существования файла
int fileExists(const char *filename) {
    struct stat buffer; // Буфер для информации о файле
    return stat(filename, &buffer) == 0; // Возвращаем true, если файл существует
}

// Функция обработки аргументов командной строки
void process(char *argv[])
{
    // Если первый аргумент "-encodeFile", вызываем функцию кодирования
    if (strcmp(argv[1], "-encodeFile") == 0)
    {
        struct node *head = NULL; // Инициализируем голову списка узлов дерева Хаффмана
        encodeFile(&(head), argv[2]); // Кодируем файл
    }
        // Если первый аргумент "-decodeFile", вызываем функцию декодирования
    else if (strcmp(argv[1], "-decodeFile") == 0)
    {
        struct table *head = NULL; // Инициализируем голову списка таблицы кодирования
        decodeFile(&(head), argv[2]); // Декодируем файл
    }
}

// Функция создания нового узла дерева Хаффмана
struct node *getNode()
{
    struct node *node = (struct node *)malloc(sizeof(struct node)); // Выделяем память под узел
    if (node == NULL) // Проверяем успешность выделения памяти
    {
        printf("Error: Memory allocation failed\n"); // Выводим сообщение об ошибке
        exit(EXIT_FAILURE); // Завершаем программу с ошибкой
    }
    // Инициализируем поля узла
    node->next = NULL;
    node->frequency = 1;
    node->left = NULL;
    node->right = NULL;
    return node; // Возвращаем новый узел
}

// Функция создания нового узла таблицы кодирования
struct table *getTableNode()
{
    struct table *node = (struct table *)malloc(sizeof(struct table)); // Выделяем память под узел
    if (node == NULL) // Проверяем успешность выделения памяти
    {
        printf("Error: Memory allocation failed\n"); // Выводим сообщение об ошибке
        exit(EXIT_FAILURE); // Завершаем программу с ошибкой
    }
    node->next = NULL; // Инициализируем поле next
    return node; // Возвращаем новый узел
}

// Функция проверки наличия символа в списке
int isPresent(char c, struct node *head)
{
    if (head == NULL) // Если список пуст, возвращаем false
    {
        return 0;
    }
    else
    {
        // Проходим по списку
        while (head != NULL)
        {
            // Если находим символ, увеличиваем его частоту и возвращаем true
            if (head->character == c)
            {
                (head->frequency)++;
                return 1;
            }
            head = head->next; // Переходим к следующему узлу
        }
        return 0; // Если символ не найден, возвращаем false
    }
}

// Функция добавления символа в список
struct node *addList(char c, struct node *head)
{
    // Если символа нет в списке, создаем для него новый узел
    if (!isPresent(c, head))
    {
        struct node *temp = getNode(); // Создаем новый узел
        temp->character = c; // Записываем символ в узел

        // Если список пуст, новый узел становится головой списка
        if (head == NULL)
        {
            head = temp;
        }
        else
        {
            // Иначе вставляем новый узел в начало списка
            temp->next = head;
            head = temp;
        }
    }
    return head; // Возвращаем обновленный список
}

// Функция печати списка
void printList(struct node *head)
{
    if (head == NULL) // Если список пуст, выводим сообщение
    {
        printf("List is empty\n");
    }
    else
    {
        // Проходим по списку и печатаем символы и их частоты
        while (head != NULL)
        {
            printf("'%c':%d\n", head->character, head->frequency);
            head = head->next; // Переходим к следующему узлу
        }
    }
}

// Функция сортировки слиянием для списка узлов
void mergeSort(struct node **headRef) {
    struct node *head = *headRef; // Получаем голову списка
    struct node *a; // Подсписок a
    struct node *b; // Подсписок b

    // Базовый случай -- длина 0 или 1
    if ((head == NULL) || (head->next == NULL)) {
        return; // Возвращаемся, если список пуст или содержит один элемент
    }

    // Разделяем список на две подсписка
    split(head, &a, &b);

    // Рекурсивно сортируем обе подсписка
    mergeSort(&a);
    mergeSort(&b);

    // Сливаем подсписки вместе
    *headRef = sortedMerge(a, b);
}

// Функция слияния двух отсортированных подсписков
struct node *sortedMerge(struct node *a, struct node *b)
{
    struct node *result = NULL; // Результат слияния
    if (a == NULL) // Если подсписок a пуст, возвращаем b
    {
        return b;
    }
    else if (b == NULL) // Если подсписок b пуст, возвращаем a
    {
        return a;
    }

    // Сравниваем частоты узлов и строим результат
    if (a->frequency <= b->frequency)
    {
        result = a;
        result->next = sortedMerge(a->next, b);
    }
    else
    {
        result = b;
        result->next = sortedMerge(a, b->next);
    }
    return result; // Возвращаем результат слияния
}

// Функция разделения списка на две части
void split(struct node *source, struct node **frontRef, struct node **backRef) {
    // 'fast' и 'slow' указатели используются для нахождения середины списка
    struct node *fast = source->next; // 'fast' двигается в два раза быстрее
    struct node *slow = source; // 'slow' двигается в один раз быстрее

    // Пока 'fast' не достигнет конца списка, перемещаем 'slow' и 'fast'
    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }

    // 'slow' теперь указывает на узел перед серединой списка
    *frontRef = source; // Передняя часть начинается с головы исходного списка
    *backRef = slow->next; // Задняя часть начинается после 'slow'
    slow->next = NULL; // Разрываем список на две части
}

// Функция кодирования файла
void encodeFile(struct node **head, const char *filename)
{
    FILE *fp = fopen(filename, "r"); // Открываем файл для чтения
    struct node *cur = *head; // Текущий узел для добавления символов
    struct table *header = NULL; // Заголовок таблицы кодирования
    char ch; // Переменная для хранения текущего символа

    // Читаем символы из файла до конца файла
    while ((ch = fgetc(fp)) != EOF)
    {
        cur = addList(ch, cur); // Добавляем символ в список
    }
    fclose(fp); // Закрываем файл

    mergeSort(&cur); // Сортируем список символов
    createTree(&cur); // Создаем дерево Хаффмана
    createTable(&header, cur, 0b0, 0); // Создаем таблицу кодирования
    freeNodesList(&cur); // Освобождаем память списка символов
    fileEncoding(header, filename); // Кодируем файл
    freeHuffmanTable(&header); // Освобождаем память таблицы кодирования
    printf("Successfully encodeFiled\n"); // Выводим сообщение об успешном кодировании
    exit(EXIT_SUCCESS); // Завершаем программу успешно
}

// Функция освобождения памяти для списка узлов
void freeNodesList(struct node **head)
{
    // Проходим по всему списку
    while (*head != NULL)
    {
        struct node *temp = *head; // Временный указатель на текущий узел
        *head = (*head)->next; // Перемещаем голову списка на следующий узел
        temp->next = NULL; // Обнуляем указатель на следующий узел
        free(temp); // Освобождаем память текущего узла
    }
}

// Функция освобождения памяти для таблицы кодирования
void freeHuffmanTable(struct table **head)
{
    // Проходим по всей таблице
    while (*head != NULL)
    {
        struct table *temp = *head; // Временный указатель на текущий элемент таблицы
        *head = (*head)->next; // Перемещаем голову таблицы на следующий элемент
        temp->next = NULL; // Обнуляем указатель на следующий элемент
        free(temp); // Освобождаем память текущего элемента
    }
}


// Функция поиска узла в таблице по символу
struct table *findNode(struct table *head, char c)
{
    struct table *cur = head; // Начинаем поиск с головы таблицы
    // Проходим по таблице до её конца
    while (cur != NULL)
    {
        if (cur->character == c) // Если символ найден
        {
            return cur; // Возвращаем указатель на найденный узел
        }
        cur = cur->next; // Перемещаемся к следующему узлу
    }
    return NULL; // Если символ не найден, возвращаем NULL
}

// Функция кодирования файла с использованием таблицы Хаффмана
void fileEncoding(struct table *head, const char *filename)
{
    FILE *srcfp = fopen(filename, "r"); // Открываем исходный файл для чтения
    // Выделяем память под имя нового файла с расширением .huff
    char *hufffile = (char *)malloc(sizeof(char) * (strlen(filename) + 5));
    strcpy(hufffile, filename); // Копируем имя исходного файла
    strcat(hufffile, ".huff"); // Добавляем расширение .huff
    FILE *dstfp = fopen(hufffile, "wb+"); // Открываем новый файл для записи в двоичном режиме

    char ch; // Переменная для хранения текущего символа
    unsigned char byte = 0b0; // Байт для записи закодированного символа
    int overflow = 0; // Счётчик для отслеживания заполнения байта

    // Записываем заголовок таблицы в новый файл
    struct table *temp = head;
    while (temp != NULL)
    {
        fprintf(dstfp, "%c:%d:%d ", temp->character, temp->num, temp->size);
        temp = temp->next;
    }
    fprintf(dstfp, "##\n"); // Маркер конца заголовка

    // Кодируем содержимое исходного файла
    while ((ch = fgetc(srcfp)) != EOF) // Читаем символы до конца файла
    {
        struct table *cur = findNode(head, ch); // Ищем символ в таблице
        int size = cur->size; // Размер кода символа
        unsigned int value = cur->num; // Числовое значение кода символа

        // Записываем код символа в байт
        while (size != 0)
        {
            int add = value & (1 << (size - 1)); // Получаем бит кода символа
            if (add != 0)
            {
                byte = byte | (1 << (7 - overflow)); // Добавляем бит в байт
            }
            overflow++; // Увеличиваем счётчик заполнения байта
            if (overflow == 8) // Если байт заполнен
            {
                fwrite(&byte, sizeof(unsigned char), 1, dstfp); // Записываем байт в файл
                byte = 0b0; // Обнуляем байт
                overflow = 0; // Сбрасываем счётчик
            }
            size--; // Уменьшаем размер кода символа
        }
    }
    if (overflow != 0) // Если остались незаполненные биты в байте
    {
        fwrite(&byte, sizeof(unsigned char), 1, dstfp); // Записываем последний байт в файл
    }
    fprintf(dstfp, "###%d", overflow); // Записываем количество незаполненных бит в последнем байте
    fclose(srcfp); // Закрываем исходный файл
    fclose(dstfp); // Закрываем новый файл
}

// Функция создания таблицы кодирования Хаффмана
void createTable(struct table **header, struct node *cur, unsigned int number, int size)
{
    // Проверяем, не является ли текущий узел NULL
    if (cur != NULL)
    {
        // Рекурсивно обрабатываем левую ветвь дерева
        createTable(header, cur->left, number << 1, size + 1);
        // Рекурсивно обрабатываем правую ветвь дерева
        createTable(header, cur->right, (number << 1) + 1, size + 1);

        // Если текущий узел является листом (не имеет потомков)
        if (cur->left == NULL && cur->right == NULL)
        {
            // Создаем новый узел таблицы, если это первый элемент
            if (*header == NULL)
            {
                *header = getTableNode(); // Выделение памяти для нового узла таблицы
                (*header)->character = cur->character; // Сохраняем символ
                (*header)->num = number; // Сохраняем бинарный код символа
                (*header)->size = size; // Сохраняем длину бинарного кода
            }
            else
            {
                // Добавляем новый узел в начало списка, если таблица уже содержит элементы
                struct table *temp = getTableNode(); // Выделение памяти для нового узла таблицы
                temp->character = cur->character; // Сохраняем символ
                temp->num = number; // Сохраняем бинарный код символа
                temp->size = size; // Сохраняем длину бинарного кода
                temp->next = *header; // Связываем новый узел с предыдущим
                *header = temp; // Обновляем указатель на голову списка
            }
        }
    }
}

// Функция печати таблицы кодирования Хаффмана
void printTable(struct table *head)
{
    // Проходим по всем узлам таблицы
    while (head != NULL)
    {
        // Выводим информацию о каждом символе и его коде
        printf("Character = '%c', Code = ", head->character);
        // Выводим бинарный код символа
        for (int i = head->size - 1; i >= 0; i--)
        {
            printf("%d", (head->num >> i) & 1);
        }
        printf(", Code Length = %d\n", head->size);
        head = head->next; // Переходим к следующему узлу
    }
}


// Функция для печати дерева Хаффмана
void printTree(struct node *cur)
{
    // Проверяем, не является ли текущий узел NULL
    if (cur != NULL)
    {
        // Рекурсивно печатаем левое поддерево
        printTree(cur->left);
        // Рекурсивно печатаем правое поддерево
        printTree(cur->right);

        // Проверяем, является ли текущий узел листом (не имеет потомков)
        if (cur->left == NULL && cur->right == NULL)
        {
            // Печатаем символ и его частоту в дереве
            printf("%c:%d\n", cur->character, cur->frequency);
        }
    }
}


// Функция для добавления нового узла в дерево Хаффмана
// Объединяет два узла (node1 и node2) в новый узел, где
// частота нового узла равна сумме частот двух узлов
struct node *addNode(struct node *node1, struct node *node2)
{
    // Выделение памяти для нового узла
    struct node *node = getNode();

    // Установка частоты нового узла равной сумме частот двух дочерних узлов
    node->frequency = node1->frequency + node2->frequency;

    // Установка левого дочернего узла
    node->left = node1;

    // Установка правого дочернего узла
    node->right = node2;

    // Возвращаем новый узел, который теперь объединяет два поддерева
    return node;
}


// Функция для обновления отсортированного списка узлов
// Вставляет узел в список в соответствии с его частотой
void updateList(struct node **head, struct node *node)
{
    // Начинаем с головы списка
    struct node *cur = *head;

    // Если частота в голове списка больше или равна частоте нового узла,
    // вставляем новый узел перед головой
    if (cur->frequency >= node->frequency)
    {
        node->next = cur;
        *head = node;
        return;
    }

        // В противном случае ищем место для вставки нового узла
    else
    {
        // Используем два указателя: 'temp' для отслеживания предыдущего узла
        // и 'cur' для текущего узла
        struct node *temp = cur;
        cur = cur->next;

        // Проходим по списку до тех пор, пока не найдем подходящее место для вставки
        // или не достигнем конца списка
        while (cur != NULL)
        {
            // Если нашли место для вставки, делаем это и выходим из функции
            if (cur->frequency >= node->frequency)
            {
                node->next = cur;
                temp->next = node;
                return;
            }
            temp = cur;
            cur = cur->next;
        }

        // Если мы достигли конца списка, вставляем новый узел в конец
        temp->next = node;
    }
}


void createTree(struct node **head)
{
    struct node *cur1 = *head;
    struct node *temp1;
    struct node *temp2;
    struct node *temp3;
    if (cur1->next != NULL)
    {
        while (cur1 != NULL)
        {
            temp1 = cur1;
            temp2 = cur1->next;
            cur1 = temp2->next;
            temp1->next = NULL;
            temp2->next = NULL;
            temp3 = addNode(temp1, temp2);
            if (cur1 != NULL)
            {
                updateList(&cur1, temp3);
            }
        }
        *head = temp3;
    }
}

char findCharacter(struct table *head, unsigned int number, int size)
{
    struct table *cur = head;
    while (cur != NULL)
    {
        if ((cur->num == number) && (cur->size == size))
        {
            return cur->character;
        }
        cur = cur->next;
    }
    return '\0';
}

void decodeFile(struct table **head, const char *filename)
{
    FILE *fp = fopen(filename, "rb");
    char fch = fgetc(fp);
    char sch = fgetc(fp);
    unsigned int val;
    int number;
    while (!(fch == '#' && sch == '#'))
    {
        struct table *temp = getTableNode();
        temp->character = fch;
        sch = fgetc(fp);
        val = sch - '0';
        sch = fgetc(fp);
        while (sch != ':')
        {
            val = (val * 10) + (sch - '0');
            fch = sch;
            sch = fgetc(fp);
        }
        temp->num = val;
        sch = fgetc(fp);
        number = sch - '0';
        sch = fgetc(fp);
        while (sch != ' ')
        {
            number = (number * 10) + (sch - '0');
            sch = fgetc(fp);
        }
        temp->size = number;
        sch = fgetc(fp);
        fch = sch;
        sch = fgetc(fp);
        if (*head == NULL)
        {
            *head = temp;
        }
        else
        {
            temp->next = *head;
            *head = temp;
        }
    }
    sch = fgetc(fp);
    unsigned char c1, c2, c0, c3;
    c3 = fgetc(fp);
    c0 = fgetc(fp);
    c1 = fgetc(fp);
    c2 = fgetc(fp);
    unsigned int tempch = 0b0;
    char mych;
    int size = 0;
    int overflow = 8;
    char *hufffile = (char *)malloc(sizeof(char) * (strlen(filename) - 4));
    strncpy(hufffile, filename, (strlen(filename) - 5));
    hufffile[strlen(filename) - 5] = '\0';
    FILE *noice = fopen(hufffile, "w+");
    while (!(c0 == '#' && c1 == '#' && c2 == '#'))
    {
        while (overflow != 0)
        {
            if ((c3 & (1 << (overflow - 1))) == 0)
            {
                tempch = tempch << 1;
            }
            else
            {
                tempch = (tempch << 1) + 0b1;
            }
            size++;
            overflow--;
            if ((mych = findCharacter(*head, tempch, size)) != '\0')
            {
                fprintf(noice, "%c", mych);
                tempch = 0b0;
                size = 0;
            }
        }
        overflow = 8;
        c3 = c0;
        c0 = c1;
        c1 = c2;
        c2 = fgetc(fp);
    }
    overflow = 8;
    fseek(fp, -1, SEEK_END);
    char asdch = fgetc(fp);
    int gotch = (asdch - '0');
    if(gotch == 0)
    {
        gotch = 8;
    }
    fseek(fp, -5, SEEK_END);
    asdch = fgetc(fp);
    while(gotch != 0)
    {
        if ((asdch & (1 << (overflow - 1))) == 0)
        {
            tempch = tempch << 1;
        }
        else
        {
            tempch = (tempch << 1) + 0b1;
        }
        overflow--;
        gotch--;
        size++;
        if ((mych = findCharacter(*head, tempch, size)) != '\0')
        {
            fprintf(noice, "%c", mych);
            tempch = 0b0;
            size = 0;
        }
    }
    fclose(fp);
    fclose(noice);
    freeHuffmanTable(&(*head));
    printf("Successfully decodeFiled\n");
    exit(EXIT_SUCCESS);
}
