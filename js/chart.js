(function ($) {
  
  $(document).ready(function (){
    
	//todo: set 1 color =  1 kingdom in pie-chart
   console.log($("#kingdom-chart"));      
   var json =  JSON.parse($(".stat_provider").html(),function(prop,value){
	switch(prop)
        {
	case "Other":
		this.Autres = value;
		return;
        case "Viruses":
		this.Virus = value;
		return;
	  default:
		return value;
        }
   
  });  
   
   var kingdomData=$.map( json.kingdom, function( obj,i){       

	return [[ i  , parseInt(json.kingdom[i]) ]];
  })
  
  console.log(kingdomData);
  var cptOcc = 0;
  resBor = new Object();
   
  $.each( json.basisOfRecord, function( obj,i){       

    switch(obj) 
    {
	case 'HUMAN_OBSERVATION':
	     resBor["Observation_humaine"] = i;	     
	     
	break;
	
	case 'OBSERVATION':
	     resBor["Observation"] = i ;
	break;
	
	case 'PRESERVED_SPECIMEN':
	     resBor["Specimen_preservé"] = i;
	break;
	
	case 'UNKNOWN':
	     resBor["Inconnu"] = i;
	break;
	
	case 'FOSSIL_SPECIMEN':
	     resBor["Fossile"] = i;
	break;
	
	case 'LIVING_SPECIMEN':
	 resBor["Specimen_vivant"] = i;
	break;
	
	case 'LITERATURE':
	  resBor["Littérature"] = i;
	break;
	
	case 'MATERIAL_SAMPLE':
	 resBor["Echantillon"] = i;
	break;
    }

    cptOcc  += i;
  })

 

    if (kingdomData.length != 0)
    {
     $('#kingdom-chart').highcharts({
	  chart: {
	      plotBackgroundColor: null,
	      plotBorderWidth: null,
	      plotShadow: false
	  },
	  title: {
	      text: 'Distribution des occurrences par royaume'
	  },
	  tooltip: {
	      pointFormat: '{point.name}: <b>{point.y:f} occurrences</b>'
	  },
	  credits:{
          enabled:false
          },
	  plotOptions: {
	      pie: {
		  allowPointSelect: true,
		  cursor: 'pointer',
		  dataLabels: {
		      enabled: true,
		      format: '<b>{point.name}</b>: {point.percentage:.1f} %',
		      style: {
			  color: (Highcharts.theme && Highcharts.theme.contrastTextColor) || 'black'
		      }
		  }
	      }
	  },
	  series: [{
	      type: 'pie',
	      name: '',
	      data:  kingdomData
	  }]
      });

   }
   else{
			$("#kingdom-chart").hide();
   }
  
	
    if (cptOcc != 0)
    {
      
      var list = $("#bor-chart").append('<ul></ul>').find('ul');
	  
      $.each( json.basisOfRecord, function(obj,i){
	    percent = Math.floor((i/cptOcc)*100);
	    name = obj.replace(/_/g, " ");
	    list.append('<li>'+name + ' : '  + i + ' occurrences </li>');
	
      });
         
    }  
   

  });    
	
	
})(jQuery);
