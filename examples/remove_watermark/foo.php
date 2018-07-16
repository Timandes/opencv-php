<?php
/**
 * Remove Watermark
 *
 * @author Timandes White <timands@gmail.com>
 */

$path = 'foo.jpg';
$mat = opencv_imread($path);

$width = opencv_mat_rows($mat);
$height = opencv_mat_cols($mat);

$maskh = (int)($height * 0.075);
$masky = (int)($height * 0.44);

$mask = opencv_mat_zeros($height, $width);

opencv_rectangle($mask, [0, $masky], [$width, $masky+$maskh], array(255, 255, 255, 0), -1, 8);

$finalMat = opencv_inpaint($mat, $mask, 3);

$dest = 'bar.jpg';
$r = opencv_imwrite($dest, $finalMat);
if ($r)
    echo "OK" . PHP_EOL;
else
    echo "Error" . PHP_EOL;
