#include "types.h"
#include "stat.h"
#include "user.h"

// pingpong에 관한 궁금증
// 1. 좀비 뜨는 문제 어떻게 해결?
// 2. 핑퐁을 실행하면 할수록 계속 늘어나는 pid가 늘어나는 이유?

// xv6에 관한 궁금증
// 버퍼에 화살표와 같은 기호 들어가지 못하게 하는 법
// printf는 한글자씩 출력되는 듯?

// 알아둬야할 상식
// pipe[0] : 읽기용
// pipe[1] : 쓰기용

char buf[512];

void pingpong()
{
    int pipe_from_parent[2];
    int pipe_from_child[2];

    pipe(pipe_from_parent);
    pipe(pipe_from_child);
    if (fork() == 0) // child
    {
        char message[] = "0";
        int pid = getpid();

        // 읽기 부분
        close(pipe_from_parent[1]);
        read(pipe_from_parent[0], buf, sizeof(buf));
        printf(1, "%d: received ping\n", buf[0]);
        close(pipe_from_parent[0]);

        // 쓰기 부분
        close(pipe_from_child[0]);
        memset(message, pid, sizeof(char));
        write(pipe_from_child[1], message, sizeof(message));
        close(pipe_from_child[1]);
    }
    else // parent
    {
        char message[] = "0";
        int pid = getpid();

        // 쓰기 부분
        close(pipe_from_parent[0]);
        memset(message, pid, sizeof(char));
        write(pipe_from_parent[1], message, sizeof(message));
        close(pipe_from_parent[1]);

        // 읽기 부분
        close(pipe_from_child[1]);
        read(pipe_from_child[0], buf, sizeof(buf));
        printf(1, "%d: received pong\n", buf[0]);
        close(pipe_from_child[0]);

        wait(); // 1. wait으로 해결하는 것이 현명?
    }
}

int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        pingpong();
    }
    else
    {
        printf(1, "Can't use parameters\n");
    }
    exit();
}