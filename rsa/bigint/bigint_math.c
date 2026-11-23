#include "bigint_math.h"

void bigint_mod(bigint_t *result, const bigint_t *A, const bigint_t *B)
{
    if (bigint_is_zero(B))
    {
        bigint_init(result);

        return;
    }

    bigint_t rem;
    bigint_init(&rem);

    int bit_len_A = A->length * 32;

    while (bit_len_A > 0 && !bigint_get_bit(A, bit_len_A - 1))
    {
        bit_len_A--;
    }

    for (int i = bit_len_A - 1; i >= 0; i--)
    {
        bigint_shl(&rem, 1); // rem = rem << 1

        if (bigint_get_bit(A, i))
        {
            rem.limbs[0] |= 1;

            if (rem.length == 0)
            {
                rem.length = 1;
            }
        }

        if (bigint_cmp(&rem, B) >= 0)
        {
            bigint_sub(&rem, &rem, B);
        }
    }

    bigint_copy(result, &rem);
}
/*
остаток от деления (A (mod B)) - (теория Драгунова)

функция вычисляет сволько останеться если разделить огромное число А на число В

использует побитовое деление (сдвиг и вычитаение)
т к писать деление в столбик для массив не хочу

если В равно нулю то программа просто обнуляет результат

создаеться переменная rem (reminder - остаток) и онуляеться

вычисляеться bit_len_A 
    отсекает ведущие нули чтобы найти самый старший значащий бит в числа А
    нужно чтоы не гонять цикл по пустым ячейкам памяти

    это происходит от самого старшего бита А вниз к младшему (ну типо слево направо как при чтении)
    bigint_shl(&rem, 1)
        сдвигает текущий остаток влево на 1 (умножает на 2) чтото типо дописываем нолик в конце
    Если текущий бит числа A равен 1, мы делаем 
        rem.limbs[0] |= 1 -> прибавляем эту единицу к остатку)
    if (bigint_cmp(&rem, B) >= 0) -> если накопленный остаток стал больше или равен делителю B вычитаем B из остатка

в итоге в rem лежит остаток от деления который копируеться в result
*/

void bigint_mod_exp(bigint_t *result, const bigint_t *base, const bigint_t *exp, const bigint_t *mod)
{
    bigint_t res, base_mod, temp_mul;

    bigint_init(&res);
    bigint_from_int(&res, 1); // res = 1
    bigint_init(&base_mod);
    bigint_mod(&base_mod, base, mod);

    int bit_len_exp = exp->length * 32;

    while (bit_len_exp > 0 && !bigint_get_bit(exp, bit_len_exp - 1))
    {
        bit_len_exp--;
    }

    for (int i = 0; i < bit_len_exp; i++)
    {
        if (bigint_get_bit(exp, i))
        {
            bigint_mul(&temp_mul, &res, &base_mod);
            bigint_mod(&res, &temp_mul, mod);
        }

        bigint_mul(&temp_mul, &base_mod, &base_mod);
        bigint_mod(&base_mod, &temp_mul, mod);
    }
    
    bigint_copy(result, &res);
}
/*
напрямую возводить огромное число в огромную степерь нельзя поэтому будет использоваться алгоритм Square-and-Multiply (подсказал ДикСик)

как она работает
    инициализация
        res = 1 (сюда будем накапливать результат)

        base_mod = base mod mod - сразу берем базу по модулю чтобы числа были поменьше

    поиск старшего бита степени
        так же как и в первой функции вычисляеться bit_len_exp чтобы знать сколько бит в показателе стапени exp среди которых есть еденицы

    главный цикл (от младшего бита к старшему)
        программа идет по битам показателя стапени (справа налево)
            если текущий бит стпени равен 1
                res = (res * bade_mod) mod mod
                умножаем текущий результат на базу и берем по модулю

            возведение бащы в квадрат (выполняеться на каждом шаге)
                base_mod = (base_mod* base_mod) mod mod
                готовим базу для следующего бита степени
    результат
        когда цикл проходит все биты то в res лежит ответ (base ^ exp) mod mod который копируеться в result

вместо того чтобы перемножать base само на себя exp раз (что заняло бы миллионы лет для RSA) алгоритм 
делает всего несколько умножений 
количество операций равно количеству битов в показатели степени 
именно поэтому RSA вообще работает так быстро
*/