<?php

/**
 * @file
 * Utility functions used by the Islandora Object Clone module.
 */

/**
 * Ingests a new Islandora object based on the source object.
 *
 * We can't perform a dumb 'clone' of the object because the owner may be
 * different, and the createdDate will certainly be different. So, we will
 * need to copy the properties of the source object and ingest the new
 * object using them; we'll also need to iterate over the source object's
 * datastreams and ingest the new version of each of those as well.
 *
 * We do not copy RELS-EXT or RELS-INT since they are rebuilt using
 * relationships.
 *
 * @param array $form_state
 *   The $form_state from the submitted confirmation form.
 *
 * @return string
 *   The PID of the cloned object.
 */
function islandora_change_cmodel_change_cmodel($form_state) {
  $source_pid = $form_state['values']['islandora_object_clone_source_pid'];

  $source_object = islandora_object_load($source_pid);

  // Let's talk relationships. Copy them all (except for collection
  // relationships) from the source to the new object.
  foreach ($source_object->relationships->get() as $rel) {
    if ($rel['predicate']['value'] != 'isMemberOfCollection') {
      $obj->relationships->add(
        $rel['predicate']['namespace'],
        $rel['predicate']['value'],
        $rel['object']['value']
      );
    }
  }
  // Define collection relationships based on form input values.
  foreach ($confirmed_collections as $collection) {
    $obj->relationships->add(FEDORA_RELS_EXT_URI, 'isMemberOfCollection', $collection);
  }

  return $new_object->id;
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
