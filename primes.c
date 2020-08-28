#include "types.h"
#include "user.h"
#define MAX_NUM 35

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

        while (1) // break 했을 때, 처음으로 돌리게 하기 위한 장치
        {
            close(current_pipe[1]);
            if (read(current_pipe[0], &prime, sizeof(prime)) == 0)
            {
                break;
            }
            printf(1, "prime %d\n", prime);

            while (read(current_pipe[0], &number, sizeof(number)) != 0)
            {
                if (number % prime != 0)
                {
                    if (no_child) // 자식 프로세스가 없다면 생성한다.
                    {
                        pipe(new_pipe);
                        if (fork() == 0) // child
                        {
                            current_pipe[0] = new_pipe[0]; // 자식의 파이프를 새로운 파이프와 교체한다.
                            current_pipe[1] = new_pipe[1]; // 자식의 파이프를 새로운 파이프와 교체한다.
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

                if (number == MAX_NUM) // MAX_NUM 과 같다면 모든 숫자를 다 쓴 것이기 때문에 쓰기 파이프를 닫는다.
                {
                    close(current_pipe[1]);
                }
            }
        }
        close(current_pipe[0]);

        if (no_child == 0)
        {
            wait();
        }
        exit();
    }
}

int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        primes();
    }
    else
    {
        printf(1, "Can't use parameter!!!\n");
    }
    exit();
}