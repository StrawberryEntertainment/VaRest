GIT_SSH_COMMAND="`cat ./.gittools/git-ssh-command-github.sh`" git push github
GIT_SSH_COMMAND="`cat ./.gittools/git-ssh-command-github.sh`" git push github refs/heads/*:refs/heads/*
GIT_SSH_COMMAND="`cat ./.gittools/git-ssh-command-github.sh`" git push github refs/remotes/ue4/*:refs/heads/*
GIT_SSH_COMMAND="`cat ./.gittools/git-ssh-command-github.sh`" git push github refs/remotes/github/*:refs/heads/*

