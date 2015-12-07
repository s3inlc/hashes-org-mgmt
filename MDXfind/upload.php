<?php
/*
 * Created 2015 by Sein Coray
 *
 * https://github.com/s3inlc/hashes-org-mgmt/tree/master/MDXfind
 */
 
if(!isset($argv[1])){
	die("Basename required!");
}

$dir = getcwd();
$basename = $argv[1];

$url = "https://hashes.org/crackers.php";
$loginurl = "https://hashes.org/login.php";
$username = $argv[2];

echo "Please enter the password for '$username':\n";
$password = readline();

$ch = curl_init();
curl_setopt($ch, CURLOPT_URL, $loginurl);
curl_setopt($ch, CURLOPT_POST, 1);
curl_setopt($ch, CURLOPT_COOKIEFILE, "cookie.txt");
curl_setopt($ch, CURLOPT_POSTFIELDS, "username=$username&password=$password");
curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
curl_setopt($ch, CURLOPT_SAFE_UPLOAD, false);

$server_output = curl_exec($ch);
if(strpos($server_output, "login_form") !== false){
	die("Login failed!\n");
}

$dir = scandir($dir);
$count = 0;
$size = sizeof($dir);
foreach($dir as $file){
	if(strpos($file, $basename) !== 0){
		continue;
	}
	if(strpos($file, "OTHER") !== false){
		continue;
	}
	upload($file);
	echo "$count/$size   \r";
	$count++;
}

echo "Uploaded $count files!\n";


function upload($file){
	global $ch, $url, $basename;
	
	$file_name_with_full_path = realpath($file);
	$algorithm = str_replace(".txt", "", str_replace($basename, "", $file));
	$post = array('found_upload' => 'send', 'found_file' => '@'.$file_name_with_full_path, "algorithm" => $algorithm);
	curl_setopt($ch, CURLOPT_URL, $url);
	curl_setopt($ch, CURLOPT_POST, 1);
	curl_setopt($ch, CURLOPT_COOKIEFILE, "cookie.txt");
	curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
	curl_setopt($ch, CURLOPT_POSTFIELDS, $post);
	curl_setopt($ch, CURLOPT_SAFE_UPLOAD, false);
	$server_output = curl_exec($ch);
}



