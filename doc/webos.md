Requirements
------------

* Palm PDK (tested with 2.1) installed. Linux users can [use this](https://github.com/kayahr/linux-webos-pdk) to generate a working PDK.
* A physical device (tested on a Pre 2), PDK apps aren't supported in the emulator.

Building
--------

1.  Download the SDL and Mozilla submodules:

        git submodule init
        git submodule update

2.  Copy external/build.cfg.in to external/build.cfg and edit to point to your PDK:

        PALM_PDK=/opt/PalmPDK

    (Note the lack of spaces around the equal signs. This file is actually a Bash script)

3.  Build dependencies:

        cd external
        ./build-deps.sh

4.  Build Amity:

        cd webos
        waf configure --pdk-path=/path/to/PalmPDK    # If omitted, --pdk-path is /opt/PalmPDK
        waf build

5.  Deploy and run:

        ./data.sh    # Pushes the test app to the staging area on the device
        waf push

You can watch log output by running `tail -f /var/log/messages | grep amity` on the device.
