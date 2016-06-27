#!/bin/env python
"""Copyright (c) 2016, William, Bundy

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE."""

#
# git_post_rev.py
# usage :
#   python git_post_rev.py rev.txt branch
# 
#

import os
import sys
import time 

DEFAULT_FILE = "rev.txt"

def init_tracking_file(filename):
    if not os.path.exists(filename): 
        try:
            ff = open(filename, "w")
            ff.write("0\n")
            ff.write("0\n")
            ff.close()
            return filename
        except IOError as err:
            print("Could not initialize file", filename)
            print(err)
            return False
    else:
        # so the file exists... can we open it?
        if os.access(filename, os.R_OK):
            return filename
        else:
            return False


def main(tracking_file):
    if not tracking_file:
        return

    rev = 0
    last_t = 0
    try:
        ff = open(tracking_file, "r")
        lines = []
        for line in ff:
            lines.append(line)
        ff.close()
    except IOError as err:
        print("Could not open file for reading:", tracking_file)
        print(err)
    try:
        rev = int(lines[0])
    except ValueError:
        print("Could not read revision number from file")
        print("["+lines[0]+"]")
    try:
        last_t = float(lines[1])
    except ValueError:
        print("Could not read last time from file")
        print("["+lines[1]+"]")
    tdiff = time.time() - last_t
    print("Time since last commit:", tdiff)
    if tdiff < 10:
        print("Flood warning: wait a minute between commits!")
    print("Commit message: \"rituals rev {0}\"".format(rev))
    rev += 1
    
    try:
        ff = open("autogit.sh", "w")
        ff.write("git add --all\n")
        ff.write("git commit -m \"rituals build {0}\" \n".format(rev))
        ff.write("git push \n")
        ff.close()
    except IOError as err:
        print("Could not write to batch file streaming_auto_git.bat")
        print(err)
    os.system('"c:\\program files\\git\\usr\\bin\\bash.exe" -login autogit.sh')
    tt = time.time()
    try:
        ff = open(tracking_file, "w")
        ff.write(str(rev) + "\n")
        ff.write(str(tt) + "\n")
        ff.close()
    except IOError as err:
        print("Could not open file for writing:", tracking_file)
        print(err)

if __name__ == "__main__":
    tracking_file = None
    if len(sys.argv) < 2:
        tracking_file = init_tracking_file(DEFAULT_FILE)
    else:
        tracking_file = init_tracking_file(sys.argv[1])
    main(tracking_file)


