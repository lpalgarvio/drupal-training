<?php

/**
 * Display a dropdown of installed modules with the option to reinstall them.
 */
function dummy_test1_form($form, &$form_state) {
  $output = '';
  $modules = module_list();
  sort($modules);
  $options = drupal_map_assoc($modules);
  
  $form['list'] = array(
    '#type' => 'checkboxes',
    '#options' => $options,
    '#description' => t('Uninstall and then install the selected modules. <code>hook_uninstall()</code> and <code>hook_install()</code> will be executed and the schema version number will be set to the most recent update number. You may have to manually clear out any existing tables first if the module doesn\'t implement <code>hook_uninstall()</code>.'),
  );
  $form['submit'] = array(
    '#value' => t('Reinstall'),
    '#type' => 'submit',
  );
  
  return $form;
}

/**
 * Process reinstall menu form submissions.
 */
function dummy_test1_form_submit($form, &$form_state) {
  $modules = array_filter($form_state['values']['list']);
  module_disable($modules, FALSE);
  drupal_uninstall_modules($modules, FALSE);
  module_enable($modules, FALSE);
  drupal_set_message(t('Uninstalled and installed: %names.', array('%names' => implode(', ', $modules))));
}

/**
 * Test #2.
 */
function dummy_test2_form($form, &$form_state) {
  $form = array();
  //$form['#cache'] = FALSE;
  //$form['#theme'] = 'dummy_test2_form';
  
  $form['header']['text1'] = array(
    '#type' => 'textfield',
    '#title' => t('Text'),
    '#description' => t('Dummy text.'),
    '#default_value' => 'hello!',
    '#size' => 20,
  );
  $form['header']['check1'] = array(
    '#type' => 'checkbox',
    '#title' => t('Checkbox'),
    '#description' => t('Dummy checkbox.'),
    '#default_value' => 0,
  );
  
  $form['footer']['submit'] = array(
    '#value' => t('Submit'),
    '#type' => 'submit',
  );
  
  return $form;
}

/**
 * Test #2.
 */

/**
 * Form validation handler for dummy_test2_form().
 *
 */
function dummy_test2_form_validate_save($form, &$form_state) {
  // Get and parse the form values
  $text1 = check_plain($form_state['values']['text1']);
  $check1 = check_plain($form_state['values']['check1']);
  
  // Validate form values
  if (!is_string($text1)) {
    form_set_error('form', t('Invalid string: @value',
      array('@value' => $text1)
    ));
  }
  if (!is_numeric($check1)) {
    form_set_error('form', t('Invalid option: @value',
      array('@value' => $check1)));
  }
}

/**
 * Form submission handler for dummy_test2_form().
 *
 */
function dummy_test2_form_submit_save($form, &$form_state) {
  // Get and parse the form values
  $text1 = check_plain($form['text1']['#value']);
  $check1 = check_plain($form['check1']['#value']);
  
  // Output result to status
  $message = t('Did some stuff: @text1 / @check1', array(
    '@text1' => $text1, '@check1' => $check1));
  drupal_set_message($message, 'status');
  
  // Output result to log
  $log = t('Did some stuff: @text1 / @check1', array(
    '@text1' => $text1, '@check1' => $check1));
  watchdog('dummy', $log, array(), WATCHDOG_DEBUG);
  
  // Return to some form
  drupal_goto('dummy/');
}

/**
 * Theme override for dummy_test2_form.
 * Implements hook_theme().
 *
 */
function theme_dummy_test2_form($form) {
  // Element styling
  $columnClass = 'table-col';
  $columnStyle = "padding: 8px 2px !important; border: 1px solid #ccc; background-color: #f5f5f5; color: #555; text-shadow: #fff 1px 1px 1px; font-size: 1em; font-weight: bold; text-align: center;";
  $cellClass = 'table-row-cell table-row-:class';
  $cellText = '<div class="form-item"><input type="text" class="form-text" value=":value" :disabled/></div>';
  $cellCheckbox = '<div class="form-item" style="text-align: center;"><input type="checkbox" class="form-checkbox" value=":value" :checked :disabled/></div>';
  
  // Populate the table columns
  $cols = array();
  $cols = array(
    array('data' => t('Name'), 'class' => $columnClass, 'style' => $columnStyle, 'width' => '20%'),
    array('data' => t('Status'), 'class' => $columnClass, 'style' => $columnStyle, 'width' => '50px'),
  );
  
  // Populate the table rows
  $status = 1;
  $rows[] = array(
    'data' => array(
      array('data' => strtr($cellText, array(':value' => 'ola!', ':disabled' => 'disabled')), 'class' => strtr($cellClass, array(':class' => 'name'))),
      array('data' => strtr($cellCheckbox, array(':value' => $status, ':disabled' => 'disabled', ':checked' => ($status) ? 'checked' : '')), 'class' => strtr($cellClass, array(':class' => 'status'))),
    ),
  );
  $status = 0;
  $rows[] = array(
    'data' => array(
      array('data' => strtr($cellText, array(':value' => 'adeus!', ':disabled' => 'disabled')), 'class' => strtr($cellClass, array(':class' => 'name'))),
      array('data' => strtr($cellCheckbox, array(':value' => $status, ':disabled' => 'disabled', ':checked' => ($status) ? 'checked' : '')), 'class' => strtr($cellClass, array(':class' => 'status'))),
    ),
  );
  
  // Initialize html
  $html = NULL;
  
  // Render the header section
  $html .= drupal_render($form['header']);
  
  // Render the table
  $html .= theme('table', $cols, $rows, array('class' => 'grid-table'));
  
  // Render the footer section
  $html .= drupal_render($form['footer']);
  
  return $html;
}
