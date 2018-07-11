<?php
/**
 * @file
 * This file documents hook functions defined by Islandora Change Cmodel.
 */

/**
 * Fires just before the content model is changed. Can block the change.
 *
 * @param string $pid
 *   The PID of the object whose content model has just been changed.
 * @param string $old_cmodel
 *   The PID of the content model that is being removed.
 * @param string $new_cmodel
 *   The PID of the content model that is being added.
 *
 * @return bool
 *   TRUE to not block the change, FALSE to block the change.
 */
function mymodule_islandora_change_cmodel_prechange($pid, $old_cmodel, $new_cmodel) {
  global $user;
  if ($user->name == 'rusty' && $pid == 'islandora:100') {
    drupal_set_message("Rusty, I forbid you from changing this node's content model", 'error');
    return FALSE;
  }
  else {
    // Everyone else is allowed to change the cmodel.
    return TRUE;
  }
}

/**
 * Informs modules that an object's content model has been changed.
 *
 * @param string $pid
 *   The PID of the object whose content model has just been changed.
 * @param string $old_cmodel
 *   The PID of the content model that is being removed.
 * @param string $new_cmodel
 *   The PID of the content model that is being added.
 */
function mymodule_islandora_change_cmodel_cmodel_changed($pid, $old_cmodel, $new_cmodel) {
  if ($old_cmodel == 'islandora:collectionCModel') {
    drupal_set_message(t("All members of this collection are no longer 'in' the collection."));
  }

  // You could loop through all the member objects and remove the relationship
  // with the collection with $pid, for example.
}
