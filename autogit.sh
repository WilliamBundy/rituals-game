./find_ssh_agent.sh
ssh-find-agent -a
if [ -z "$SSH_AUTH_SOCK" ]
then
   eval $(ssh_agent) > /dev/null
   ssh-add -l >/dev/null || alias ssh='ssh-add -l >/dev/null || ssh-add && unalias ssh; ssh'
fi
git add --all
git commit -m "testing stuff..." 
git push 
