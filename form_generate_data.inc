<?php 

function gbif_publishers_form_generate() {
  return drupal_get_form('gbif_publishers_generate_form');
}

function gbif_publishers_generate_form($form_state)
{
   $form['description'] = array(
    '#type' => 'markup',
    '#markup' => t('Generate data from GBIF API (this process may takes a few minutes ...) <br />'),
   );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Generate data',
  );

  return $form;
}

function gbif_publishers_generate_form_submit($form, &$form_state) {
     drupal_set_message(t('The form has been submitted.'));
     $batch = gbif_publisher_generate_data();
     batch_set($batch);
}

function gbif_publisher_generate_data()
{
  drupal_set_message(t('Start the process...'));

  $operations[] = array(
      'gbif_publishers_generate_institutions',array());

  $batch = array(
    'operations' => $operations,
    'finished' => 'gbif_publisher_generate_data_finished',
  );

  return $batch;
}


function gbif_publishers_generate_institutions(&$context) {
 include_once('stats.inc');
 $institutions = getInstitutions($gbiffr_key);
 watchdog(); 
 write_file($organizationFile,$institutions);
}

function gbif_publisher_generate_data_finished($success) {
  if ($success){
    drupal_set_message(t('process finished'));
  }
  else {
    drupal_set_message(t('An error occurred while processing'));
  }
}
