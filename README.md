Amity
=====

Amity is a minimal C++ game engine, with an embedded Javascript VM (Mozilla
Spidermonkey) that drives all game logic.

It runs on Android, webOS and (soon) iOS. The intention is to be able to write
a single game in Javascript that runs fast on all these platforms. Because
Amity Javascript games don't run in a web browser, they can more easily be
hardware accelerated and receive full access to the device. Also, games don't
have to deal with the device-specific browser bugs or incomplete
implementations (which are very common at the time of this writing).

While you can write a game in pure Javascript on Amity, the recommended route
is [Flambe](http://github.com/aduros/flambe), a higher level haXe game engine
that allows you to write games that run on Amity, as well as Flash and HTML5!

Amity is in early development. If you need tons of features, come back later.

Building
--------

See the articles in /doc for setting up a development environment for each
platform.
