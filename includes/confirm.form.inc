<?php

/**
 * @file
 * Handles the uploading of the XML object file as part of the ingest process.
 */

/**
 * Defines a file upload form for uploading the XML file.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 *
 * @return array
 *   The drupal form definition.
 */
function islandora_change_cmodel_confirm_form(array $form, array &$form_state) {
  module_load_include('inc', 'islandora_change_cmodel', 'includes/utilities');
  $object = menu_get_object('islandora_object', 2);

  $form['islandora_change_cmodel_confirm_message'] = array(
    '#markup' => t('You are about to change content model for "' . check_plain($object->label) . '".'),
  );

  $cmodels = islandora_change_cmodel_get_cmodels();
  $form['islandora_change_cmodel_target_cmodel'] = array(
    '#type' => 'select',
     '#title' => t('New content model'),
     '#options' => $cmodels,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit')
  );

  return $form;
}

/**
 * Addes the uploaded file into the ingestable objects 'OBJ' datastream.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 */
function islandora_change_cmodel_confirm_form_submit(array $form, array &$form_state) {
  module_load_include('inc', 'islandora_change_cmodel', 'includes/utilities');
  $source_pid = $form_state['values']['islandora_change_cmodel_source_pid'];
  if ($pid = islandora_change_cmodel_change_cmodel($form_state)) {
    drupal_set_message(t("Object @clone_pid cloned from @source_pid.", array('@clone_pid' => $pid, '@source_pid' => $source_pid)));
    drupal_goto('islandora/object/' . $pid);
  }
}
