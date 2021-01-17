#!/usr/bin/python

import sys
import struct
import time

usage = 'Usage: vg_send <cmd> <count>'

# TODO... Store these commands into dictionary where,
# key = cmd_str, value = cmd
# List of messages predefined.
cmd_single_shot = struct.pack("!BHBBBB", 0x1, 1, 13, 1, 0xff, 0xff)
cmd_spray_shot = struct.pack("!BHBBBB", 0x1, 1, 13, 2, 0xff, 0xff)
cmd_aimdown = struct.pack("!BHBBBB", 0x1, 1, 15, 1, 0xff, 0xff)
cmd_walk_forward = struct.pack("!BHBBBB", 0x1, 1, 16, 1, 0xff, 0xff)
cmd_jump = struct.pack("!BHBBBB", 0x1, 1, 20, 1, 0xff, 0xff)

# Get the appropriate pre-packed command object
def str2cmd(cmd_str):
    if cmd_str == 'cmd_single_shot':
        return cmd_single_shot
    if cmd_str == 'cmd_spray_shot':
        return cmd_spray_shot
    if cmd_str == 'cmd_aimdown':
        return cmd_aimdown
    if cmd_str == 'cmd_walk_forward':
        return cmd_walk_forward
    if cmd_str == 'cmd_jump':
        return cmd_jump
    else:
        return None

# Send a command @count times.
def send_cmd(cmd_str, count):
    cmd = str2cmd(cmd_str)
    while count > 0:
        sys.stdout.write(cmd)
        time.sleep(0.5)
        count -= 1

def main():
    if len(sys.argv) != 3:
        print(usage)
        return -1
    cmd_str = sys.argv[1]
    count = int(sys.argv[2])
    send_cmd(cmd_str, count)

exit(main())
