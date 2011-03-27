__amity.log("Hello world!");

var mouseX = 0;
var mouseY = 0;
var bogartTexture = __amity.createTexture("bogart.png");
__amity.log("Loaded texture with size: " + bogartTexture.width + " by " + bogartTexture.height);

var tentacleTexture = __amity.createTexture("tentacle.png");
var manTexture = __amity.createTexture("man.png");

__amity.log("JS sees the screen is " + __amity.canvas.WIDTH + " by " + __amity.canvas.HEIGHT);

__amity.onMouseMove = function (event) {
    mouseX = event.x;
    mouseY = event.y;
};

var elapsed = 0;
__amity.onEnterFrame = function (dt) {
    elapsed += dt;

    for (var ii = 0; ii < 5; ++ii) {
        var n = 0.1 * elapsed + 1000*ii;
        var x = n % __amity.canvas.WIDTH;
        var y = n % __amity.canvas.HEIGHT;

        //__amity.canvas.alpha = (Math.sin(elapsed/1000)+1)/2;
        __amity.canvas.save();
        __amity.canvas.translate(x, y);
        var scale = 0.25*Math.sin(0.1*n) + 1;
        __amity.canvas.scale(scale, scale);
        __amity.canvas.drawTexture(bogartTexture, 0, 0);
        __amity.canvas.restore();
    }

    __amity.canvas.save();
    __amity.canvas.translate(mouseX, mouseY);
    __amity.canvas.rotate(180*Math.sin(0.001*elapsed) + 180);
    __amity.canvas.drawTexture(manTexture, -0.5*manTexture.width, -0.5*manTexture.height);
    __amity.canvas.restore();
};
