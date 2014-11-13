#!/usr/bin/env php
<?php
define('DS', DIRECTORY_SEPARATOR);
$time = time();


$build = 'build'.DS;
mkdir($build);

$need_reconfigure = false;
$fn = 'config.m4';
if (file_exists($fn) and (!file_exists($build.$fn) or (filemtime($fn) > filemtime($build.$fn)))) {
  copy($fn, $build.$fn);
  $need_reconfigure = true;
}
$fn = 'config.w32';
if (file_exists($fn) and (!file_exists($build.$fn) or (filemtime($fn) > filemtime($build.$fn)))) {
  copy($fn, $build.$fn);
  $need_reconfigure = true;
}

foreach (glob('*.{c,h}',GLOB_BRACE) as $filepath) {
  copy($filepath, $build.basename($filepath));
}
foreach (glob($build.'*.xml') as $fp) unlink($fp);
foreach (glob($build.'*.tgz') as $fp) unlink($fp);

$name = 'Kiwi';
$ver = '';

$xml_cont = file_get_contents($name.'.xml');

chdir($build);


#$command = 'phpize'; echo $command."\n"; passthru($command);
#$command = 'CFLAGS=-m32 CPPFLAGS=-m32 CCASFLAGS=-m32 ./configure --host=i386-linux --enable-kiwi'; echo $command."\n"; passthru($command);
#$command = 'make clean all'; echo $command."\n"; passthru($command);
#copy('modules/'.$name.'.so', $name.'_linux_32.so');
#
#
#$command = 'phpize'; echo $command."\n"; passthru($command);
#$command = 'CFLAGS=-m64 CPPFLAGS=-m64 CCASFLAGS=-m64 ./configure --host=x86_64-linux --enable-kiwi'; echo $command."\n"; passthru($command);
#$command = 'make clean all'; echo $command."\n"; passthru($command);
#copy('modules/'.$name.'.so', $name.'_linux_64.so');
#
#
#$command = 'phpize'; echo $command."\n"; passthru($command);
#$command = 'CFLAGS=-m32 CPPFLAGS=-m32 CCASFLAGS=-m32 ./configure --host=i386-apple-darwin10 --enable-kiwi'; echo $command."\n"; passthru($command);
#$command = 'make clean all'; echo $command."\n"; passthru($command);
#copy('modules/'.$name.'.so', $name.'_darwin10_32.so');
#
#
#$command = 'phpize'; echo $command."\n"; passthru($command);
#$command = 'CFLAGS=-m64 CPPFLAGS=-m64 CCASFLAGS=-m64 ./configure --host=x86_64-apple-darwin10 --enable-kiwi'; echo $command."\n"; passthru($command);
#$command = 'make clean all'; echo $command."\n"; passthru($command);
#copy('modules/'.$name.'.so', $name.'_darwin10_64.so');

$command = 'phpize'; echo $command."\n"; passthru($command);
$command = './configure --enable-kiwi'; echo $command."\n"; passthru($command);
$command = 'make clean all'; echo $command."\n"; passthru($command);
copy('modules/'.$name.'.so', $name.'.so');



$xml_cont = str_replace('<date></date>', '<date>'.date('Y-m-d',$time).'</date>', $xml_cont);
$xml_cont = str_replace('<time></time>', '<time>'.date('H:i:s',$time).'</time>', $xml_cont);

$matches = array();
preg_match('/<release>([^<]*)<\/release>/', $xml_cont, $matches);
$ver = $matches[1];

$files = array();
$files[] = array(
  'name' => $name.'_linux_32.so',
  'role' => 'ext',
);
$files[] = array(
  'name' => $name.'_linux_64.so',
  'role' => 'ext',
);
$files[] = array(
  'name' => $name.'_darwin10_32.so',
  'role' => 'ext',
);
$files[] = array(
  'name' => $name.'_darwin10_64.so',
  'role' => 'ext',
);

$files_str = '';
$files_str .= "\n".'<dir name="/">'."\n";
foreach ($files as $file) {
  $files_str .= '<file role="'.$file['role'].'" md5sum="'.md5($build.$file['name']).'" name="'.$file['name'].'"/>'
    ."\n";
}
$files_str .= '</dir>'."\n";
$xml_cont = str_replace('<contents></contents>','<contents>'.$files_str.'</contents>',$xml_cont);

file_put_contents($build.$name.'.xml', $xml_cont);




//$command = 'php pyrus.phar . package Kiwi.xml';
//echo $command."\n"; passthru($command);
//
//$command =
//  'rsync -av '.
//  './'.$name.'-'.$ver.'.tgz '.
//  'v3project@v3project.ru:kiwi-pear.v3project.ru/get/';
//echo $command."\n"; passthru($command);
//
//$command =
//    'ssh v3project@v3project.ru '.
//    '"cd kiwi-pear.v3project.ru;php pyrus.phar scs-release get/'.$name.'-'.$ver.'.tgz;php pyrus.phar scs-update;"';
//echo $command."\n"; passthru($command);
?>
