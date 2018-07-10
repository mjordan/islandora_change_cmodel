<?php

/**
 * @file
 * Utility functions used by the Islandora Object Clone module.
 */

/**
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
    drupal_set_message("Sorry, this object has more than one content model so we can't complete this operation.", 'warning');
    return FALSE;
  }

  // First delete the existing hasModel relationship, then add the new hasModel.

  // During development, return TRUE.
  return TRUE;
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
