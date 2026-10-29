#include "bigint.h"
#include <stdio.h>
#include <string.h>

void bigint_init(bigint_t *number){
    memset(number->limbs, 0, sizeof(number->limbs));

    number->length = 0;
}
/*
функция обнуление числа
т е в С локальные переменные не обнуляються автоматически, 
перед любыми математическими операциями нужно явно обнулить что-либо

memset(number->limbs, 0, sizeof(number->limbs));
- это зануление всего массива limbs в ктором храняться 32-х битные куски нашего огромного числа

number->limbs               это массив из 128 элементов по 32 бита каждый
sizeof(number->limbs)       вычисляет полный размер этого массива в байтах
memset()                    заполняет всю память нулями (0x00)

number->length = 0 -> обнуляет длину числа
length - показвает сколько слов реально используеться

если просто обнулить память то программа не поймет что число равно нулю
для нее это может быть число размером в 4096 бит (128 слов), где все старшие цифры - это нули
установив length = 0, мы как бы подразумевает - "это число ноль и оно занимает 0 ячеек памяти"

в итоге когда мы пишем 
    bigint_t my_number;
    bigint_init(&my_number);

    в памяти создаеться структура в которой все 128 ячеек (limbs) заполненны нулями (0x00000000)
    length = 0;

    и число готово к тому чтобы в него чтото записали 

    а вот без этой функции программа пыталась бы сложить новы bigint 
    с мусорными числами из оперативной памяти и результат был бы непредстазуемым и скорее всего программа упала бы с ошибкой
*/

void bigint_from_int(bigint_t *number, uint64_t value){

}

void bigint_copy(bigint_t *dest, const bigint_t *src){

}

void bigint_print_hex(const bigint_t *number){

}

int bigint_cmp(const bigint_t *number_A, const bigint_t *number_b){

}

int bigint_is_zero(const bigint_t *number){
    return number->length == 0;
}
/*
функия проверяет равно ли число нулю, она возвращает 1 если истинна и 0 если ложь

return number->length == 0;
    number->length - это поле структуры которое зранит сколько 32-х битный слов реально занимает число

    идет проверка - если length равен нулю то выраение number->length == 0 дает истинну (1) 
    если больше нуля то выражение дает 0 - ложь

вообще эта функция нужна 
    для читаемости кода 
        (ну это понятно почему)
    для безопасности 
        мы можем хранить длинуу в length сейчас
        а после, мы можем хранить ее иначе или добавить отрицательные числа и если мы поменяем внутренне устройство структуры, нам придеться искать по всему коду проверки number->length == 0 и менять их
        а если мы используем вот такую функцию то нужно будет изменить ее одну
    производительность
        можно было бы реализовать чуть чуть по другому и проверять все 128 ячеек памяти
            for(int i = 0; i < 128; i++){
                if(number->limbs[i] != 0){
                    return 0;
                }
            }
        и это заняло бы 128 операций что оказваеться медленно
        а проверка length == 0 - это одна мнгновенная операция

    так же в самом RSA эта функция нужно для защиты от деления на ноль, циклы в алгоритмах евклида и возведение в степень по модулю

*/

void bigint_add(bigint_t *result, const bigint_t *number_A, const bigint_t *number_B){

}

void bigint_sub(bigint_t *result, const bigint_t *number_A, const bigint_t *number_B){

}

void bigint_mul(bigint_t *result, const bigint_t *number_A, const bigint_t *number_B){

}

void bigint_shl(bigint_t *number, int bits){

}

void bigint_shr(bigint_t *number, int bits){

}

void bigint_print_hex(const bigint_t *number){
    if(number->length == 0){
        printf("0\n");

        return;
    }

    printf("0x%X", number->limbs[number->length - 1]);

    for(int i = number->length - 2; i >= 0; i--){
        printf("%08X", number->limbs[i]);
    }

    printf("\n");
}
/*
эта функция выводит огромное число bigint на экран в шестнадцатеричном формате 
и она нужна для отладки чтобы можно было увидеть глазами и проверить что математические операции типа сложения или умножения работают правильно

if(number->length == 0)                                 если число равно нулю то просто надо напечатать 0
printf("0x%X", number->limbs[number->length - 1]);      печать самого старшего слова (берем самое последнее слово и печатаем его без ведущих нулей)
for(...)                                                печать остальных слов (в обратном порядке)
*/