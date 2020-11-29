GIT_SSH_COMMAND="`cat ./.git-tools/git-ssh-command.sh`" git fetch github
git push . refs/remotes/github/*:refs/heads/*
git push . refs/remotes/github/mynrea/*:refs/heads/mynrea/*
