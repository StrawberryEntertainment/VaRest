GIT_SSH_COMMAND="`cat ./.gittools/git-ssh-command-origin.sh`" git push origin
GIT_SSH_COMMAND="`cat ./.gittools/git-ssh-command-origin.sh`" git push origin refs/heads/*:refs/heads/va-rest/*
GIT_SSH_COMMAND="`cat ./.gittools/git-ssh-command-origin.sh`" git push origin refs/remotes/ue4/*:refs/heads/va-rest/*
GIT_SSH_COMMAND="`cat ./.gittools/git-ssh-command-origin.sh`" git push origin refs/remotes/github/*:refs/heads/va-rest/*
