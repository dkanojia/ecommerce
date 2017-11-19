<?php

// For future refrence
/*else
 {
 echo '<p><a href="'.$base_url.'/user/logout">Logout</a></p>';
}*/

/**
 * @file
 * Inc File for open pop up form for user change password.
 */

/**
 * Ajax menu callback.
 */
function user_popup_form($ajax = NULL) {
  if ($ajax) {
    ctools_include('ajax');
    ctools_include('modal');

    $form_state = array(
      'ajax' => TRUE,
      'title' => t('For change user setting enter your details'),
      // 'node' => $node,
    );

    // Use ctools to generate ajax instructions for the browser to create
    // a form in a modal popup.
    // $output = ctools_modal_form_wrapper('mymodule_form', $form_state);
    $output = ctools_modal_form_wrapper('user_change_password_form', $form_state);
    // If the form has been submitted, there may be additional instructions
    // such as dismissing the modal popup.
    if (!empty($form_state['ajax_commands'])) {
      $output = $form_state['ajax_commands'];
    }

    // Return the ajax instructions to the browser via ajax_render().
    print ajax_render($output);
    drupal_exit();
  }
  else {
    // return drupal_get_form('mymodule_form');
    return drupal_get_form('user_change_password_form');
  }
}

function user_change_password_form($form, &$form_state){

  ctools_include('modal');
  ctools_modal_add_js();

  $form['password'] = array(
    '#type' => 'password_confirm',
    '#title' => 'Please enter your Password',
    '#required' => TRUE,
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'), 
    '#submit' => array('user_change_password_form_submit'),
    '#prefix' => '<div class="pull-left custom-test-program margin-bottom-10">',
    '#suffix' => '</div>', 
  );

  return $form;
}

function user_change_password_form_submit($form, &$form_state){
  // print "<pre>";
  // print_r($form_state);
  // die;
}