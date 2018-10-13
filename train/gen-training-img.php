#!/usr/bin/php
<?php
	set_time_limit(0);
	$files1 = scandir("./fonts/");
	array_splice($files1,0, 1);
	array_splice($files1,0, 1);
	$file1totalcount = count($files1);
	$file1count = 0;
	$letters = "A B C D E F G H I J K L M N O P Q R S T U V W X Y Z a b c d e f g h i j k l m n o p q r s t u v w x y z";
	//$letters = "a b c d e f g h i j k l m n o p q r s t u v w x y z";
	$array = explode(" ",$letters);
	$number = 200;

	foreach($array as $letter) {
	 for($i=0;$i<$number;$i++) {
	  $im = imagecreatetruecolor(28, 28);
	  $black = imagecolorallocate($im, 0, 0, 0);
	  $white = imagecolorallocate($im, 255, 255, 255);
	  // Create some colors
	  #$white = imagecolorallocate($im, 255, 255, 255);
	  #$grey = imagecolorallocate($im, 128, 128, 128);
	  #$black = imagecolorallocate($im, 0, 0, 0);
	  imagefilledrectangle($im, 0, 0, 27, 27, $white);

	  $font = './fonts/'.$files1[rand(0,$file1totalcount-1)];	
	  imagettftext($im, 22, 0, rand(0,3),rand(16,29), $black, $font, $letter);
	  if(!is_dir($letter))
	   mkdir($letter);
	   imagegif($im,"./".$letter."/$i.gif");
	   imagedestroy($im);
	  }
	}
?>
