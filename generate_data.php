<?php 
include_once("config.inc");
include_once("stats.inc");


// Generate a JSON file containing all publishers stats 

// Get all organizations endorsed by GBIF France
// Loop through all organizations
// Get their datasets => getProviderDatasets
// Request GBIF API to get dataset's metrics  => getDatasetMetrics
// Store all datasets metrics into a json
// Loop until providers list completed
// Return a well formed json storing all providers uuid/name, datasets name/uuid, dataset metrics 
// Then use generated JSON into Drupal to read stats

$exec_date = date("d/m/Y");


echo "This script will generate two JSON files (1 for endorsed organization and 1 for datasets stats for each org.)  \n\n";
echo "1st STEP : Fetch all organization endorsed  ... \n";
$institutions = getInstitutions($node_key);
write_file($organizationFile,$institutions);


echo "2nd STEP : Fetch all datasets information for organization endorsed ... \n";
$i = 1;
$objInstitutions = json_decode($institutions);
$total_element = count($objInstitutions->results);
$datasets_list = array("script_exec_date"=>$exec_date,"datasets_list"=>array());

foreach($objInstitutions->results as $k=>$v)
{

  $institution = array("institution_name"=>$v->title,
			"institution_uuid"=>$v->key);
			
  $institution["datasets_list"] = array();
  $datasets = getProviderDatasets($v->key);

  foreach($datasets as $ds)
  {
    array_push($institution["datasets_list"],getDatasetMetrics($ds)); 
  }

   array_push($datasets_list["datasets_list"],$institution);    
   $i++;
   show_status($i, $total_element);
}


$json = json_encode($datasets_list);

write_file($datasetFile,$json);

?>
