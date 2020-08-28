#include "types.h"
#include "user.h"
#define MAX_NUM 3

// 파이프라인 구현에 대해서 모르겠다!
// 왜 탈출을 못할까요... ㅠㅠ
// 소수는 왜 제대로 안찍힐까요...

void primes()
{
    int _pipe[2];
    pipe(_pipe);

    if (fork() != 0) // generate numbers
    {
        int index = 0;
        close(_pipe[0]);
        for (index = 2; index <= MAX_NUM; index++)
        {
            write(_pipe[1], &index, sizeof(index));
        }
        close(_pipe[1]);
        wait();
    }

    else // child
    {
        int prime = 0;
        int number = 0;

        while (prime < MAX_NUM)
        {
            // 소수 받음
            read(_pipe[0], &prime, sizeof(prime));
            printf(1, "prime %d\n", prime);

            while (number < MAX_NUM && read(_pipe[0], &number, sizeof(number)) != 0)
            {
                if (fork() == 0)
                {
                    break;
                }
                else
                {
                    write(_pipe[1], &number, sizeof(number));
                }

                if (number % prime != 0) // 소수일시 보낸다.
                {
                    if (fork() == 0)
                    {
                        break;
                    }
                    else
                    {
                        write(_pipe[1], &number, sizeof(number));
                    }
                }
            }
        }
        close(_pipe[0]);
        close(_pipe[1]);
        exit();
    }
}

int main(int argc, char *argv[])
{
    primes();
    exit();
}