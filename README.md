# tinker
homebrew dhcp server

tinker (https://en.wikipedia.org/wiki/Tinker) is a homebrew attempt at putting together a compiled dhcp server in C with the ultimate goal of running as my production DHCP service on my home network (running on my ASUS DSL-AC68U)

Stay tuned.

![](tinkercap.GIF?raw=true "Title")

## tinker in action:
```
# ./tinker 
Starting tinker dhcp ip server now...
Printing IP Table
IP Node 0x1c24030 -- Used: 0 -- IP: 0.0.0.0
IP Node 0x1c24050 -- Used: 0 -- IP: 192.168.2.150
IP Node 0x1c24070 -- Used: 0 -- IP: 192.168.2.151
IP Node 0x1c24090 -- Used: 0 -- IP: 192.168.2.152
IP Node 0x1c240b0 -- Used: 0 -- IP: 192.168.2.153
IP Node 0x1c240d0 -- Used: 0 -- IP: 192.168.2.154
IP Node 0x1c240f0 -- Used: 0 -- IP: 192.168.2.155
IP Node 0x1c24110 -- Used: 0 -- IP: 192.168.2.156
IP Node 0x1c24130 -- Used: 0 -- IP: 192.168.2.157
IP Node 0x1c24150 -- Used: 0 -- IP: 192.168.2.158
IP Node 0x1c24170 -- Used: 0 -- IP: 192.168.2.159
IP Node 0x1c24190 -- Used: 0 -- IP: 192.168.2.160
IP Node 0x1c241b0 -- Used: 0 -- IP: 192.168.2.161
IP Node 0x1c241d0 -- Used: 0 -- IP: 192.168.2.162
IP Node 0x1c241f0 -- Used: 0 -- IP: 192.168.2.163
IP Node 0x1c24210 -- Used: 0 -- IP: 192.168.2.164
IP Node 0x1c24230 -- Used: 0 -- IP: 192.168.2.165
Waiting for data...
Received packet from 0.0.0.0:68
Recieved DHCP Hardware Type of ETHERNET
Recieved DHCP Message type of REQUEST
Client MAC Address: 08:00:27:de:eb:f8
Transaction ID: d1317831
Seconds Elapsed: 0059
Brand New Transaction
Starting Parser
First byte is 01
First option is is 35
i is 240
DHCP OPTION: DHCP MESSAGE TYPE
DHCP OPTION LENGTH: 1
dhcp_discover mode
i is 243
DHCP OPTION: HOSTNAME
DHCP OPTION LENGTH: 6
ADVERTISED HOSTNAME: slave1
i is 251
DHCP OPTION: UNKNOWN, NUMBER: 37
DHCP OPTION LENGTH: 13
Getting Ready to Send DHCP OFFER
Printing Transaction Table:
Node: Transaction ID: 00000000 Current Count: 0 Current State: 0 Current Address: 0x1c24010 Next Address: 0x1c24250
Node: Transaction ID: d1317831 Current Count: 1 Current State: 1 Current Address: 0x1c24250 Next Address: (nil)
Waiting for data...
Received packet from 0.0.0.0:68
Recieved DHCP Hardware Type of ETHERNET
Recieved DHCP Message type of REQUEST
Client MAC Address: 08:00:27:de:eb:f8
Transaction ID: 26c4e13d
Seconds Elapsed: 0000
Brand New Transaction
Starting Parser
First byte is 01
First option is is 35
i is 240
DHCP OPTION: DHCP MESSAGE TYPE
DHCP OPTION LENGTH: 1
dhcp_discover mode
i is 243
DHCP OPTION: HOSTNAME
DHCP OPTION LENGTH: 6
ADVERTISED HOSTNAME: slave1
i is 251
DHCP OPTION: UNKNOWN, NUMBER: 37
DHCP OPTION LENGTH: 13
Getting Ready to Send DHCP OFFER
Printing Transaction Table:
Node: Transaction ID: 00000000 Current Count: 0 Current State: 0 Current Address: 0x1c24010 Next Address: 0x1c24250
Node: Transaction ID: d1317831 Current Count: 1 Current State: 1 Current Address: 0x1c24250 Next Address: 0x1c24270
Node: Transaction ID: 26c4e13d Current Count: 1 Current State: 1 Current Address: 0x1c24270 Next Address: (nil)
Waiting for data...
```
