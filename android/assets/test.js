$amity.log("Hello world!");

var elapsed = 0;
$amity.onEnterFrame = function (dt) {
    elapsed += dt;

    $amity.canvas.drawTestImage(elapsed*0.1, elapsed*0.12);

    for (var ii = 0; ii < 5; ++ii) {
        $amity.canvas.save();
        $amity.canvas.rotate(Math.random()*80);
        $amity.canvas.drawTestImage(Math.random()*260, 0);
        $amity.canvas.restore();
    }
};
