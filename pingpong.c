#include "types.h"
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
        int pid = 0;

        // 읽기 부분
        close(pipe_from_parent[1]);
        read(pipe_from_parent[0], &pid, sizeof(pid));
        printf(1, "%d: received ping\n", pid);
        close(pipe_from_parent[0]);

        // 쓰기 부분
        pid = getpid();
        close(pipe_from_child[0]);
        write(pipe_from_child[1], &pid, sizeof(pid));
        close(pipe_from_child[1]);
    }
    else // parent
    {
        int pid = 0;

        // 쓰기 부분
        pid = getpid();
        close(pipe_from_parent[0]);
        write(pipe_from_parent[1], &pid, sizeof(pid));
        close(pipe_from_parent[1]);

        // 읽기 부분
        close(pipe_from_child[1]);
        read(pipe_from_child[0], &pid, sizeof(pid));
        printf(1, "%d: received pong\n", pid);
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