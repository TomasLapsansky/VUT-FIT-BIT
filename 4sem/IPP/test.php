<?php
    
    //Arg vals
    $folderArg = getcwd();
    $interpArg = $folderArg."/interpret.py";
    $parseArg = $folderArg."/parse.php";
    $rFlag = false;
    $HTMLHeader = "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">
    <title> IPP - Test Results </title> <style> td,th { border-collapse:collapse;text-align:left}
    </style></head><body><h1>IPP test results</h1><br>";
    $HTMLoutput ="<TABLE style='width: 100%;border:none;border-collapse: collapse;'>
    <tbody><tr style='background-color:#333333;color:white;border-collapse: collapse'>
    <th>number</th><th>Filename</th><th>Success</th><th>Error type</th></tr>";
    $grayback = false;
    
    $success = 0;
    $testCount = 1;
    
    
    $longopts  = array(
                       "directory:",
                       "recursive",
                       "parse-script:",
                       "int-script:",
                       "help",
                       );
    $options = getopt("h", $longopts);
    
    foreach( $options as $arg => $optarg){
        switch($arg){
            case "recursive":
                $rFlag = true;
                break;
            case "directory":
                $rp = realpath($optarg);
                if($rp != false){
                    $folderArg = $rp;
                }
                break;
            case "int-script":
                echo($optarg."\n");
                $rp = realpath($optarg);
                if($rp != false){
                    $interpArg = $rp."/interpret.py";
                }
                break;
            case "parse-script":
                $rp = realpath($optarg);
                if($rp != false){
                    $parseArg = $rp."/parse.php";
                }
                break;
            case "help":
                echo("Toto je testovaci skript IPP, argumenty jsou:\n
                     --directory,--int-script,parse-script a --recursive\n");
                     exit(0);
                     default:
                     break;
                     }
                     }
                     
                     function HtmlAdd($name,$res,$type,$rcExp,$rcGot){
                     $name = explode("/",$name);
                     $name = end($name);
                     global $grayback,$HTMLoutput,$testCount,$success;
                     if(!$res){
                     $lineStr = "<tr style='color:red";
                     }
                     else{
                     $lineStr = "<tr style='color:green";
                     }
                     if(!$grayback){
                     $lineStr = $lineStr.";background-color:#DDDDDD'>";
                     }
                     else{
                     $lineStr = $lineStr.";background-color:white'>";
                     }
                     $lineStr = $lineStr."<td>".$testCount."<td><i>".$name."</i></td>";
                     if(!$res){
                     $lineStr = $lineStr."<td>No</td>";
                     if($type == "Exit code match error: expected "){
                     $lineStr = $lineStr."<td>".$type.$rcExp."| gotten ". $rcGot."</td>";
                     }
                     else{
                     $lineStr = $lineStr."<td>".$type."</td>";
                     }
                     
                     }
                     else{
                     $lineStr = $lineStr."</td><td>Yes</td>";
                     $lineStr = $lineStr."<td></td>";
                     $success++;
                     }
                     
                     $lineStr = $lineStr."</tr>";
                     $HTMLoutput = $HTMLoutput.$lineStr."\n";
                     $grayback = !$grayback;
                     $testCount++;
                     }
                     
                     function compFiles($srcName){
                     global $parseArg,$interpArg;
                     if(substr($srcName, -2) == ".." || substr($srcName, -1) == "."){
                     return False;
                     }
                     if(substr($srcName, -4) == ".src" ){
                     $Name =substr($srcName, 0,-4);
                     $outp = str_replace(".src",".out",$srcName);
                     $rc = str_replace(".src",".rc",$srcName);
                     $in = str_replace(".src",".in",$srcName);
                     if(!file_exists($outp)){
                     $filePtr = fopen($outp, "w") or die("Error: Cannot create file");
                     fclose($filePtr);
                     }
                     if(!file_exists($rc)){
                     $filePtr = fopen($rc, "w") or die("Error: Cannot create file");
                     fwrite($filePtr,'0');
                     fclose($filePtr);
                     }
                     if(!file_exists($in)){
                     $filePtr = fopen($in, "w") or die("Error: Cannot create file");
                     fclose($filePtr);
                     }
                     
                     //read corresponding values
                     $filePtr = fopen($rc, "r") or die("Error: Cannot create file");
                     $expRc = fread($filePtr,2);
                     $expRc = intval($expRc);
                     fclose($filePtr);
                     
                     exec("cat ".$srcName." | php ".$parseArg ." > TempFileOutp", $ParseOut,$rcParser);
                     if($rcParser){
                     HtmlAdd($Name,False,"Chyba Parseru",0,0);
                     }
                     else {
                     exec("cat " . $in . "| python3 " . $interpArg . " --source=TempFileOutp > TempFileOutp2", $result, $exitCode);
                     if ($expRc == $exitCode) {
                     if($exitCode == 0){
                     exec("diff -q TempFileOutp2 ".$outp,$diffout,$diffRet);
                     if($diffRet){
                     HtmlAdd($Name,False,"Interpret output match error",0,0);
                     }
                     else{
                     HtmlAdd($Name,True,"Diff",0,0);
                     }
                     }
                     else{
                     HtmlAdd($Name,True,"RC",0,0);
                     }
                     } else {
                     HtmlAdd($Name,False,"Exit code match error: expected ",$expRc,$exitCode);
                     }
                     }
                     unlink("TempFileOutp");
                     }
                     return True;
                     }
                     
                     
                     $folderArg = realpath ( $folderArg );
                     if($rFlag){
                     $objects = new RecursiveIteratorIterator(new RecursiveDirectoryIterator($folderArg), RecursiveIteratorIterator::SELF_FIRST);
                     foreach($objects as $name => $object) {
                     compFiles($name);
                     }
                     }
                     else{
                     $ret = glob($folderArg.'/*.src', GLOB_BRACE);
                     foreach ($ret as $filename){
                     compFiles($filename);
                     }
                     }
                     if($testCount > 1){
                     $HTMLoutput = $HTMLHeader."<h2>Succes rate: ". 100*$success/($testCount-1)."% (".$success." / ".$testCount.") </h2> ".$HTMLoutput."</tbody></table></body></html>";
                     }
                     echo($HTMLoutput);
                /*
                 $web = fopen("test-out.html","w");
                 fwrite($web,$HTMLoutput,strlen($HTMLoutput));
                 fclose($web);*/
    ?>
