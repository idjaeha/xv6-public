#include "types.h"
#include "user.h"
#define MAX_NUM 35

// 문제점
// 1. 파이프 라인이 잘 작동하고 있는지 사실 모르겠다.
// 2. read를 탈출 못한다.

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
        int check_array[MAX_NUM + 1] = {0};

        check_array[0] = 1;
        check_array[1] = 1;
        int prime = 0;
        int number = 0;
        int index = 0;
        int no_child = 1;

        while (number < MAX_NUM)
        {
            read(_pipe[0], &prime, sizeof(prime));
            printf(1, "prime %d\n", prime);
            for (index = prime; index <= MAX_NUM; index += prime)
            {
                check_array[index] = 1;
            }

            while (number < MAX_NUM && read(_pipe[0], &number, sizeof(number)) != 0)
            {
                if (number % prime != 0 &&
                    check_array[number] == 0)
                {
                    if (no_child) // 자식 프로세스가 없다면 생성한다.
                    {
                        if (fork() == 0)
                        {
                            break;
                        }
                        else
                        {
                            no_child = 0;
                        }
                    }
                    write(_pipe[1], &number, sizeof(number));
                }
            }
        }

        // 좀비를 안만들기위한 임시방편 대신 꺼지질 않는다 ㅠ
        if (no_child == 0)
        {
            wait();
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