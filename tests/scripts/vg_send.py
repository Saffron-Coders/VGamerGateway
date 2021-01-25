#!/usr/bin/python

import sys
import struct
import time
import socket

usage = 'Usage: vg_send <cmd> <count> [<delay_time>]'

VGAMER_GATEWAY_IP = "192.168.1.4"
VGAMER_GATEWAY_PORT = 15000

# Initialize socket object globally
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# TODO... Store these commands into dictionary where,
# key = cmd_str, value = cmd
# List of messages predefined.
cmd_stop_shoot = struct.pack("!BH BB BB", 0x1, 1, 13, 0, 0xff, 0xff)
cmd_single_shot = struct.pack("!BH BB BB", 0x1, 1, 13, 1, 0xff, 0xff)
cmd_spray_shot = struct.pack("!BH BB BB", 0x1, 1, 13, 2, 0xff, 0xff)
cmd_aimdown = struct.pack("!BH BB BB", 0x1, 1, 15, 1, 0xff, 0xff)
cmd_walk_forward = struct.pack("!BH BB BB", 0x1, 1, 16, 2, 0xff, 0xff)
cmd_walk_stop = struct.pack("!BH BB BB", 0x1, 1, 16, 0, 0xff, 0xff)
cmd_jump = struct.pack("!BH BB BB", 0x1, 1, 20, 1, 0xff, 0xff)

# Move (+10, +10) from current mouse position.
cmd_mouse_right = struct.pack("!BH hh BB", 0x2, 1, 10, 10, 0xff, 0xff)
cmd_mouse_left = struct.pack("!BH hh BB", 0x2, 1, -10, -10, 0xff, 0xff)

DEFAULT_DELAY = 0.5

# Get the appropriate pre-packed command object
def str2cmd(cmd_str):
    if cmd_str == 'cmd_stop_shoot':
        return cmd_stop_shoot
    if cmd_str == 'cmd_single_shot':
        return cmd_single_shot
    if cmd_str == 'cmd_spray_shot':
        return cmd_spray_shot
    if cmd_str == 'cmd_aimdown':
        return cmd_aimdown
    if cmd_str == 'cmd_walk_forward':
        return cmd_walk_forward
    if cmd_str == 'cmd_walk_stop':
        return cmd_walk_stop
    if cmd_str == 'cmd_jump':
        return cmd_jump
    if cmd_str == 'cmd_mouse_right':
        return cmd_mouse_right
    if cmd_str == 'cmd_mouse_left':
        return cmd_mouse_left
    else:
        return None

def udp_send(data):
    sock.sendto(data, (VGAMER_GATEWAY_IP, VGAMER_GATEWAY_PORT))

# Send a command @count times.
def send_cmd(cmd_str, count, delay):
    cmd = str2cmd(cmd_str)
    if cmd is None:
        return False
    while count > 0:
        sys.stdout.write(cmd)
        udp_send(cmd)
        time.sleep(delay)
        count -= 1
    return True

def main():
    if len(sys.argv) < 3:
        print(usage)
        return -1
    cmd_str = sys.argv[1]
    count = int(sys.argv[2])

    # Optional delay time
    delay = DEFAULT_DELAY
    if len(sys.argv) == 4:
        delay = float(sys.argv[3])

    send_cmd(cmd_str, count, delay)

exit(main())
