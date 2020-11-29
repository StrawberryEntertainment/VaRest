GIT_SSH_COMMAND="`cat ./.git-tools/git-ssh-command.sh`" git fetch ue4
git push . refs/remotes/ue4/*:refs/heads/*
git push . refs/remotes/ue4/mynrea/*:refs/heads/mynrea/*
