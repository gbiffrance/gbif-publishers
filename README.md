gbif-publishers
===============

A Drupal module providing a GBIF node information about datasets ([using GBIF API](www.gbif.org/developer/summary)). 


This is a project following GBIF FR/PT/ES mentoring organized in Paris in October 2014. 

This module is an adaptation of the code used in the french GBIF website to display data-publisher statistics and 
highlight their contribution to the GBIF network.


Note that all data are retrieved one time from the API and then are read locally (due to performance).
You can add the command "***php generate_data.php***" to your CRON to update data each week for example.

## How-to use

* 1) Module should be first installed in your Drupal installation folder (copy/paste all files from the repo)
```
/your-drupal-installation/sites/all/modules/custom/gbif-publishers
```

* 2) Change your node key in **config.php**
```
$node_key = ""; // SET YOUR NODE KEY HERE
```

* 3) Execute PHP script that retrieves data from GBIF API and store it under ./json folder (this may take a few minutes to process)
```
php generate_data.php
```

* 4) Enable the module in your drupal module admin page (Configuration > GBIF Publishers) 

* 5) Once activated, this module should create a menu called 'GBIF Providers menu'. Go to Structure > Menu  to make sure it is present.

* 6) You can now display this menu in your site, using block for instance (Structure > Block)
