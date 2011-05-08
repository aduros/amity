#!/bin/sh
#
# Uploads the test app to the device

novacom run 'file:///bin/mkdir -p /tmp/amity-dev'

cd "../test"
for i in *; do
    novacom put file:///tmp/amity-dev/$i < $i;
done
