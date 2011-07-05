__amity.log("Hello world!");

var mouseX = 0;
var mouseY = 0;
var bogartTexture = __amity.loadTexture("bogart.png");
__amity.log("Loaded texture with size: " + bogartTexture.width + " by " + bogartTexture.height);

var tentacleTexture = __amity.loadTexture("tentacle64.png");
var manTexture = __amity.loadTexture("man.png");

__amity.log("JS sees the screen is " + __amity.canvas.WIDTH + " by " + __amity.canvas.HEIGHT);

__amity.events.onMouseMove = function (event) {
    mouseX = event.x;
    mouseY = event.y;
    //__amity.log("delta: " + event.deltaX + ", " + event.deltaY);
};

__amity.events.onMouseDown = function (event) {
    __amity.log("Called onMouseDown");
    __amity.log("Button: " + event.button);
};

__amity.events.onMouseUp = function (event) {
    __amity.log("Called onMouseUp");
    __amity.log("Button: " + event.button);
};

var elapsed = 0;
__amity.events.onEnterFrame = function (dt) {
    elapsed += dt;

    // Pattern texture dimensions must be a power of two
    __amity.canvas.drawPattern(tentacleTexture, 0, 0, __amity.canvas.WIDTH, __amity.canvas.HEIGHT);

    for (var ii = 0; ii < 5; ++ii) {
        var n = 0.1 * elapsed + 1000*ii;
        var x = n % __amity.canvas.WIDTH;
        var y = n % __amity.canvas.HEIGHT;

        //__amity.canvas.alpha = (Math.sin(elapsed/1000)+1)/2;
        __amity.canvas.save();
        __amity.canvas.multiplyAlpha(0.5);
        __amity.canvas.translate(x, y);
        var scale = 0.25*Math.sin(0.1*n) + 1;
        __amity.canvas.scale(scale, scale);
        if (ii == 0) {
            __amity.canvas.fillRect(0xffcc00, 0, 0, 100, 100);
        } else {
            __amity.canvas.drawImage(bogartTexture, 0, 0);
        }
        __amity.canvas.restore();
    }

    __amity.canvas.save();
    __amity.canvas.translate(mouseX, mouseY);
    __amity.canvas.rotate(180*Math.sin(0.001*elapsed) + 180);
    __amity.canvas.drawImage(manTexture, -0.5*manTexture.width, -0.5*manTexture.height);
    __amity.canvas.restore();
};

var http = __amity.net.createHttpRequest("http://google.com");
http.setHeader("X-custom-header", "foo");
http.onComplete = function (data) {
    __amity.log("Http request complete:");
    __amity.log(data);
    __amity.log("Length: " + data.length);
};
http.onError = function (message) {
    __amity.log("Http error: " + message);
};
http.send(null);

__amity.log("Holy quine, Batman! " + __amity.loadFile("app.js"));
