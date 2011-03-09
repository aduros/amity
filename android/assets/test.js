$amity.log("Hello world!");

var mouseX = 0;
var mouseY = 0;
var bogartTexture = $amity.createTexture("/sdcard/data/bogart.png");
$amity.log("Loaded texture with size: " + bogartTexture.width + " by " + bogartTexture.height);

var tentacleTexture = $amity.createTexture("/sdcard/data/tentacle.png");
var manTexture = $amity.createTexture("/sdcard/data/man.png");

$amity.log("JS sees the screen is " + $amity.canvas.WIDTH + " by " + $amity.canvas.HEIGHT);

$amity.onMouseMove = function (event) {
    mouseX = event.x;
    mouseY = event.y;
};

var elapsed = 0;
$amity.onEnterFrame = function (dt) {
    elapsed += dt;

    for (var ii = 0; ii < 5; ++ii) {
        var n = 0.1 * elapsed + 1000*ii;
        var x = n % $amity.canvas.WIDTH;
        var y = n % $amity.canvas.HEIGHT;

        //$amity.canvas.alpha = (Math.sin(elapsed/1000)+1)/2;
        $amity.canvas.save();
        $amity.canvas.translate(x, y);
        var scale = 0.25*Math.sin(0.1*n) + 1;
        $amity.canvas.scale(scale, scale);
        $amity.canvas.drawTexture(bogartTexture, 0, 0);
        $amity.canvas.restore();
    }

    $amity.canvas.save();
    $amity.canvas.translate(mouseX, mouseY);
    $amity.canvas.rotate(180*Math.sin(0.001*elapsed) + 180);
    $amity.canvas.drawTexture(manTexture, -0.5*manTexture.width, -0.5*manTexture.height);
    $amity.canvas.restore();
};
