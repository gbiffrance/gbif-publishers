<?php
$gbiffr_key = "da44cd31-5901-4687-a106-6d1c7734ee3a";

/* JSON PATH (depending on your Drupal installation)*/
$jsonDir = '/var/www/drupal/sites/all/modules/custom/gbif_publishers/json/';
$organizationFile = $jsonDir."org.json";
$datasetFile	=   $jsonDir."stats.json";

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

function write_file($file,$data)
{
  $myfile = fopen($file, "w") or die("Unable to open file!");
  fwrite($myfile, $data);
  fclose($myfile);
  echo  "Done ! [Saved in ".$file." ] \n";
}
