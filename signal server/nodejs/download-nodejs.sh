#!/bin/sh

rm deb -rf
mkdir deb
cd deb

apt-rdepends nodejs |
	grep "^\w" |

	while read line
	do
		apt-get download $line
	done

ls |
	grep "perl" |
	xargs rm
cd ..