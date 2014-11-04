<?php
define("API","http://api.gbif.org/v1/");

function nodeStats($node)
{
  $res = array();
  
  //Returns  count of occurrence for the node
  $url = API."occurrence/count?publishingCountry=FR";
  $output = getCurl($url);
  $totalOccurences = intval($output) ;
  $res["countOccurrences"] = floor($totalOccurences / 1000000);
  
  // Number of dataset published by organizations endorsed by the node
  $url = API."node/".$node."/dataset?limit=0";
  $output = getCurl($url);
  $json =  json_decode($output);
  $res["countDataset"] =  $json->count;
  
  // Number of organizations endorsed by the node
  $url = API."node/".$node."/organization?limit=0";
  $output = getCurl($url);
  $json =  json_decode($output);
  $res["countOrganization"] = $json->count;

  return $res;
}

//Get all institutions endorsed by a node and store results into JSON file 
//TODO: Make changes so endOfRecords is verified as True (for the moment limit parameter is arbitrary set to 1000 which is enough)
function getInstitutions($uuid)
{
  $url = API."node/".$uuid."/organization?limit=2";
  $output = getCurl($url);
  
  return $output;
}


//Receives provider UUID & returns matching datasets UUIDs
function getProviderDatasets($providerName,$uuids=array())
{
    static $i = 0;
    static $j = 0;
    
    $provider = $providerName;
    
    $offset = $j * 20;
    
   //First fetch all the datasets UUIDs via cURL
    $url = API . "organization/" . $provider . "/publishedDataset?offset=" . $offset ;
     
    $output  = getCurl($url);
    
    $json =  json_decode($output);

    //Loop through results to get datasets UUIDs
    foreach($json->results as $res)
    {
      $curRes = array($res->type,$res->key,$res->title);
      array_push($uuids,$curRes);
      $i++;
    }
    
    //Because results are limited by default
    if($json->count > $i)
    {
      $j++;
      return getProviderDatasets($providerName,$uuids);
    }
    else
    {
      unset($i,$j);
      return $uuids;
    }
}



// //TODO: Retrieve DWC url for METADATA & CHECKLIST dataset
// Receives a dataset and returns its stats
function getDatasetMetrics($dataset)
{
  $metrics = array("type"=>$dataset[0],"key"=>$dataset[1],"title"=>$dataset[2]); // store the results
  
  
  //Requests are made according to dataset's type
  //Total number of download stats are unavailable for CHECKLIST and METADATA (not provided with API)
  switch($dataset[0])
  {
    case "CHECKLIST": 
		      $metrics["kingdom"] = array();
		      $url = API . "dataset/".$dataset[1]."/metrics";
		      $res = getCurl($url);
		      $json = json_decode($res);
		      $metrics["total"] = $json->usagesCount;
		      
		      foreach($json->countByKingdom as $name=>$val)
		      {
			array_push($metrics["kingdom"],array($name => $val));
		      }
		
		      break;
    
    case "METADATA":
		      
		      break;

    case "OCCURRENCE":
		       $metrics["kingdom"] = array();
		       $metrics["basisOfRecord"] = array();
		       $requests =  getOccurrenceRequests($dataset[1]);
		    
		      //Loop through each type of occurrence request and fetch result from GBIF API
		      foreach($requests as $type=>$req)
		      {
			    switch($type)
			    {
				case "kingdom" :
				
				case "basisOfRecord" : 
				  foreach($req as $arg)
				  {
				    $key =  array_keys($arg);
				    $url = API.$arg[$key[0]];
				    $res = getCurl($url);
				    
				    //If results
				    if($res>0)
				    { 
				      $count = array($key[0] => $res); 
				      array_push($metrics[$type],$count);
				    }
				  }
				  break;
				
				case "dataset":
				
				  $url = API.$req;
				  $res = getCurl($url);
				  
				  //Fetch endpoint url which type is DWC_ARCHIVE
				  $metrics["dwc_url"] = "";
				  $json = json_decode($res);
				  $endpoints = $json->endpoints;
				  
				  foreach($endpoints as  $v)
				  {
				    if($v->type == "DWC_ARCHIVE")
				    {
				      $metrics["dwc_url"] = $v->url;
				    }
				  }
				
				break;
				
			      default:
				      $url = API.$req;
				      $res = getCurl($url);
				
				      if ($type=="download")
				      {
					$json = json_decode($res);
					$metrics[$type] = $json->count;
				      }
				      else
				      {
					$metrics[$type] = $res;
				      }
				      
				      break;
			    }
		      }
		      
			$metrics["percent_geo"] = 0;
			
		       if(!(div_check($metrics["georef"],$metrics["total"]))) 
		       {
			  $metrics["percent_geo"] = round(($metrics["georef"] / $metrics["total"]) * 100);
		       }
		        
					    
		      break;
  }
  return $metrics;
}
