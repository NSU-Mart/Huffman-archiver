#include <stdio.h>  // Подключение стандартной библиотеки ввода-вывода
#include <stdlib.h> // Подключение библиотеки стандартных утилит (например, для динамического выделения памяти)
#include <string.h> // Подключение библиотеки для работы со строками
#include <sys/stat.h> // Подключение библиотеки для работы с файловой системой
#include <stddef.h> // Подключение библиотеки для определения некоторых стандартных типов и макросов
#include "huffman.h" // Подключение заголовочного файла для алгоритма Хаффмана

int main() {
    char operation[10]; // Массив символов для хранения выбранной операции (encodeFile/decodeFile)
    char extractFileName[260]; // Массив символов для хранения имени файла
    struct HuffmanNode *head = NULL; // Указатель на начало списка узлов дерева Хаффмана
    struct HuffmanTableItem *HuffmanTableItemHead = NULL; // Указатель на начало списка для таблицы кодирования Хаффмана

    // Запрос у пользователя выбора операции и имени файла
    printf("Select operation (encodeFile/decodeFile): ");
    scanf("%s", operation);

    printf("Enter file name: ");
    scanf("%s", extractFileName);

    // Обработка операции кодирования
    if (strcmp(operation, "encodeFile") == 0) {
        if (fileExists(extractFileName)) { // Проверка существования файла
            encodeFile(&head, extractFileName); // Вызов функции кодирования
        } else {
            printf("File does not exist.\n"); // Сообщение об отсутствии файла
        }
    }
        // Обработка операции декодирования
    else if (strcmp(operation, "decodeFile") == 0) {
        if (fileExists(extractFileName) && checkExtension(extractFileName, ".huff")) { // Проверка существования файла и корректности расширения
            decodeFile(&HuffmanTableItemHead, extractFileName); // Вызов функции декодирования
        } else {
            printf("The file does not exist or has an incorrect extension.\n"); // Сообщение об ошибке
        }
    }
        // Обработка некорректного ввода операции
    else {
        printf("Invalid operation.\n");
    }

    // Освобождение памяти, если это необходимо
    freeNodesList(&head); // Освобождение памяти, занятой списком узлов
    freeHuffmanTable(&HuffmanTableItemHead); // Освобождение памяти, занятой таблицей кодирования

    return 0; // Возврат 0 указывает на успешное завершение программы
}
