#include <stdio.h>
#include <time.h>

int main()
{
    int year, mon, day, hour, min, sec;

    while (scanf("%d%d%d%d%d%d", &year, &mon, &day, &hour, &min, &sec) == 6) {
        struct tm lt;

        lt.tm_year = year - 1900;
        lt.tm_mon = mon - 1;
        lt.tm_mday = day;
        lt.tm_hour = hour;
        lt.tm_min = min;
        lt.tm_sec = sec;
        lt.tm_isdst = -1;

        time_t res = mktime(&lt);
        printf("%ld %d-%02d-%02d %02d:%02d:%02d %d %d %d\n",
                (long) res,
                    lt.tm_year + 1900, lt.tm_mon + 1, lt.tm_mday,
                    lt.tm_hour, lt.tm_min, lt.tm_sec,
                    lt.tm_yday, lt.tm_wday, lt.tm_isdst);

    }
}
