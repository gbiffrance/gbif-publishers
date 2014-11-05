<?php
// MODIFY NODE KEY HERE (default is GBIF France Node key)
// => $node_key 
$node_key = "da44cd31-5901-4687-a106-6d1c7734ee3a";

/* JSON PATH */
$organizationFile = dirname(__FILE__) .'/json/org.json';
$datasetFile = dirname(__FILE__)  .'/json/stats.json';

$basisOfRecords = array("HUMAN_OBSERVATION", "OBSERVATION", "PRESERVED_SPECIMEN", "UNKNOWN", "FOSSIL_SPECIMEN",  "LIVING_SPECIMEN", "MACHINE_OBSERVATION", "LITERATURE","MATERIAL_SAMPLE");

$kingdoms = array(0 => "Other", 1 => "Animalia", 2 => "Archaea",3 => "Bacteria",4 =>"Chromista",5 =>"Fungi", 6 =>"Plantae",7 =>"Protozoa", 8 =>"Viruses");

function getCurl($url)
{
  $ch = curl_init();
  curl_setopt_array($ch, array(CURLOPT_URL => $url,CURLOPT_RETURNTRANSFER => true));
  return curl_exec($ch);
}

//Setup all necessary requests
function getOccurrenceRequests($uuid)
{
    $requests["total"] = "occurrence/count?datasetKey=" . $uuid;
  
    $requests["georef"] =  $requests["total"] . "&isGeoreferenced=true";
    
    $requests["download"] = "occurrence/download/dataset/".$uuid."?limit=0";
    
    $requests["dataset"] = "dataset/" . $uuid ;
    
    $requests["basisOfRecord"] = array();
    
    $requests["kingdom"] = array();
    
    foreach ($GLOBALS["basisOfRecords"] as $basisOfRecord)
    {
	$curReq = array($basisOfRecord => $requests["total"]."&basisOfRecord=" . $basisOfRecord);
	array_push($requests["basisOfRecord"],$curReq);
    }
    
    foreach($GLOBALS["kingdoms"] as $key=>$kingdomName)
    {
      array_push($requests["kingdom"],array($kingdomName => $requests["total"] ."&taxonKey=".$key ));
    }
    

    return $requests;
}

//credits : http://rosettacode.org/wiki/Detect_division_by_zero#PHP
function div_check($x, $y) {
  @trigger_error(''); // a dummy to detect when error didn't occur
  @($x / $y);
  $e = error_get_last();
  return $e['message'] != '';
}


//credits: http://snipplr.com/view/29548/
function show_status($done, $total, $size=30) {

    static $start_time;

    // if we go over our bound, just ignore it
    if($done > $total) return;

    if(empty($start_time)) $start_time=time();
    $now = time();

    $perc=(double)($done/$total);

    $bar=floor($perc*$size);

    $status_bar="\r[";
    $status_bar.=str_repeat("=", $bar);
    if($bar<$size){
        $status_bar.=">";
        $status_bar.=str_repeat(" ", $size-$bar);
    } else {
        $status_bar.="=";
    }

    $disp=number_format($perc*100, 0);

    $status_bar.="] $disp%  $done/$total";

    $rate = ($now-$start_time)/$done;
    $left = $total - $done;
    $eta = round($rate * $left, 2);

    $elapsed = $now - $start_time;

    $status_bar.= " remaining: ".number_format($eta)." sec.  elapsed: ".number_format($elapsed)." sec.";

    echo "$status_bar  ";

    flush();

    // when done, send a newline
    if($done == $total) {
        echo "\n";
    }

}

function write_file($file,$data)
{
  $myfile = fopen($file, "w") or die("Unable to open file!");
  fwrite($myfile, $data);
  fclose($myfile);
  echo  "Done ! [Saved in ".$file." ] \n";
}

?>
