<?php

/**
 * @file
 * Utility functions used by the Islandora Change Cmodel module.
 */

/**
 * Form submit callback.
 *
 * @param array $form_state
 *   The $form_state from the submitted confirmation form.
 *
 * @return bool
 *   Whether or not the operation was successful.
 */
function islandora_change_cmodel_change_cmodel($form_state) {
  $pid = $form_state['values']['pid'];
  $new_cmodel = $form_state['values']['islandora_change_cmodel_target_cmodel'];
  $object = islandora_object_load($pid);
  $models = $object->models;
  // How many content models does this object have?
  // First, remove the Fedora system model.
  if (($key = array_search('fedora-system:FedoraObject-3.0', $models)) !== FALSE) {
    unset($models[$key]);
  }
  if (count($models) > 1) {
    drupal_set_message(t("Sorry, this object has more than one content model so we can't complete this operation."), 'warning');
    return FALSE;
  }

  if ($new_cmodel == $models[0]) {
    drupal_set_message(t("You have chosen to change this object's content model to the one it already has, %cmodel.",
      array('%cmodel' => $new_cmodel)), 'error');
    return FALSE;
  }

  $prechange_results = module_invoke_all('islandora_change_cmodel_prechange', $pid, $models[0], $new_cmodel);
  if (in_array(FALSE, $prechange_results)) {
    return FALSE;
  }

  try {
    // First add the new content model relationship.
    $object->relationships->add(FEDORA_MODEL_URI, 'hasModel', $new_cmodel);
    // Then delete the existing hasModel relationship.
    $object->relationships->remove(FEDORA_MODEL_URI, 'hasModel', $models[0]);
    module_invoke_all('islandora_change_cmodel_cmodel_changed', $pid, $models[0], $new_cmodel);
    return TRUE;
  }
  catch (Exception $e) {
    drupal_set_message(t('Unable to change content model: %e', array('%e' => $e->getMessage())), 'error');
  }
}

/**
 * Query the resource index to get a list of all content model objects.
 *
 * @return array
 *   Associative array of pid => label pairs.
 */
function islandora_change_cmodel_get_cmodels() {
  $skip_these = array(
    'fedora-system:FedoraObject-3.0',
    'fedora-system:ServiceDefinition-3.0',
    'fedora-system:ContentModel-3.0',
    'fedora-system:ServiceDeployment-3.0',
  );

  // Query the rindex to get all cmodels.
  $tuque = islandora_get_tuque_connection();
  $ri_query = 'PREFIX fedora-model: <info:fedora/fedora-system:def/model#>
select $object $label from <#ri>
     where { $object fedora-model:hasModel <info:fedora/fedora-system:ContentModel-3.0> ;
      fedora-model:label $label .}';
  $results = $tuque->repository->ri->sparqlQuery($ri_query, 'unlimited');
  $cmodels = array();
  foreach ($results as $member) {
    if (!in_array($member['object']['value'], $skip_these)) {
      $pid = preg_replace('/info:fedora\//', '', $member['object']['value']);
      $label = $member['label']['value'];
      $cmodels[$pid] = $label;
    }
  }
  return $cmodels;
}

/**
 * Checks to see if the current object has any of the allowed content models.
 *
 * @return array
 *   An array containing the content models that the object has that
 *   are allowed.
 */
function islandora_change_cmodel_check_for_cmodels($object) {
  $object_models = $object->models;
  $allowed_models = variable_get('islandora_change_cmodel_allowed_cmodels', array());
  $allowed_models_filtered = array();
  foreach ($allowed_models as $key => $value) {
    if ($value != '0') {
      $allowed_models_filtered[] = $value;
    }
  }
  return array_intersect($object_models, $allowed_models_filtered);
}

/**
 * Redirect user to content model change form.
 */
function islandora_change_cmodel_show_confirm_form($form, $form_state) {
  drupal_goto("islandora/object/{$form_state['object']}/change_cmodel");
}
