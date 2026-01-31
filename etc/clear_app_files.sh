#!/bin/sh

fatal() {
	echo "$1" > /dev/stderr
	exit 1
}

adb shell "run-as bleed.conic rm -rf /data/user/0/bleed.conic/files/*" || fatal failed
