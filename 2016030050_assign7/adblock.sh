#!/bin/bash
# You are NOT allowed to change the files' names!
domainNames="domainNames.txt"
IPAddresses="IPAddresses.txt"
adblockRules="adblockRules"

function adBlock() {
    if [ "$EUID" -ne 0 ];then
        printf "Please run as root.\n"aa
        exit 1
    fi
    if [ "$1" = "-domains"  ]; then
        #MY CODE-----------------
        while read -r line
        do
        	nslookup $line |awk ' /Address/ {print $2} '|awk 'NR==2 {print}'
        done < $domainNames > $IPAddresses
        
        while read -r line
        do
        	iptables -A INPUT -s $line -j REJECT
        done < $IPAddresses
        #MY CODE-----------------
        true
    elif [ "$1" = "-ips"  ]; then
        #MY CODE-----------------
        while read -r line
        do
        	iptables -A INPUT -s $line -j REJECT
        done < $IPAddresses
        #MY CODE-----------------
        true
        
    elif [ "$1" = "-save"  ]; then
        #MY CODE-----------------
        iptables-save > $adblockRules
        #MY CODE-----------------
        true
        
    elif [ "$1" = "-load"  ]; then
        #MY CODE-----------------
        iptables-restore < $adblockRules
        #MY CODE-----------------
        true

        
    elif [ "$1" = "-reset"  ]; then
        #MY CODE-----------------

        #SET ACCEPT POLICIES
        iptables -P INPUT ACCEPT
        iptables -P OUTPUT ACCEPT
        iptables -P FORWARD ACCEPT
        #DELETE ALL EXISTING RULES
        iptables -F

        #MY CODE-----------------
        true

        
    elif [ "$1" = "-list"  ]; then
        #MY CODE-----------------
        iptables -L
        #MY CODE-----------------
        true
        
    elif [ "$1" = "-help"  ]; then
        printf "This script is responsible for creating a simple adblock mechanism. It rejects connections from specific domain names or IP addresses using iptables.\n\n"
        printf "Usage: $0  [OPTION]\n\n"
        printf "Options:\n\n"
        printf "  -domains\t  Configure adblock rules based on the domain names of '$domainNames' file.\n"
        printf "  -ips\t\t  Configure adblock rules based on the IP addresses of '$IPAddresses' file.\n"
        printf "  -save\t\t  Save rules to '$adblockRules' file.\n"
        printf "  -load\t\t  Load rules from '$adblockRules' file.\n"
        printf "  -list\t\t  List current rules.\n"
        printf "  -reset\t  Reset rules to default settings (i.e. accept all).\n"
        printf "  -help\t\t  Display this help and exit.\n"
        exit 0
    else
        printf "Wrong argument. Exiting...\n"
        exit 1
    fi
}

adBlock $1
exit 0
