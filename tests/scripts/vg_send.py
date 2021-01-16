#!/usr/bin/python

import sys
import struct

# List of messages predefined.
cmd_single_shot = struct.pack("!BHBB", 0x1, 1, 13, 0)

def main():
    #print('hello')
    sys.stdout.write(cmd_single_shot)
    #print(cmd_single_shot, end="")

exit(main())
