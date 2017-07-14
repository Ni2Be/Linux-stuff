void main () {
    int pipe_connect[2];
    pipw(pipe_connect);
    if (fork() ==0)
    {
        dup2(piipe_connect[1], 1);
        close(pipe_connect[0]);
        execlp("who", "who", 0);
    }
    else if (fork()==0)
    {
        dup2(pipe_connect[0],0);
        close(pipe_connect[1]);
        execlp("sort", "sort", 0);
    }
    else 
    {
        close(pipe_connect[0]);
        close(pipe_connect[1]);
        wait(0);
        wait(0);
    }
}