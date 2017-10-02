#include <stdio.h>
#include <time.h>

/*
 * Пример работы с функциями time, localtime_r, mktime
 */

int main()
{
    // получаем текущее время в формате time_t
    time_t cur = time(NULL);
    printf("%ld %zu\n", cur, sizeof(cur));

    // конвертируем время из формата time_t в формат struct tm
    struct tm ttm;
    localtime_r(&cur, &ttm);

    // выводим время
    printf("%04d-%02d-%02d %02d:%02d:%02d %d\n",
        ttm.tm_year + 1900, ttm.tm_mon + 1, ttm.tm_mday,
        ttm.tm_hour, ttm.tm_min, ttm.tm_sec, ttm.tm_isdst);

    ttm.tm_mday += 59;
    ttm.tm_isdst = -1; // если не знаем статус зимнего/летнего времени, устанавливаем флаг в -1
    // как правило, неизвестно, будет ли действовать летнее время, поэтому перед вызовом
    // mktime необходимо не забыть присвоить tm_isdst в -1
    // mktime конвертирует время в формат time_t и нормализует поля в структуре, переданной на вход
    time_t next = mktime(&ttm);
    printf("%ld\n", next);
    printf("%04d-%02d-%02d %02d:%02d:%02d %d\n",
        ttm.tm_year + 1900, ttm.tm_mon + 1, ttm.tm_mday,
        ttm.tm_hour, ttm.tm_min, ttm.tm_sec, ttm.tm_isdst);
}
