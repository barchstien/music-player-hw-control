#script for systemd
cp mp-hw-control.service /etc/systemd/system
#enable at boot time
systemctl enable mp-hw-control
#start
systemctl start mp-hw-control
