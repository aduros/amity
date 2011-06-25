#!/bin/sh
#
# Fetches some external dependencies from Mercurial

MOZILLA_TAG=FIREFOX_5_0_RELEASE

if [ ! -d "mozilla-release" ]; then
    hg clone http://hg.mozilla.org/releases/mozilla-release
fi
cd mozilla-release
hg pull
hg update $MOZILLA_TAG
