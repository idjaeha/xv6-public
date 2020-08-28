#include "types.h"
#include "user.h"
#define MAX_NUM 35

// 문제점
// 1. 파이프 라인이 잘 작동하고 있는지 사실 모르겠다.
// 2. read를 탈출 못한다.

void primes()
{
    int current_pipe[2];
    pipe(current_pipe);

    if (fork() != 0) // generate numbers
    {
        int index = 0;
        for (index = 2; index <= MAX_NUM; index++)
        {
            close(current_pipe[0]);
            write(current_pipe[1], &index, sizeof(index));
        }
        close(current_pipe[1]);
        wait();
    }

    else // child
    {
        int prime = 0;
        int number = 0;
        int no_child = 1;
        int new_pipe[2];

        while (number < MAX_NUM && prime < MAX_NUM)
        {
            // close(current_pipe[1]);
            if (read(current_pipe[0], &prime, sizeof(prime)) == 0)
            {
                break;
            }
            printf(1, "prime %d\n", prime);

            while (number < MAX_NUM)
            {
                if (read(current_pipe[0], &number, sizeof(number)) == 0)
                {
                    break;
                }

                if (number % prime != 0)
                {
                    if (no_child) // 자식 프로세스가 없다면 생성한다.
                    {
                        pipe(new_pipe);
                        if (fork() == 0) // child
                        {
                            current_pipe[0] = new_pipe[0]; // 자식의 읽기 파이프를 새로운 파이프와 연결한다.
                            break;
                        }
                        else // parent
                        {
                            current_pipe[1] = new_pipe[1]; // 부모의 쓰기 파이프를 새로운 파이프와 연결한다.
                            no_child = 0;
                        }
                    }
                    write(current_pipe[1], &number, sizeof(number)); // 자식 프로세스에게 넘긴다.
                }
            }
        }

        // 좀비를 안만들기위한 임시방편 대신 꺼지질 않는다 ㅠ
        if (no_child == 0)
        {
            wait();
        }
        close(current_pipe[0]);
        close(current_pipe[1]);
        exit();
    }
}

int main(int argc, char *argv[])
{
    primes();
    exit();
}