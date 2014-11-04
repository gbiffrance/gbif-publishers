#!/bin/bash

drush dis gbif_publishers -y
#drush pm-uninstall gbif_publishers -y
drush en gbif_publishers -y
