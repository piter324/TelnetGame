#!/bin/bash
	for i in `seq 1 $1`;
        do
                python3 telnet_script.py -u $i -p $i -inf &
        done    
