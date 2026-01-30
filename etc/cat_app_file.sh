#!/bin/sh

fatal() {
	echo "$1" > /dev/stderr
	exit 1
}

[ -z "$1" ] && fatal "usage: $0 [path]"

adb shell "run-as bleed.conic ls -Al /data/user/0/bleed.conic/files/conic/"
adb shell "run-as bleed.conic cat /data/user/0/bleed.conic/files/conic/$1"
