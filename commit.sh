#!/bin/sh
git add .
git commit -m "my commit"
git push https://github.com/stefanniculae2208/DELILA_TQDC.git main

expect "Username for 'https://github.com': "
send -- "stefanniculae2208"

expect "Password for 'https://stefanniculae2208@github.com': "
send -- "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"
