$amity.log("Hello world!");

var mouseX = 0;
var mouseY = 0;

$amity.onMouseMove = function (event) {
    mouseX = event.x;
    mouseY = event.y;
};

var elapsed = 0;
$amity.onEnterFrame = function (dt) {
    elapsed += dt;

    for (var ii = 0; ii < 5; ++ii) {
        var n = 0.1 * elapsed + 1000*ii;
        var x = n % 320;
        var y = n % 480;

        //$amity.canvas.alpha = (Math.sin(elapsed/1000)+1)/2;
        $amity.canvas.drawTestImage(x, y);
    }

    $amity.canvas.drawTestImage(mouseX-32, mouseY-32);
};
