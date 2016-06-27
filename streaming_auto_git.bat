REM @echo off

REM c:\sshagent.cmd

echo %SSH_AGENT_PID%

git add --all
git commit -m "testing stuff..." 
git push 
