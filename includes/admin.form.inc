<?php

/**
 * @file
 * The admin form for the Islandora Change Cmodel module.
 */

/**
 * Defines the admin settings form.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 *
 * @return array
 *   The Drupal form definition.
 */
function islandora_change_cmodel_admin_form(array $form, array &$form_state) {
  module_load_include('inc', 'islandora_change_cmodel', 'includes/utilities');
  $get_default_value = function($name, $default) use(&$form_state) {
    return isset($form_state['values'][$name]) ? $form_state['values'][$name] : variable_get($name, $default);
  };

  $form['islandora_change_cmodel_cmodels'] = array(
    '#title' => t("Allowed content models"),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $cmodels = islandora_change_cmodel_get_cmodels();
  $form['islandora_change_cmodel_cmodels']['islandora_change_cmodel_allowed_cmodels'] = array(
    '#type' => 'checkboxes',
    '#options' => $cmodels,
    '#default_value' => variable_get('islandora_change_cmodel_allowed_cmodels', array()),
    '#description' => t("The 'Change content model' button will only show up for objects with the selected content models."),
  );

  return system_settings_form($form);
}
